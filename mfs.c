#include "mfs.h"
#include "udp.h"
int main() {
	return 0;
}
int MFS_Init(char *hostname, int port)
{
	struct sockaddr_in addr;
	int fd = UDP_FillSockAddr(&addr, hostname, port);
	
	return fd;
}
int MFS_Lookup(int pinum, char *name)
{

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

	return 0;
}