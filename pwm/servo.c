// servo.c - Servo motor handling example
// compile: cc servo.c -o servo -lbbb
// execute: ./servo

#include <bbb.h>

#define PIN "P9_16"

int main(int argc, char *argv[]){
  int ret;
  int fd;
  int duty;
  char buf[16];

  pwm_setup(PIN, 20000000, 1000000);

  fd = pwm_open(PIN, "run");
  if(fd < 0){
    perror("pwm_open:run");
    return -1;
  }
  write(fd, "1", 1);
  close(fd);
  sleep(3);

  fd = pwm_open(PIN, "duty");

  for(duty = 1000000; duty <= 2000000; duty += 50000){
    sprintf(buf, "%d", duty);
    write(fd, buf, strlen(buf));
    sleep(1);
  }

  close(fd);

  fd = pwm_open(PIN, "run");
  write(fd, "0", 1);
  close(fd);

  return 0;
}
