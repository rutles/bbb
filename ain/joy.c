// joy.c analog joystick reader
// compile: cc joy.c -o joy -lbbb
// execute: ./joy

#include <bbb.h>

#define CH_X 4
#define CH_Y 6

main(){
  int vx, vy;
  int rx, ry;

  adc_setup();
  do{
    vx = adc_volt(CH_X);
    vx = adc_volt(CH_X);
    vy = adc_volt(CH_Y);
    vy = adc_volt(CH_Y);
    rx = adc_raw(CH_X);
    rx = adc_raw(CH_X);
    ry = adc_raw(CH_Y);
    ry = adc_raw(CH_Y);

    if(vx < 0 || vy < 0 || rx < 0 || ry < 0)
      printf("error\n");
    else
    printf("X:%4dmV(%4d), Y:%4dmV(%4d)\n", vx, rx, vy, ry);
    sleep(1);
  } while (rx > 512 && ry > 512);
}
