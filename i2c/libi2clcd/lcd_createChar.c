#include <i2clcd.h>

void lcd_createChar(uint8_t *ptn){
  int i;
  int fd;

  lcd_command(0x38);
  lcd_command(0x40 | ptn[0] << 3);
  for(i = 1; i < 9; i++)
    lcd_putch(ptn[i]);
}
