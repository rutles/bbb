// rc_transmit.c
// libremocon member function

#include <remocon.h>

void rc_transmit(int fd, irform_t *ir){
  int i, j;// loop counter
  uint8_t code;// a byte of ir code
  struct timespec org;

  clock_gettime(CLOCK_MONOTONIC, &org);

  write(fd, "1", 1);// leader on
  RC_NSLEEP(org, ir->lon);
  write(fd, "0", 1);// leader off
  RC_NSLEEP(org, ir->loff);

  for(i = 0; i < ir->count1; i++){
    code = ir->code1[i];
    for(j = 0; j < 8; j++){
      write(fd, "1", 1);// signal on
      RC_NSLEEP(org, ir->t);
      write(fd, "0", 1);// signal off
      RC_NSLEEP(org, (code & 0x80 ? ir->h : ir->l));
      code <<= 1;
    }
  }
  write(fd, "1", 1);// signal on
  RC_NSLEEP(org, ir->t);
  write(fd, "0", 1);// signal off

  if(!ir->multi)
    return;

  RC_NSLEEP(org, ir->interval);// interval

  write(fd, "1", 1);// leader on
  RC_NSLEEP(org, ir->lon);
  write(fd, "0", 1);// leader off
  RC_NSLEEP(org, ir->loff);

  for(i = 0; i < ir->count2; i++){
    code = ir->code2[i];
    for(j = 0; j < 8; j++){
      write(fd, "1", 1);// signal on
      RC_NSLEEP(org, ir->t);
      write(fd, "0", 1);// signal off
      RC_NSLEEP(org, (code & 0x80 ? ir->h : ir->l));
      code <<= 1;
    }
  }
  write(fd, "1", 1);// signal on
  RC_NSLEEP(org, ir->t);
  write(fd, "0", 1);// signal off
}
