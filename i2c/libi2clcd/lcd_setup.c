#include <i2clcd.h>

int lcd_setup(uint8_t c){
  int fd;
  static uint8_t init[] = {
    0x38, 0x39, 0x14, 0x70, 0x54, 0x6c,
    0x38, 0x0d, 0x01
  };

  init[3] = 0x70 | (c & 0x0f);
  init[4] = 0x54 | (c >> 4);

  fd = lcd_open();
  if(fd < 0)
    return -1;

  write(fd, &init[0], 6);
  usleep(200000);

  write(fd, &init[6], 3);
  usleep(1080);

  close(fd);
  return 0;
}
