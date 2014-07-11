#include <i2clcd.h>

int lcd_open(){
  int fd;

  fd = open(I2C_BUS, O_RDWR);
  if(fd >= 0){
    if(ioctl(fd, I2C_SLAVE, 0x3e) >= 0){
      return fd;
    }
  }
  return -1;
}
