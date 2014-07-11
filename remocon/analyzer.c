// analyzer.c - Ir-Remocon signal analyzer
// compile: cc analyzer.c -o analyzer -lrt -lremocon
// execute: ./analyzer [-r<numx>] [-t<pin>] [-d<dir>]

#include <termios.h>
#include <remocon.h>

int getch(){
  struct termios b;
  struct termios a;
  int c;

  tcgetattr(STDIN_FILENO, &b);
  a = b;
  a.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &a);
  c = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &b);

  return c;
}

int main(int argc, char *argv[]){
  int serial = 0;
  int fd;
  int ret;
  irform_t ir;
  option_t op;

  int son[RC_SIGLEN], soff[RC_SIGLEN];
  uint8_t code1[RC_SIGCNT], code2[RC_SIGCNT];

  ir.son = son;
  ir.soff = soff;
  ir.code1 = code1;
  ir.code2 = code2;

  rc_getopt(argc, argv, &op);
  rc_rassign(op.r);
  rc_tassign(op.t);

  while(1){
    fd = rc_ropen(op.r);
    printf("Ready.Push button.\n");
    ret = rc_analyze(fd, &ir);
    close(fd);

    if(ret < 0){
      perror("rc_analyze");
      return -1;
    }

    rc_statist(&ir);
    printf("Analyze done.\n\n");
    rc_report(&ir);

    while(1){
      printf("[T]est [M]emo [N]ext [Q]uit");
      do{
        ret = toupper(getch());
      } while(strchr("TMNQ", ret) == NULL);
      printf(" %c\n\n", ret);

      if(ret == 'T'){
        fd = rc_topen(op.t);
        rc_transmit(fd, &ir);
        close(fd);
        printf("IR transmit via %s.\n", op.t);
        continue;
      }

      if(ret == 'M'){
        rc_memo(serial++, &ir);
        printf("memo done.\n\n");
        continue;
      }

      if(ret == 'N')
        break;

      if(ret == 'Q'){
        if(serial == 0){
          rc_rfree(op.r);
          return 0;
        }

        ret = rc_save(serial, op.d, &ir);
        if(ret < 0)
          fprintf(stderr, "fail to save.\n");
        else
          printf("Create %s/analyzer.conf.\n", op.d);
        rc_rfree(op.r);
        return ret;
      }
    }
  }
}
