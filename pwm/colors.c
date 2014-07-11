// colors.c - Color LED handling example
// compile: cc colors.c -o colors -lm -lbbb
// execute: ./colors

#include <math.h>
#include <bbb.h>

#define LED_R "P9_14"
#define LED_G "P9_42"
#define LED_B "P8_13"

#define PERIOD 10000000
#define SIN(d) sin(2 * M_PI / 360 * (d % 360))
#define DUTY(d) (int)((SIN(d) + 1) / 2 * PERIOD)

main(){
  int i;
  int fd_r;
  int fd_g;
  int fd_b;
  char buf[16];

  pwm_setup(LED_R, PERIOD, DUTY(0));
  pwm_setup(LED_G, PERIOD, DUTY(0));
  pwm_setup(LED_B, PERIOD, DUTY(0));

  fd_r = pwm_open(LED_R, "run");
  write(fd_r, "1", 1);
  close(fd_r);
  fd_g = pwm_open(LED_G, "run");
  write(fd_g, "1", 1);
  close(fd_g);
  fd_b = pwm_open(LED_B, "run");
  write(fd_b, "1", 1);
  close(fd_b);

  fd_r = pwm_open(LED_R, "duty");
  fd_g = pwm_open(LED_G, "duty");
  fd_b = pwm_open(LED_B, "duty");

  for(i = 0; i < 360; i++){
    sprintf(buf, "%d", DUTY(i));
    write(fd_r, buf, strlen(buf));
    sprintf(buf, "%d", DUTY(i + 120));
    write(fd_g, buf, strlen(buf));
    sprintf(buf, "%d", DUTY(i + 240));
    write(fd_b, buf, strlen(buf));
    usleep(50000);
  }

  close(fd_r);
  close(fd_g);
  close(fd_b);

  fd_r = pwm_open(LED_R, "run");
  write(fd_r, "0", 1);
  close(fd_r);
  fd_g = pwm_open(LED_G, "run");
  write(fd_g, "0", 1);
  close(fd_g);
  fd_b = pwm_open(LED_B, "run");
  write(fd_b, "0", 1);
  close(fd_b);
}
