// rc_speccmp.c
// libremocon member function

#include <remocon.h>

int rc_speccmp(irform_t *ir1, irform_t *ir2){
  int i;// loop counter

  if(ir1->count1 != ir2->count1)
    return -1;

  for(i = 0; i < ir1->count1; i++)
    if(ir1->code1[i] != ir2->code1[i])
      return -1;

  if(ir1->multi != ir2->multi)
    return -1;

  if(ir1->multi){
    if(ir1->count2 != ir2->count2)
      return -1;

    for(i = 0; i < ir1->count2; i++)
      if(ir1->code2[i] != ir2->code2[i])
        return -1;
  }
  return 0;
}
