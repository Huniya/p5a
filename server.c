#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "udp.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		return 0;
	}
	int port = atoi(argv[1]);
	char filename[60];
	strcpy(filename, argv[2]);
	printf("Running server on port %d with file %s\n", port, argv[2]);
	int fd = UDP_Open(port);
	while (fd > 0) {
		printf("fd %d\n", fd);

	}
	return 0;
}