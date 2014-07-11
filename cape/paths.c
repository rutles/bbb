// paths.c - Display uncertain paths
// compile: cc paths.c -o paths -lbbb
// execute: ./paths

#include <bbb.h>

#define PIN_PWM "P9_16"

int main(int argc, char *argv[]){
  char *ocp;
  char *path;
  char buf[80];

  //get path to cape manager
  path = cape_path("/sys/devices", "bone_capemgr.");
  printf("Cape manager: %s/slots\n", path);
  free(path);

  //PWM setup
  cape_add("am33xx_pwm");
  sprintf(buf, "bone_pwm_%s", PIN_PWM);
  cape_add(buf);

  //get PWM paths
  ocp = cape_path("/sys/devices", "ocp.");
  sprintf(buf, "pwm_test_%s.", PIN_PWM);
  path = cape_path(ocp, buf);
  free(ocp);
  printf("PWM pin%s: %s\n", PIN_PWM, path);
  free(path);

  //ADC setup
  cape_add("cape-bone-iio");

  //get path to ADC
  path = cape_path("/sys/bus/iio/devices", "iio:device");
  printf("ADC channels: %s\n", path);
  free(path);

  return 0;
}
