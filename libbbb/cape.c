// cape.c - cape manager handler

#include <bbb.h>

char *cape_path(const char *dir, const char *s){
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

int cape_number(const char *dtbo){
  char *path;
  FILE *fp;
  char buf[80];
  int ret = -1;

  path = cape_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);
  fp = fopen(buf, "r");
  while(fgets(buf, 80, fp) != NULL){
    if(strstr(buf, dtbo) != NULL){
      ret = atoi(strtok(buf, ":"));
      break;
    }
  }
  fclose(fp);
  return ret;
}

int cape_add(const char *dtbo){
  char *path;
  int fd;
  char buf[80];
  int ret;

  ret = cape_number(dtbo);
  if(ret >= 0)
    return 0;

  path = cape_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);

  fd = open(buf, O_WRONLY);
  ret = write(fd, dtbo, strlen(dtbo));
  close(fd);
  usleep(500000);
  return ret;
}

int cape_remove(const char *dtbo){
  char *path;
  int fd;
  int n;
  char buf[80];

  n = cape_number(dtbo);
  if(n < 0)
    return 0;

  path = cape_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);

  fd = open(buf, O_WRONLY);
  sprintf(buf, "-%d", n);
  write(fd, buf, strlen(buf));
  close(fd);
  return 0;
}
