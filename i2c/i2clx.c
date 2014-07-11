// i2clx.c - S9705 board handling example
// compile: cc i2clx.c -o i2clx
// execute: ./i2clx

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define I2C_BUS "/dev/i2c-1"
#define I2C_ADRS 0x70

main(){
  int fd;
  union{
    uint8_t b[4];
    uint32_t w;
  } buf;
  float lx;

  printf("brightness\n\n");

  fd = open(I2C_BUS, O_RDWR);
  if(fd < 0){
    perror("open");
    return;
  }

  ioctl(fd, I2C_SLAVE, I2C_ADRS);

  do{
    read(fd, buf.b, 4);

    lx = (float)buf.w / 500;
    printf("lx=%f\n", lx);
    sleep(1);
  } while(buf.w > 5000);

  close(fd);
}
