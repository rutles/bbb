// adc.c - A/D converter handler

#include <bbb.h>

int adc_setup(void){
  return cape_add("cape-bone-iio");
}

int adc_volt(int ch){
  char *ocp;
  char *helper;
  char buf[80];
  FILE *fp;
  int v;

  //get path /sys/devices/ocp.*/helper*
  ocp = cape_path("/sys/devices", "ocp.");
  helper = cape_path(ocp, "helper.");
  free(ocp);
  sprintf(buf, "%s/AIN%d", helper, ch);
  free(helper);

  fp = fopen(buf, "r");
  if(fp == NULL)
    return -1;
  fscanf(fp, "%d", &v);
  fclose(fp);
  return v;
}

int adc_raw(int ch){
  char *device;
  char buf[80];
  FILE *fp;
  int r;

  //get path /sys/bus/iio/devices/iio:device*/in_voltage*_raw
  device = cape_path("/sys/bus/iio/devices", "iio:device");
  sprintf(buf, "%s/in_voltage%d_raw", device, ch);
  free(device);

  fp = fopen(buf, "r");
  if(fp == NULL)
    return -1;
  fscanf(fp, "%d", &r);
  fclose(fp);
  return r;
}
