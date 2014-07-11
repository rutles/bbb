// rc_save.c
// libremocon member function

#include <remocon.h>

int rc_save(int serial, const char *dir, irform_t *ir){
  FILE *file, *ftmp;
  char buf[80];
  int i, j;

  // check memo file exists
  ftmp = fopen("/tmp/rcmemo00.tmp", "rb");
  if(ftmp == NULL)
    return 0;
  fclose(ftmp);

  // save
  sprintf(buf, "%s/analyzer.conf", dir);
  file = fopen(buf, "w");
  if(file == NULL)
    return -1;

  fprintf(file, "# This file was automatically generated\n");
  fprintf(file, "leader = %d : %d\n", ir->lon, ir->loff);
  fprintf(file, "signal = %d : %d : %d\n", ir->t, ir->h, ir->l);
  fprintf(file, "interval = %d\n\n", ir->multi ? ir->interval : 20000);

  for(i = 0; i < serial; i++){
    sprintf(buf, "/tmp/rcmemo%02d.tmp", i);
    ftmp = fopen(buf, "rb");
    if(ftmp == NULL){
      fclose(file);
      return -1;
    }

    fread(&ir->multi, sizeof(int), 1, ftmp);
    fread(&ir->count1, sizeof(int), 1, ftmp);
    fread(ir->code1, sizeof(uint8_t), ir->count1, ftmp);
    if(ir->multi){
      fread(&ir->count2, sizeof(int), 1, ftmp);
      fread(ir->code2, sizeof(uint8_t), ir->count2, ftmp);
    }
    fclose(ftmp);
    //remove(buf);

    fprintf(file, "button = %02d\n", i);
    fprintf(file, "multi = %s\n", ir->multi ? "true" : "false");
    fprintf(file, "count1 = %d", ir->count1);
    for(j = 0; j < ir->count1; j++){
      if((j % 8) == 0) fprintf(file, "\n");
      fprintf(file, "%02X", ir->code1[j]);
      fprintf(file, (j < ir->count1 - 1) ? ", " : "\n");
    }

    if(ir->multi){
      fprintf(file, "count2 = %d", ir->count2);
      for(j = 0; j < ir->count2; j++){
        if((j % 8) == 0) fprintf(file, "\n");
        fprintf(file, "%02X", ir->code2[j]);
        fprintf(file, (j < ir->count2 - 1) ? ", " : "\n");
      }
    }
    fprintf(file, "\n");
  }
  fclose(file);
  return 0;
}
