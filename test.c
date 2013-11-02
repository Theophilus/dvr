#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "crypt_ioctl.h"

int main()
{
  int fd = 0;
  int ret = 0;
  char buff[80] = "";
  
  fd = open("/dev/cryptctl",O_RDWR);
  
  ioctl(fd, CRYPT_CREATE);
  ret = open("/dev/Encrypt0",O_RDWR);
  ioctl(ret, CRYPT_WRITE);
  
  close(fd);
  close(ret);
}
