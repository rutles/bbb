// buzzer.c - GPIO output example
// compile: cc buzzer.c -o buzzer -lbbb
// execute: ./buzzer

#include <bbb.h>

#define PIN 49

main(){
  int i;
  int fd;

  gpio_export(PIN);
  fd = gpio_open(PIN, "direction", O_WRONLY);
  write(fd, "out", 3);
  close(fd);

  fd = gpio_open(PIN, "value", O_WRONLY);
  for(i = 0; i < 5; i++){
    write(fd, "1", 1);
    usleep(50000);
    write(fd, "0", 1);
    usleep(50000);
  }

  close(fd);
  gpio_unexport(PIN);
}
