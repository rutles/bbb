// gpio.c - GPIO handler

#include <bbb.h>

void gpio_export(int n){
  int fd;
  char buf[40];

  fd = open("/sys/class/gpio/export", O_WRONLY);
  sprintf(buf, "%d", n);
  write(fd, buf, strlen(buf));
  close(fd);
}

void gpio_unexport(int n){
  int fd;
  char buf[40];

  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  sprintf(buf, "%d", n);
  write(fd, buf, strlen(buf));
  close(fd);
}

int gpio_open(int n, char *file, int flag){
  int fd;
  char buf[40];

  sprintf(buf, "/sys/class/gpio/gpio%d/%s", n, file);
  fd = open(buf, flag);
  return fd;
}
