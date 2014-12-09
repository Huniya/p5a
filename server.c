#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "udp.h"
#include "mfs.h"
#include "common.h"

//From http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )            
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

int main(int argc, char *argv[])
{
	if (argc != 3) {
		return 0;
	}
	int port = atoi(argv[1]);
	char filename[60];
	strcpy(filename, argv[2]);

	int fd = UDP_Open(port);
	struct sockaddr_in addr;
	
	UDP_FillSockAddr(&addr, "localhost", port);

	int filesystem = open(argv[2], O_CREAT | O_RDWR);

	if (fd < 0 || filesystem < 0)
		return -1;

	char meta_blocks[3*BSIZE];
	struct superblock* sb = (struct superblock*) &meta_blocks[0*BSIZE];
	struct dinode* inodes = (struct dinode*) &meta_blocks[1*BSIZE];
	struct bitmap* bm = (struct bitmap*) &meta_blocks[2*BSIZE]; //first 64 inodes, next 1024 data

	int result = pread(filesystem, meta_blocks, 3*BSIZE, 0);

	printf("number of blocks is %d\n", sb->nblocks);

	if (result == 0) {
		SetBit(bm->inodes, 0);
		SetBit(bm->data, 0);
		sb->ninodes = 1;
		sb->size = 1;
		sb->nblocks = 4; //superblock, dinode block, bitmap, data block for root inode
		inodes->type = MFS_DIRECTORY;
		inodes->addrs[0] = 3;

		struct nameblock root;		
		strcpy(root.names[0].name, ".");
		strcpy(root.names[1].name, "..");
		root.names[0].inum = 0;
		root.names[1].inum = 0;
		int i;
		for (i = 2; i < 64; i++){
			root.names[i].inum = -1;
		}

		int asdf = pwrite(filesystem, meta_blocks, 3*BSIZE, 0);
		pwrite(filesystem, &root, 1*BSIZE, 3*BSIZE);
		printf("writing %d\n", asdf);
	}

	printf("Running server on port %d with file %s\n", port, argv[2]);

	message msg;
	while (UDP_Read(fd, &addr, (char*) &msg, sizeof(message)) != -1) {
		// msg = (message*) readbuffer;
		response res;
		if (msg.cmd[0] == LOOKUP) {
			int blocknum = (inodes + msg.inum)->addrs[0];
			struct nameblock nb;
			int result = pread(filesystem, &nb, 1*BSIZE, blocknum*BSIZE);
			int i;
			int inum = -1;
			if (result != -1) {
				for (i = 0; i < 64; i++)
				{	
					printf("Looking for '%s', found '%s'\n", msg.name, nb.names[i].name);
					if (strcmp(nb.names[i].name, msg.name) == 0){
						inum = nb.names[i].inum;
					}
				}
			}
			res.rc = inum;
			printf("Lookup inum %d\n", inum);
			UDP_Write(fd, &addr, (char*) &res, sizeof(response));
		} else if (msg.cmd[0] == CREAT) {
			struct dinode* pinode = (inodes + msg.inum);
			int blocknum = pinode->addrs[0];
			struct nameblock nb;
			int result = pread(filesystem, &nb, 1*BSIZE, blocknum*BSIZE);
			int i;
			int newinum;

			for (i = 0; i < 64; i++)
			{
				if (!TestBit(bm->inodes, i)) {
					SetBit(bm->inodes, i);
					newinum = i;
					break;
				}
			}

			int newdatablocknum;
			struct dinode* newdinode = (inodes + newinum);
			for (i = 0; i < 1024; i++)
			{
				if (!TestBit(bm->data, i)) {
					SetBit(bm->data, i);
					newdatablocknum = i;
					break;
				}
			}
			newdinode->type = msg.type;
			newdinode->size = 0;
			newdinode->addrs[0] = newdatablocknum;

			sb->ninodes++;
			sb->size++;
			sb->nblocks++;

			if (newdinode->type == MFS_DIRECTORY) {
				struct nameblock newnb;		
				strcpy(newnb.names[0].name, ".");
				strcpy(newnb.names[1].name, "..");
				newnb.names[0].inum = newinum;
				newnb.names[1].inum = msg.inum;
				pwrite(filesystem, &newnb, 1*BSIZE, newdatablocknum*BSIZE);
			}

			if (result != -1) {
				for (i = 0; i < 64; i++)
				{
					if (nb.names[i].inum == -1){
						strcpy(nb.names[i].name, msg.name);
						printf("****%d\n", newinum);
						nb.names[i].inum = newinum;
						break;
					}
				}
			}

			pwrite(filesystem, &nb, 1*BSIZE, blocknum*BSIZE);
			pwrite(filesystem, meta_blocks, 3*BSIZE, 0);
			fsync(filesystem);
			UDP_Write(fd, &addr, (char*) &res, sizeof(response));
		} else if (msg.cmd[0] == SHUTDOWN) {
			res.rc = 200;
			fsync(filesystem);
			UDP_Write(fd, &addr, (char*) &res, sizeof(response));
			return 0;
		} else {
			printf("Invalid type\n");
		}
	}
	return 0;
}