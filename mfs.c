#include "mfs.h"
#include "udp.h"
#include "common.h"

struct sockaddr_in addr;
int fd;

int main() {
	return 0;
}
int command(message* msg, response* res) {
	UDP_Write(fd, &addr, (char*) msg, sizeof(message));
	UDP_Read(fd, &addr, (char*) res, sizeof(response));
	return 0;
}
int MFS_Init(char *hostname, int port)
{
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	UDP_FillSockAddr(&addr, hostname, port);
	return 0;
}
int MFS_Lookup(int pinum, char *name)
{
	if (strlen(name) > 60) {
		return -1;
	}
	message msg;
	response res;
	msg.cmd[0] = LOOKUP;
	msg.inum = pinum;
	strcpy(msg.name, name);
	command(&msg, &res);
	return res.rc;
}
int MFS_Stat(int inum, MFS_Stat_t *m)
{
	message msg;
	response res;
	msg.cmd[0] = STAT;
	msg.inum = inum;
	command(&msg, &res);
	memcpy(m,&res.stat,sizeof(MFS_Stat_t));
	m = &res.stat;
	return 0;
}
int MFS_Write(int inum, char *buffer, int block)
{
	message msg;
	response res;
	msg.cmd[0] = WRITE;
	msg.inum = inum;
	msg.blocknum = block;
	memcpy(msg.block, buffer, sizeof(char[4096]));
      // printf("msgblock '%s'\n", msg.block);
	command(&msg, &res);
	if (res.rc == -1) {
		return -1;
	}
	return 0;
}
int MFS_Read(int inum, char *buffer, int block)
{
	message msg;
	response res;
	msg.cmd[0] = READ;
	msg.inum = inum;
	msg.blocknum = block;
	command(&msg, &res);
	if (res.rc == -1) {
		return -1;
	}
	memcpy(buffer, res.block, sizeof(res.block));
	return 0;
}
int MFS_Creat(int pinum, int type, char *name)
{
	if (strlen(name) > 60) {
		return -1;
	}
	message msg;
	response res;
	msg.cmd[0] = CREAT;
	msg.type = type;
	msg.inum = pinum;
	strcpy(msg.name, name);
	command(&msg, &res);
	if (res.rc == -1) {
		return -1;
	}
	return 0;
}
int MFS_Unlink(int pinum, char *name)
{
	if (strlen(name) > 60) {
		return -1;
	}
	message msg;
	response res;
	msg.cmd[0] = UNLINK;
	msg.inum = pinum;
	strcpy(msg.name, name);
	command(&msg, &res);
	if (res.rc == -1) {
		return -1;
	}
	return res.rc;
}
int MFS_Shutdown()
{
	message msg;
	response res;
	msg.cmd[0] = SHUTDOWN;
	command(&msg, &res);
	return 0;
}