// measure_lcd - ultrasonic measure daemon
// compile: cc measure_lcdd.c -o measure_lcdd -lrt -lbbb -li2clcd
// execute: ./measure_lcdd

#include <time.h>
#include <poll.h>
#include <bbb.h>
#include <i2clcd.h>

#define PIN_T "P8_19" //Pin for PWM output
#define PIN_R 60 //Pin for GPIO input
#define T_CONST 0 //tolerance
#define T_BURST 350 //transmit burst time in usec
#define T_DELAY 1500 //pass ringing time in usec

int main(int argc, char *argv[]){
  int ret;//poll return value
  int t;//descriptor for transmit
  int r;//descriptor for receive
  int l;//descriptor for LCD

  struct timespec ttime;//transmit time
  struct timespec rtime;//receive time
  struct pollfd pfd;//for poll
  char c;//for read received signal
  int dtime;//round trip time
  int d;//distance

  //transmit setup
  pwm_setup(PIN_T, 25000, 8000);
  t = pwm_open(PIN_T, "polarity");
  write(t, "0", 1);
  close(t);
  t = pwm_open(PIN_T, "run");

  //receive setup
  gpio_export(PIN_R);
  r = gpio_open(PIN_R, "direction", O_WRONLY);
  write(r, "in", 2);
  close(r);
  r = gpio_open(PIN_R, "edge", O_WRONLY);
  write(r, "falling", 7);
  close(r);
  r = gpio_open(PIN_R, "value", O_RDONLY);

  pfd.fd = r;//watch receive
  pfd.events = POLLPRI; //detect value change

  lcd_setup(32);
  lcd_setCursor(0, 0);
  lcd_printf("Distance");

  daemon(0, 0);

  while(1){
    clock_gettime(CLOCK_MONOTONIC_RAW, &ttime);//record transmit time
    write(t, "1", 1);//transmit start
    usleep(T_BURST);
    write(t, "0", 1);//transmit stop
    usleep(T_DELAY);

    do{
      lseek(r, 0, SEEK_SET);
      read(r, &c, 1);
    } while(c != '1');
    ret = poll(&pfd, 1, 10);// wait for receive until 10msec

    clock_gettime(CLOCK_MONOTONIC_RAW, &rtime);
    read(r, &c, 1);//dummy read
    if(ret <= 0){
      lcd_setCursor(0, 1);
      lcd_printf("Error");
      usleep(1000000);
      continue;
    }

    dtime = 
    (rtime.tv_sec - ttime.tv_sec) * 1000000 +
    (rtime.tv_nsec - ttime.tv_nsec) / 1000;

    //calculate distance
    d = (dtime - T_CONST) / 2 * 346 / 1000;

    lcd_setCursor(0, 1);
    lcd_printf("%4dmm", d);

    usleep(1000000);
  }
}
