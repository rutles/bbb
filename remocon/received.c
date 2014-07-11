// received.c - Ir-Remocon signal receive daemon
// compile: cc received.c -o received -lrt -lremocon
// execute: ./received [-r<xx>] [-d<dir>]

#include <remocon.h>

int main(int argc, char **argv){
  int i;
  int fd;
  int ret;
  char buf[80];
  FILE *file1;
  FILE *file2;
  int count;
  char **button;
  char *command;
  option_t op;
  irform_t ir1;
  irform_t ir2;
  uint8_t code1[RC_SIGCNT];
  uint8_t code2[RC_SIGCNT];

  ir2.son = NULL;
  ir2.soff = NULL;
  ir2.code1 = code1;
  ir2.code2 = code2;

  rc_getopt(argc, argv, &op);
  rc_rassign(op.r);
  fd = rc_ropen(op.r);

  sprintf(buf, "%s/analyzer.conf", op.d);
  file1 = fopen(buf, "r");
  if(file1 == NULL){
    perror("fopen:file1");
    return -1;
  }
  sprintf(buf, "%s/received.conf", op.d);
  file2 = fopen(buf, "r");
  if(file2 == NULL){
    perror("fopen:file2");
    return -1;
  }

  count = rc_getbuttons(file1, &button);

  printf("Supported commands\n");
  for(i = 0; i < count; i++)
    printf("button %s: %s\n",
      button[i],
      rc_getcommand(file2, button[i]));
  printf("received started.\n");

  ret = daemon(0, 0);

  while(1){
    ret = rc_analyze(fd, &ir2);//ir catch
    if(ret < 0)// occur some error during ir catch
      goto exit;

    for(i = 0; i < count; i++){
      ret = rc_getdata(file1, button[i], &ir1);
      if(ret < 0)// button not entry in analyzer.conf
        continue;
      if(rc_speccmp(&ir1, &ir2) == 0){
        command = rc_getcommand(file2, button[i]);
        if(strcmp(command, "stop") == 0){
          ret = EXIT_SUCCESS;
          goto exit;
        }
        system(command);//if not entry, no operation
        break;
      }
    }
  }

exit:
  for(i = 0; i < count; i++)
    free(button[i]);
  fclose(file1);
  fclose(file2);
  close(fd);
  rc_rfree(op.r);
  exit(ret);
}
