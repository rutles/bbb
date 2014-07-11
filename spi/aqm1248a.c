// aqm1248a - AQM1248A handling example
// compile: cc aqm1248a.c -o aqm1248a -lbbb
// execute: ./aqm1248a

#include <bbb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define CONTRUST 0x1c
#define CAPE_DTBO "BB-SPIDEV0"
#define SPI_CH "/dev/spidev1.0"
#define GPIO_RS 48

struct spi_ioc_transfer tr ={
  .rx_buf = (uint32_t)NULL,
  .len = 1,
};

int transfer(int fd, uint8_t value){
  int ret;

  tr.tx_buf = (uint32_t)&value;
  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if(ret < 1){
    perror("SPI transfer");
  }
  return ret;
}

int main(int argc, char *argv[]){
  int i, j;
  int sfd;
  int pfd;

  cape_add(CAPE_DTBO);
  sfd = open(SPI_CH, O_RDWR);
  if(sfd < 0){
    perror("SPI open");
    return -1;
  }

  gpio_export(GPIO_RS);
  pfd = gpio_open(GPIO_RS, "direction", O_WRONLY);
  if(pfd < 0){
    perror("GPIO open");
    return -1;
  }
  write(pfd, "out", 3);
  close(pfd);
  pfd = gpio_open(GPIO_RS, "value", O_WRONLY);

  write(pfd, "0", 1);
  transfer(sfd, 0xae);
  transfer(sfd, 0xa0);
  transfer(sfd, 0xc8);
  transfer(sfd, 0xa3);
  transfer(sfd, 0x2c); usleep(2000);
  transfer(sfd, 0x2e); usleep(2000);
  transfer(sfd, 0x2f);
  transfer(sfd, 0x23);
  transfer(sfd, 0x81);
  transfer(sfd, CONTRUST);
  transfer(sfd, 0xa4);
  transfer(sfd, 0x40);
  transfer(sfd, 0xa6);
  transfer(sfd, 0xaf);

  for(i = 0; i <= 5; i++){
    write(pfd, "0", 1);
    transfer(sfd, 0xb0 | i);//page
    transfer(sfd, 0x10);//column 0
    transfer(sfd, 0x00);

    write(pfd, "1", 1);
    for(j = 0; j < 64; j++){
      transfer(sfd, 0x55);
      transfer(sfd, 0xaa);
    }
  }

  close(pfd);
  gpio_unexport(GPIO_RS);
  close(sfd);
  return 0;
}
