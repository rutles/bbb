// glcd.c AQM1248A handler source file

#include <bbb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdarg.h>
#include "glcd.h"

int gl_pfd;
int gl_sfd;
struct spi_ioc_transfer tr ={
  .rx_buf = (uint32_t)NULL
};
uint8_t bitmap[6][GL_WIDTH];

#define COMMAND() write(gl_pfd, "0", 1)
#define DRAWING() write(gl_pfd, "1", 1)

int gl_xfer(uint8_t value){
  tr.tx_buf = (uint32_t)&value;
  tr.len = 1;
  return ioctl(gl_sfd, SPI_IOC_MESSAGE(1), &tr);
}

int gl_xfers(uint8_t *array, uint8_t len){
  tr.tx_buf = (uint32_t)array;
  tr.len = len;
  return ioctl(gl_sfd, SPI_IOC_MESSAGE(1), &tr);
}

int gl_setup(uint8_t contrust){
  int i, j;

  cape_add(CAPE_DTBO);
  gl_sfd = open(SPI_CH, O_RDWR);
  if(gl_sfd < 0){
    perror("SPI open");
    return -1;
  }

  gpio_export(GPIO_RS);
  gl_pfd = gpio_open(GPIO_RS, "direction", O_WRONLY);
  if(gl_pfd < 0){
    perror("GPIO open");
    return -1;
  }
  write(gl_pfd, "out", 3);
  close(gl_pfd);
  gl_pfd = gpio_open(GPIO_RS, "value", O_WRONLY);

  COMMAND();
  uint8_t init[] = {
    0xae, 0xa0, 0xc8, 0xa3, 0x2c,
    0x2e,
    0x2f, 0x23, 0x81, 0x1c, 0xa4, 0x40, 0xa6, 0xaf
  };
  init[9] = contrust;
  gl_xfers(&init[0], 5); usleep(2000);
  gl_xfers(&init[5], 1); usleep(2000);
  gl_xfers(&init[6], 8);

  for(i = 0; i <= 5; i++){
    COMMAND();
    gl_xfer(0xb0 | i);//page
    gl_xfer(0x10);//column 0
    gl_xfer(0x00);

    DRAWING();
    for(j = 0; j < 128; j++)
      gl_xfer(0);
  }
}

void gl_locate(uint8_t p, uint8_t c){
  COMMAND();
  gl_xfer(0xb0 | p);
  gl_xfer(0x10 | (c >> 4));
  gl_xfer(c & 0x0f);
}

void gl_printf(const char *format, ...){
#include "font.h"
  char *s;
  char buf[80];

  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, 80, format, ap);
  va_end(ap);

  DRAWING();
  s = buf;
  while(*s){
    gl_xfers(&font[*s * 6], 6);
    s++;
  }
}

char bigcode(char c){
  if(c >= '0' && c <= '9')
    return c - '0';
  if(c == '-')
    return 10;
  return 11;
}

void bigdraw(char *s, int line){
#include "bigfont.h"
  uint8_t dot[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1c, 0x1c, 0x1c, 0x00
  };

  DRAWING();
  while(*s){
    if(*s == '.'){
      gl_xfers(&dot[line * 5], 5);
    }
    else {
      gl_xfers(&bigfont[bigcode(*s) * 20 + line * 10], 10);
    }
    s++;
  }
}

void gl_bigprintf(uint8_t p, uint8_t c, const char *format, ...){
  char *s;
  char buf[80];

  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, 80, format, ap);
  va_end(ap);

  gl_locate(p, c);
  bigdraw(buf, 0);

  gl_locate(p + 1, c);
  bigdraw(buf, 1);
}

void plot(int now, int old, uint8_t *buf){
  uint8_t ptn[] = {
    0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f
  };
  int p;
  int y1, y2;
  int d1, d2;
  int s1, s2;

  if(now > 47)
    now = 47;
  else if(now < 0)
    now = 0;

  y1 = 47 - old;
  y2 = 47 - now;
  if(y1 < y2){
    char tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
  }
  y1++;

  d1 = y1 / 8;
  s1 = y1 % 8;
  d2 = y2 / 8;
  s2 = y2 % 8;
  for(p = 0; p < d1; p++)
    buf[p] = 0xff;
  buf[p++] = ptn[s1];
  for(; p < 6; p++)
    buf[p] = 0;
  for(p = 0; p < d2; p++)
    buf[p] = 0;
  buf[p] &= ~ptn[s2];
}

void gl_set(int data){
  int p;
  int c;
  uint8_t buf[6];
  static char prev;

  plot(data, prev, buf);
  prev = data;

  for(p = 0; p < 6; p++){
    for(c = 0; c < GL_WIDTH - 1; c++)
      bitmap[p][c] = bitmap[p][c + 1];
    bitmap[p][GL_WIDTH - 1] = buf[p];
  }
}

void gl_refresh(){
  int p;

  for(p = 0; p < 6; p++){
    COMMAND();
    gl_xfer(0xb0 | p);
    gl_xfer(0x10);
    gl_xfer(0);
    DRAWING();
    gl_xfers(bitmap[p], GL_WIDTH);
  }
}
