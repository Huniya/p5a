#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "udp.h"
#include "mfs.h"

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
	printf("Running server on port %d with file %s\n", port, argv[2]);

	if (fd < 0)
		return -1;

	// char writebuffer[sizeof(response)];
	char readbuffer[sizeof(message)];
	while (UDP_Read(fd, &addr, readbuffer, sizeof(readbuffer)) != -1) {
		printf("readbuffer %s\n", readbuffer);
	}
	return 0;
}