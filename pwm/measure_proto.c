// measure_proto - ultrasonic measure prototype
// compile: cc measure_proto.c -o measure_proto -lbbb
// execute: ./measure_proto

#include <bbb.h>

#define PIN_T "P8_19"
#define T_BURST 350

int main(int argc, char *argv[]){
  int t;

  pwm_setup(PIN_T, 25000, 8000);
  t = pwm_open(PIN_T, "polarity");
  write(t, "0", 1);
  close(t);
  t = pwm_open(PIN_T, "run");

  while(1){
    write(t, "1", 1);
    usleep(T_BURST);
    write(t, "0", 1);

    usleep(1000000);
  }
}
