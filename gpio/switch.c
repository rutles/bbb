// switch.c - GPIO input example
// compile: cc switch.c -o switch -lbbb
// execute: ./switch

#include <poll.h>
#include <bbb.h>

#define PIN 60

int main(){
  char c;
  int fd;
  int ret;
  struct pollfd pfd;

  gpio_export(PIN);

  fd = gpio_open(PIN, "direction", O_WRONLY);
  write(fd, "in", 2);
  close(fd);

  fd = gpio_open(PIN, "edge", O_WRONLY);
  write(fd, "both", 4);
  close(fd);

  fd = gpio_open(PIN, "value", O_RDONLY);
  pfd.fd = fd;
  pfd.events = POLLPRI;
  while(1){
    lseek(fd, 0, SEEK_SET);
    ret = poll(&pfd, 1, 10000);
    if(ret == 0)
      break;

    read(fd, &c, 1);
    printf(c == '0' ? "Pushed\n" : "Released\n");
  }

  printf("\nTimeout\n");
  close(fd);
  gpio_unexport(PIN);
}
