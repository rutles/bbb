#include <i2clcd.h>

void lcd_printf(const char *format, ...){
  int i;
  int fd;
  char buf[9];
  static uint8_t c = 0x40;

  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, 9, format, ap);
  va_end(ap);

  for(i = 0; i < strlen(buf); i++)
    lcd_putch(buf[i]);
}

