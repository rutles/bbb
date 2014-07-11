// pird.c - pyro alarm daemon
// compile: cc pird.c -o pird -lbbb -lrt
// execute: ./pird

#include <poll.h>
#include <time.h>
#include <bbb.h>

#define PIR 61
#define BUZ 49

main(){
  char c;
  int i;
  int pir;
  int buz;
  struct timespec b;
  struct timespec a;
  struct pollfd pfd;

  gpio_export(PIR);
  pir = gpio_open(PIR, "direction", O_WRONLY);
  write(pir, "in", 2);
  close(pir);
  pir = gpio_open(PIR, "edge", O_WRONLY);
  write(pir, "rising", 6);
  close(pir);
  pir = gpio_open(PIR, "value", O_RDONLY);
  pfd.fd = pir;
  pfd.events = POLLPRI;

  gpio_export(BUZ);
  buz = gpio_open(BUZ, "direction", O_WRONLY);
  write(buz, "out", 3);
  close(buz);
  buz = gpio_open(BUZ, "value", O_WRONLY);

  daemon(0, 0);

  while(1){
    lseek(pir, 0, SEEK_SET);
    poll(&pfd, 1, 1000);
    read(pir, &c, 1);
    if(c == '1'){
      clock_gettime(CLOCK_MONOTONIC_RAW, &b);
      continue;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &a);
    if(
    ((a.tv_sec - b.tv_sec) * 1000000 +
    (a.tv_nsec - b.tv_nsec) / 1000)
     < 10000000)
       continue;

    for(i = 0; i < 5; i++){
      write(buz, "1", 1);
      usleep(50000);
      write(buz, "0", 1);
      usleep(50000);
    }
  }
}
