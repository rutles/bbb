// uvmeterd.c UV meter daemon
// compile: cc uvmeterd.c -o uvmeterd -lbbb -li2clcd
// execute: ./uvmeterd

#include <bbb.h>
#include <i2clcd.h>

#define CH_UV 1

main(){
  int v;
  float value;
  int index;

  adc_setup();
  lcd_setup(32);

  daemon(0, 0);

  while(1){
    v = adc_volt(CH_UV);
    v = adc_volt(CH_UV);

    value = (float)v / 120;
    index = v * 125 / 10000;

    lcd_setCursor(0, 0);
    lcd_printf("UV=%5.2f", value);

    lcd_setCursor(0, 1);
    if(index < 3)
      lcd_printf("Low     ");
    else if(index < 6)
      lcd_printf("Moderate");
    else if(index < 8)
      lcd_printf("High    ");
    else if(index < 11)
      lcd_printf("Veryhigh");
    else
      lcd_printf("Extreme ");

    sleep(1);
  }
}
