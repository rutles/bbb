// dtbo_handler.c - dtbo file handling example
// compile: cc dtbo_handler.c -o dtbo_handler -lbbb
// execute: ./dtbo_handler [-a] dtbo_discriptor

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <bbb.h>

int main(int argc, char *argv[]){
  char *path;
  FILE *fp;
  char buf[80];
  int opt;
  static struct option long_options[] = {
    {"add",    required_argument, NULL, 'a'},
    {"remove", required_argument, NULL, 'r'},
    {0, 0, 0, 0}
  };

  opt = getopt_long(argc, argv, "a:r:", long_options, NULL);
  switch(opt){
    case 'a':
      return cape_add(optarg);
    case 'r':
      printf("-r option causes a kernel panic.\n");
      printf("please reboot instead.\n");
      return 0;
      //return cape_remove(optarg);
    case '?':
      return -1;
    default:
      break;
  }

  if(argc == 1){
    fprintf(stderr, "Require argument.\n");
    return -1;
  }

  path = cape_path("/sys/devices", "bone_capemgr.");
  sprintf(buf, "%s/slots", path);
  free(path);
  fp = fopen(buf, "r");
  while(fgets(buf, 80, fp) != NULL){
    if(strstr(buf, argv[1]) != NULL){
      printf("%s", buf);
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  fprintf(stderr, "%s not found.\n", argv[1]);
  return -1;
}
