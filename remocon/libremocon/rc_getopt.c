#include <remocon.h>

void rc_getopt(int argc, char *argv[], option_t *op){
  int opt;
  static struct option long_options[] = {
    {"receive",   required_argument, NULL, 'r'},
    {"transmit",  required_argument, NULL, 't'},
    {"directory", required_argument, NULL, 'd'},
    {0, 0, 0, 0}
  };

  op->r = 0;
  op->t = NULL;
  op->d = NULL;

  while((opt = getopt_long(argc, argv,
    "r:t:d:",
    long_options, NULL)) != -1){
    switch(opt){
      case 'r':
        op->r = atoi(optarg);
        break;
      case 't':
        op->t = (char *)optarg;
        break;
      case 'd':
        op->d = (char *)optarg;
        break;
      default:
        break;
    }
  }

  if(op->r == 0)
    op->r = RC_PIN;
  if(op->t == NULL)
    op->t = (char *)RC_POUT;
  if(op->d == NULL)
    op->d = (char *)get_current_dir_name();
}
