#include <i2clcd.h>

void lcd_setCursor(uint8_t x, uint8_t y){
  lcd_command(0x80 | (x + (y > 0 ? 0x40 : 0)));
}
