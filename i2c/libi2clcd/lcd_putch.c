#include <i2clcd.h>

void lcd_putch(uint8_t c){
  int fd;
  uint8_t buf[2];

  buf[0] = 0x40;
  buf[1] = c;
  fd = lcd_open();
  write(fd, buf, 2);
  close(fd);
}
