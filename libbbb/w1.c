// w1.c - 1-Wire handler

#include <bbb.h>

char *w1_setup(const char* dto){
  int ret;
  FILE *fp;
  char buf[80];

  //enable w1-gpio
  ret = cape_add(dto);
  if(ret < 0)
    return NULL;

  //get ID
  fp = fopen("/sys/devices/w1_bus_master1/w1_master_slaves", "r");
  if(fp == NULL)
    return NULL;
  fgets(buf, 80, fp);
  fclose(fp);
  return strdup(strtok(buf, "\n"));
}

int w1_read(const char *id){
  char *s;
  FILE *fp;
  char buf[80];

  sprintf(buf, "/sys/devices/w1_bus_master1/%s/w1_slave", id);
  fp = fopen(buf, "r");
  if(fp == NULL)
    return 2147483647;
  fgets(buf, 80, fp);
  fgets(buf, 80, fp);
  fclose(fp);
  s = strrchr(buf, '=');
  if(s == NULL)
    return 2147483647;

  s++;
  return atoi(s);
}
