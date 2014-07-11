// led.c - GPIO output example
// compile: cc led.c -o led -lbbb
// execute: ./led

#include <bbb.h>

#define PIN 48

main(){
  int i;
  int fd;

  gpio_export(PIN);
  fd = gpio_open(PIN, "direction", O_WRONLY);
  write(fd, "out", 3);
  close(fd);

  fd = gpio_open(PIN, "value", O_WRONLY);
  for(i = 0; i < 5; i++){
    write(fd, "0", 1);
    sleep(1);
    write(fd, "1", 1);
    sleep(1);
  }

  close(fd);
  gpio_unexport(PIN);
}
