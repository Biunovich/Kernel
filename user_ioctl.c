#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define IOC_MAGIC    'h' 
#define IOCTL_GET_STRING _IOR(IOC_MAGIC, 1, char *) 
#define IOCTL_SET_STRING _IOW(IOC_MAGIC, 0, char *)
void main()
{
	int fd;
	fd = open("/dev/ioctl", O_RDWR);
	char buf[512];
	char *msg = "Hellow kernel";
	ioctl(fd, IOCTL_SET_STRING, msg);
	ioctl(fd, IOCTL_GET_STRING, buf);
	printf("%s\n", buf);
	close(fd);
}