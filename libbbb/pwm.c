// pwm.c - pwm handler

#include <bbb.h>

int pwm_setup(const char* pname, int period, int duty){
  int ret;
  int fd;
  char buf[80];

  //write "am33xx_pwm" to cape manager
  cape_add("am33xx_pwm");

  //write pin name to cape manager
  sprintf(buf, "bone_pwm_%s", pname);
  cape_add(buf);

  //PWM stop to make sure
  fd = pwm_open(pname, "run");
  ret = write(fd, "0", 1);
  close(fd);
  if(ret <= 0)
    return -1;

  //set period
  fd = pwm_open(pname, "period");
  sprintf(buf, "%d", period);
  write(fd, buf, strlen(buf));
  close(fd);

  //set duty
  fd = pwm_open(pname, "duty");
  sprintf(buf, "%d", duty);
  write(fd, buf, strlen(buf));
  close(fd);

  return 0;
}

int pwm_open(const char* pname, const char *fname){
  char *ocp;
  char *path;
  int fd;
  char buf[80];

  //get path
  ocp = cape_path("/sys/devices", "ocp.");
  sprintf(buf, "pwm_test_%s.", pname);
  path = cape_path(ocp, buf);
  free(ocp);

  //get descriptor
  sprintf(buf, "%s/%s", path, fname);
  free(path);
  fd = open(buf, O_WRONLY);
  return fd;
}
