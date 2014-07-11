// rc_tassign, rc_topen
// libremocon member function

#include <remocon.h>

char *rc_path(const char *dir, const char *s){
  struct dirent **list;
  int n;
  char buf[80];
  char *ret = NULL;

  n = scandir(dir, &list, NULL, NULL);
  if(n <= 0)
    return NULL;
  while(n--){
    if(strncmp(list[n]->d_name, s, strlen(s)) == 0){
      sprintf(buf, "%s/%s", dir, list[n]->d_name);
      ret = strdup(buf);
      break;
    }
  }
  free(list);
  return ret;
}

int rc_slot_number(const char *dto){
  char *path;
  FILE *fp;
  char buf[80];
  int ret = -1;

  path = rc_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);
  fp = fopen(buf, "r");
  while(fgets(buf, 80, fp) != NULL){
    if(strstr(buf, dto) != NULL){
      ret = atoi(strtok(buf, ":"));
      break;
    }
  }
  fclose(fp);
  return ret;
}

int rc_slot_add(const char *dto){
  char *path;
  int ret;
  int fd;
  char buf[80];

  ret = rc_slot_number(dto);
  if(ret >= 0)
    return -1;

  path = rc_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);

  fd = open(buf, O_WRONLY);
  ret = write(fd, dto, strlen(dto));
  close(fd);
  usleep(500000);
  return ret;
}

int rc_tassign(char* pname){
  char *ocp;
  char *path;
  int ret;
  int fd;
  char buf[80];

  //write "am33xx_pwm" to cape manager
  rc_slot_add("am33xx_pwm");

  //write pin name to cape manager
  sprintf(buf, "bone_pwm_%s", pname);
  rc_slot_add(buf);

  //get path of pwm_test_Px_x
  ocp = rc_path("/sys/devices", "ocp.");
  sprintf(buf, "pwm_test_%s.", pname);
  path = rc_path(ocp, buf);
  free(ocp);

  //PWM stop
  sprintf(buf, "%s/run", path);
  fd = open(buf, O_WRONLY);
  ret = write(fd, "0", 1);
  close(fd);
  if(ret <= 0)
    return -1;

  //set period 26316nS (38kHz)
  sprintf(buf, "%s/period", path);
  fd = open(buf, O_WRONLY);
  write(fd, "26316", 5);
  close(fd);

  //set duty 10658nS (50%)
  sprintf(buf, "%s/duty", path);
  fd = open(buf, O_WRONLY);
  write(fd, "10658", 5);
  close(fd);

  //set polarity
  sprintf(buf, "%s/polarity", path);
  fd = open(buf, O_WRONLY);
  write(fd, "0", 1);
  close(fd);

  free(path);
  return 0;
}

int rc_topen(char* pname){
  char *ocp;
  char *path;
  int ret;
  int fd;
  char buf[80];

  //open run
  ocp = rc_path("/sys/devices", "ocp.");
  sprintf(buf, "pwm_test_%s.", pname);
  path = rc_path(ocp, buf);
  free(ocp);

  sprintf(buf, "%s/run", path);
  free(path);
  fd = open(buf, O_WRONLY);
  return fd;
}
