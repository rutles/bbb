// i2c7segtest.c - 7seg.LED board handling example
// compile: cc i2c7segtest.c -o i2c7segtest
// execute: ./i2c7segtest

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define I2C_BUS "/dev/i2c-1"
#define I2C_ADRS 0x69

union i2c_regs {
  uint8_t b[4];
  float f;
};

main(){
  union i2c_regs bufs;
  union i2c_regs bufr;
  float n;
  int fd;

  printf("I2C 7seg test.\n\n");
  fd = open(I2C_BUS, O_RDWR);
  if(fd < 0){
    perror("open");
    return;
  }

  ioctl(fd, I2C_SLAVE, I2C_ADRS);
  for(n = -1; n <= 1; n += 0.25){
    bufs.f = n;
    write(fd, bufs.b, 4);

    read(fd, bufr.b, 4);
    printf("%5.3f\n", bufr.f);
    sleep(1);
  }
  close(fd);
}
