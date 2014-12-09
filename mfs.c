#include "mfs.h"
#include "udp.h"
#include "common.h"

struct sockaddr_in addr;
int fd;

int main() {
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
	message msg;
	response res;
	msg.type = LOOKUP;
	UDP_Write(fd, &addr, (char*) &msg, sizeof(message));
	UDP_Read(fd, &addr, (char*) &res, sizeof(response));
	printf("Client rc %d\n", res.rc);
	return 0;
}
int MFS_Stat(int inum, MFS_Stat_t *m)
{
	
	return 0;
}
int MFS_Write(int inum, char *buffer, int block)
{

	return 0;
}
int MFS_Read(int inum, char *buffer, int block)
{

	return 0;
}
int MFS_Creat(int pinum, int type, char *name)
{

	return 0;
}
int MFS_Unlink(int pinum, char *name)
{

	return 0;
}
int MFS_Shutdown()
{
	message msg;
	response res;
	msg.type = SHUTDOWN;
	UDP_Write(fd, &addr, (char*) &msg, sizeof(message));
	UDP_Read(fd, &addr, (char*) &res, sizeof(response));
	return 0;
}