#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "udp.h"
#include "mfs.h"
#include "common.h"

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
	// char readbuffer[sizeof(message)];

	printf("Running server on port %d with file %s\n", port, argv[2]);

	message msg;
	while (UDP_Read(fd, &addr, (char*) &msg, sizeof(message)) != -1) {
		// msg = (message*) readbuffer;
		response res;
		printf("readbuffer type %d\n", msg.type);
		if (msg.type == LOOKUP) {
			res.rc = 200;
			UDP_Write(fd, &addr, (char*) &res, sizeof(response));
		} else if (msg.type == SHUTDOWN) {
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