// rc_getdata.c
// libremocon member function

#include <remocon.h>

int rc_getdata(FILE *file, const char *button, irform_t *ir){
  char *s;
  int i;
  int dummy_multi;
  int dummy_count;

  ir->lon = ir->loff = 0;
  ir->t = ir->h = ir->l = 0;
  ir->multi = -1;
  ir->interval = 0;
  ir->count1 = ir->count2 = 0;
  ir->interval = 0;

  fseek(file, 0, SEEK_SET);
  while((s = rc_gettoken(file)) != NULL){
    if(strcmp(s, "leader") == 0){
      ir->lon = atoi(rc_gettoken(file));
      ir->loff = atoi(rc_gettoken(file));
    } else
    if(strcmp(s, "signal") == 0){
      ir->t = atoi(rc_gettoken(file));
      ir->h = atoi(rc_gettoken(file));
      ir->l = atoi(rc_gettoken(file));
    } else
    if(strcmp(s, "interval") == 0){
      ir->interval = atoi(rc_gettoken(file));
    } else
    if(strcmp(s, "button") == 0){
      if(strcmp(button, rc_gettoken(file)) == 0){
        rc_gettoken(file);// skip "multi"
        ir->multi = (strcmp(rc_gettoken(file), "true") == 0) ? 1 : 0;
        rc_gettoken(file);// skip "count" or "count1"
        ir->count1 = atoi(rc_gettoken(file));
        ir->code1 = (uint8_t *)calloc(ir->count1, sizeof(uint8_t));
        for(i = 0; i < ir->count1; i++)
          ir->code1[i] = (uint8_t)strtol(rc_gettoken(file), NULL, 16);
        if(ir->multi){
          rc_gettoken(file);// skip "count2"
          ir->count2 = atoi(rc_gettoken(file));
          ir->code2 = (uint8_t *)calloc(ir->count2, sizeof(uint8_t));
          for(i = 0; i < ir->count2; i++)
           ir->code2[i] = (uint8_t)strtol(rc_gettoken(file), NULL, 16);
        }
      } else {
        rc_gettoken(file); // skip "multi"
        dummy_multi = (strcmp(rc_gettoken(file), "true") == 0) ? 1 : 0;
        rc_gettoken(file); // skip "count" or "count1"
        dummy_count = atoi(rc_gettoken(file));
        for(i = 0; i < dummy_count; i++)
          rc_gettoken(file);
        if(dummy_multi){
          rc_gettoken(file);// skip "count2"
          dummy_count = atoi(rc_gettoken(file));
          for(i = 0; i < dummy_count; i++)
            rc_gettoken(file);
        }
      }
    }
  }

  if(
  ir->lon == 0 || ir->loff == 0 ||
  ir->t == 0 || ir->h == 0 || ir->l == 0 ||
  ir->multi == -1 ||
  ir->count1 == 0 ||
  (ir->multi == 1 && ir->count2 == 0) ||
  (ir->multi == 1 && ir->interval == 0))
    return -1;

  return 0;
}
