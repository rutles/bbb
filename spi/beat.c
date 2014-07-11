// beat - Heartbeat counter
// compile: cc beat.c -o beat -lrt -lbbb
// execute: ./beat

#include <bbb.h>
#include <time.h>

#define AIN_CH 5
#define THRESHOLD 2560
#define INTERVAL 25 //msec

int main(){
  char path[80];
  FILE *fp;
  struct timespec old_time;
  struct timespec now_time;
  int old_v;
  int now_v;
  int old_d;
  int now_d;
  int beat;

  adc_setup();
  //get path /sys/bus/iio/devices/iio:device*/in_voltage*_raw
  char *device = cape_path("/sys/bus/iio/devices", "iio:device");
  sprintf(path, "%s/in_voltage%d_raw", device, AIN_CH);
  free(device);

  while(1){
    clock_gettime(CLOCK_MONOTONIC_RAW, &now_time);
    fp = fopen(path, "r");
    fscanf(fp, "%d", &now_v);
    fclose(fp);

    now_d = old_v <= now_v;
    if(now_v >= THRESHOLD && old_d && !now_d){
      int period =
      (now_time.tv_sec - old_time.tv_sec) * 1000 +
      (now_time.tv_nsec - old_time.tv_nsec) / 1000000;
      beat = 60000 / period;

      old_time.tv_sec = now_time.tv_sec;
      old_time.tv_nsec = now_time.tv_nsec;
      printf("%3d\n", beat);
    }
    old_d = now_d;
    old_v = now_v;

    now_time.tv_nsec += (INTERVAL * 1000000);
    if(now_time.tv_nsec >= 1000000000){
      now_time.tv_nsec -= 1000000000;
      now_time.tv_sec += 1;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &now_time, NULL);
  }
}
