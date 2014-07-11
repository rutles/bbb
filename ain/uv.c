// uv.c UV sensor cape test program
// compile: cc uv.c -o uv -lbbb
// execute: ./uv

#include <bbb.h>

#define CH_UV 1

main(){
  int v;
  float value;
  int index;

  adc_setup();

  while(1){
    v = adc_volt(CH_UV);
    v = adc_volt(CH_UV);

    value = (float)v / 120;
    index = v * 125 / 10000;

    printf("%4dmV, %4.1fmW/cm2, Index:%2d\n", v, value, index);
    sleep(1);
  }
}
