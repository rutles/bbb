// rc_memo.c
// libremocon member function

#include <remocon.h>

int rc_memo(int serial, irform_t *ir){
  FILE *ftmp;
  char buf[40];

  if(serial > 99)
    return -1;

  sprintf(buf, "/tmp/rcmemo%02d.tmp", serial);
  ftmp = fopen(buf, "wb");
  if(ftmp == NULL)
    return -1;

  fwrite(&ir->multi, sizeof(int), 1, ftmp);
  fwrite(&ir->count1, sizeof(int), 1, ftmp);
  fwrite(ir->code1, sizeof(uint8_t), ir->count1, ftmp);
  if(ir->multi){
    fwrite(&ir->count2, sizeof(int), 1, ftmp);
    fwrite(ir->code2, sizeof(unsigned char), ir->count2, ftmp);
  }

  fclose(ftmp);
  return 0;
}
