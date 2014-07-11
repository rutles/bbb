// rc_analyze.c
// libremocon member function

#include <remocon.h>

int rc_analyze(int fd, irform_t *ir){
  int i, j; // loop counter
  int record;
  char c; // for read PL-IRM2161-XD1 signal pin
  int ret;// poll return value. for detect timeout or error
  int son, soff;
  int temp;// lap time
  int last;// last lap time
  struct timespec origin;// signal start time
  struct timespec now;// signal change time
  struct pollfd pfd;// for poll

  record = (ir->son == NULL || ir->soff == NULL) ? 0 : -1;
  pfd.fd = fd; // poll fd
  pfd.events = POLLPRI; // detect value change
  ir->multi = 0;

  //detect reader of first signal
  do{
    //detect leader off->on
    do{
      lseek(fd, 0, SEEK_SET);// prepare read
      poll(&pfd, 1, -1);// wait for value change forever
      clock_gettime(CLOCK_MONOTONIC_RAW, &origin);// record start time
      read(fd, &c, 1);// read
    } while (c == '1');// if on->off, try again

    //count leader on length
    lseek(fd, 0, SEEK_SET);// prepare read
    poll(&pfd, 1, -1);// wait for on->off forever
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    read(fd, &c, 1);// dummy read
    ir->lon = RC_LAP(now, origin);// record reader on length
  } while(ir->lon < RC_DETECT);// if noise, try again

  //Count leader off length
  lseek(fd, 0, SEEK_SET);// prepare read
  ret = poll(&pfd, 1, RC_TIMEOUT);// wait for signal until timeout
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  read(fd, &c, 1);// dummy read
  last = RC_LAP(now, origin);// record lap time
  ir->loff = last - ir->lon;// record leader off length
  if(ret <= 0)
    return -1;

  // signal analyze
  for(i = 0; i < RC_SIGLEN; i++){
    lseek(fd, 0, SEEK_SET);// prepare read
    poll(&pfd, 1, -1);// wait for on->off forever
    clock_gettime(CLOCK_MONOTONIC_RAW, &now); //record lap time
    read(fd, &c, 1);// dummy read
    temp = RC_LAP(now, origin);
    son = temp - last;
    last = temp;

    lseek(fd, 0, SEEK_SET);// prepare read
    ret = poll(&pfd, 1, RC_TIMEOUT);//off->on
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    read(fd, &c, 1);// dummy read
    temp = RC_LAP(now, origin);
    soff = temp - last;
    last = temp;
    j = i / 8;
    ir->code1[j] <<= 1;
    ir->code1[j] |= (son * 2 < soff);
    if(record){
      ir->son[i] = son;
      ir->soff[i] = soff;
    }
    if(ret <= 0)
      break;
  }
  ir->length = i;
  ir->count1 = j;

  //detect leader off->on
  lseek(fd, 0, SEEK_SET);// prepare read
  ret = poll(&pfd, 1, RC_CONTINUE);// wait for value change until timeout
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  read(fd, &c, 1);// dummy read
  if(ret <= 0)// not detect next signal
    return ret;

  ir->gap = RC_LAP(now, origin);// from top of previous signal
  ir->interval = ir->gap - last;// from tail of previous signal

  //pass reader on
  lseek(fd, 0, SEEK_SET);// prepare read
  ret = poll(&pfd, 1, RC_DETECT);// on->off
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  last = RC_LAP(now, origin);
  read(fd, &c, 1);// dummy read
  if(ret <= 0)
    return ret;

  //pass leader off
  lseek(fd, 0, SEEK_SET);// prepare read
  ret = poll(&pfd, 1, RC_TIMEOUT);
  clock_gettime(CLOCK_MONOTONIC_RAW, &now);
  read(fd, &c, 1);// dummy read
  temp = RC_LAP(now, origin);
  ir->repeat = temp - last;
  last = temp;
  if(ret <= 0){
    return -1;
  } else
  if((ir->repeat < ir->loff * 8 / 10) ||
    (ir->repeat > ir->loff * 12 / 10))
    return 0;

  ir->multi = 1;

  //Signal decode
  for(i = 0; i < RC_SIGLEN; i++){
    lseek(fd, 0, SEEK_SET);// prepare read
    poll(&pfd, 1, -1);// on->off
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    read(fd, &c, 1);// dummy read
    temp = RC_LAP(now, origin);
    son = temp - last;
    last = temp;

    lseek(fd, 0, SEEK_SET);// prepare read
    ret = poll(&pfd, 1, RC_TIMEOUT);//off->on
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    read(fd, &c, 1);// dummy read
    temp = RC_LAP(now, origin);
    soff = temp - last;
    last = temp;

    j = i / 8;
    ir->code2[j] <<= 1;
    ir->code2[j] |= (son * 2 < soff);
    if(ret <= 0){
      ir->count2 = j;
      return ret;
    }
  }
  return -1;
}
