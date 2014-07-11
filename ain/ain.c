// ain.c A/D converter handling example
// compile: cc ain.c -o ain -lbbb
// execute: ./ain

#include <bbb.h>

main(){
  int i;
  int v;
  int r;

  adc_setup();
  for(i = 0; i < 8; i++){
    v = adc_volt(i);
    v = adc_volt(i);
    r = adc_raw(i);
    r = adc_raw(i);
    printf("AIN%d: %4d, %4d\n", i, v, r);
  }
}
