// aqm0802a.c - AQM0802A handling example
// compile: cc aqm0802a.c -o aqm0802a -li2clcd
// execute: ./aqm0802a

#include <i2clcd.h>

main(){
  int fd;
  int i;

  uint8_t squar[] = {
    0, 0x04, 0x0a, 0x02, 0x04, 0x0e, 0x00, 0x00, 0x00
  };

  uint8_t heart[] = {
    1, 0x00, 0x0a, 0x1f, 0x1f, 0x1f, 0x0e, 0x04, 0x00
  };

  lcd_setup(32);
  lcd_createChar(squar);
  lcd_createChar(heart);

  lcd_command(1);

  lcd_setCursor(0, 0);
  lcd_putch('I');
  lcd_putch(0);
  lcd_printf("C LCD");
  lcd_putch(1);

  lcd_setCursor(0, 1);
  lcd_printf("AQM0802A");
}
