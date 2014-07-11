// sin - sin wave display demo
// compile: cc sin.c glcd.c -o sin -lm -lbbb
// execute: ./sin

#include <stdint.h>
#include <math.h>
#include "glcd.h"

main(){
  int value;
  int d;
  int refresh;

  gl_setup(CONTRUST);

  while(1){
    value = sin((double)(d++ & 63) / 32 * 3.14) * 24 + 23;
    gl_set(value);
    if((refresh++ & 3) == 0)
      gl_refresh();

    usleep(25000);
  }
}
