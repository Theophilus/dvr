
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#define CRYPT_CREATE _IO('q', 1)
#define CRYPT_WRITE _IO('q',2)

int main(int argc, char *argv[])
{
  int fd = 0;
  int ret = 0;
  char buff[80] = "";
  
  fd = open("/dev/cryptctl",O_RDWR);
  
  ioctl(fd, CRYPT_CREATE);
  ret = open("/dev/Encrypt0",O_RDWR);
  ioctl(ret, CRYPT_WRITE);
  //sprintf(buff, "%s%s", argv[1],argv[2]);
  //printf("%s\n", buff);
  //printf("%d\n", (int)strlen(buff));
  //write(ret, buff, strlen(buff));

  close(fd);
  close(ret);
  return 0;
}
