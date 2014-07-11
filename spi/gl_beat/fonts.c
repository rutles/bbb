// fonts - glcd font viewer
// compile: cc fonts.c glcd.c -o fonts -lbbb
// execute: ./fonts

#include <stdint.h>
#include "glcd.h"

main(){
  int c;
  int p;

  gl_setup(CONTRUST);

  for(p = 0; p < 4; p++){
    gl_locate(p, 0);
    for(c = 0; c < 21; c++)
      gl_printf("%c", p * 21 + c);
  }

  gl_bigprintf(4, 0, "%13.9f", -0.123456789);
}
