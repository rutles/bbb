// gl_beat - Heartbeat counter
// compile: cc gl_beat.c glcd.c -o gl_beat -lrt -lbbb
// execute: ./gl_beat

#include <time.h>
#include <bbb.h>
#include "glcd.h"

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
  float beat;
  int refresh;

  gl_setup(CONTRUST);
  adc_setup();
  //get path /sys/bus/iio/devices/iio:device*/in_voltage*_raw
  char *device = cape_path("/sys/bus/iio/devices", "iio:device");
  sprintf(path, "%s/in_voltage%d_raw", device, AIN_CH);
  free(device);

  gl_locate(0, GL_WIDTH + 10);
  gl_printf("Fingertop");
  gl_locate(1, GL_WIDTH + 10);
  gl_printf("Health");
  gl_locate(2, GL_WIDTH + 10);
  gl_printf("Meter");
  while(1){
    clock_gettime(CLOCK_MONOTONIC_RAW, &now_time);
    fp = fopen(path, "r");
    fscanf(fp, "%d", &now_v);
    fclose(fp);

    gl_set(now_v * 48 / 4096);
    if((refresh++ & 3) == 0)
      gl_refresh();

    now_d = old_v <= now_v;
    if(now_v >= THRESHOLD && old_d && !now_d){
      int period =
      (now_time.tv_sec - old_time.tv_sec) * 1000 +
      (now_time.tv_nsec - old_time.tv_nsec) / 1000000;
      beat = 60000 / period;

      old_time.tv_sec = now_time.tv_sec;
      old_time.tv_nsec = now_time.tv_nsec;
      if(beat < 999)
        gl_bigprintf(4, GL_WIDTH + 10, "%5.1f", beat);
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
