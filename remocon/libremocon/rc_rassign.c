// rc_rassign, rc_ropen, rc_rfree
// libremocon member function

#include <remocon.h>

int rc_rassign(int gpio){
  int ret;
  int fd;
  char buf[40];

  //export gpio
  fd = open("/sys/class/gpio/export", O_WRONLY);
  sprintf(buf, "%d", gpio);
  ret = write(fd, buf, strlen(buf));
  close(fd);
  if(ret <= 0)
    return -1;

  //set gpio as input
  sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
  fd = open(buf, O_WRONLY);
  write(fd, "in", 2);
  close(fd);

  //set value change (edge) detect
  sprintf(buf, "/sys/class/gpio/gpio%d/edge", gpio);
  fd = open(buf, O_WRONLY);
  write(fd, "both", 4);
  close(fd);

  return 0;
}

int rc_ropen(int gpio){
  int ret;
  int fd;
  char buf[40];

  //open gpio for read
  sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
  fd = open(buf, O_RDONLY);

  return fd;
}

void rc_rfree(int gpio){
  int fd;
  char buf[40];

  //unexport gpio
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  sprintf(buf, "%d", gpio);
  write(fd, buf, strlen(buf));
  close(fd);
}
