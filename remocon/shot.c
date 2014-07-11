// shot.c - Ir-Remocon signal transmitter
// compile: cc shot.c -o shot -lrt -lremocon
// execute: ./shot [-t<pin>] [-d<dir>] buton_name

#include <remocon.h>

int main(int argc, char **argv){
  int i;
  int fd;
  int ret;
  char buf[80];
  FILE *file;
  int count;
  char **button;
  char *name;
  option_t op;
  irform_t ir;

  rc_getopt(argc, argv, &op);
  name = argv[argc - 1];

  sprintf(buf, "%s/analyzer.conf", op.d);
  file = fopen(buf, "r");
  if(file == NULL){
    perror("fopen");
    return -1;
  }

  rc_tassign(op.t);
  fd = rc_topen(op.t);

  count = rc_getbuttons(file, &button);
  for(i = 0; i < count; i++){
    ret = strcmp(name, button[i]);
    if(ret == 0){
      rc_getdata(file, name, &ir);
      rc_transmit(fd, &ir);
      free(ir.code1);
      if(ir.multi)
        free(ir.code2);
      break;
    }
  }

  if(ret != 0)
    printf("Button %s is not found.\n", name);

  for(i = 0; i < count; i++)
    free(button[i]);
  fclose(file);
  close(fd);
  return 0;
} 
