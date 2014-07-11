// rc_statist.c
// libremocon member function

#include <remocon.h>

int rc_center(int *array, int count){
  int temp;
  int i, j;

  // sort
  for(i = 0; i < count - 1; i++)
    for(j = count - 1; j > i; j--)
      if(array[j - 1] > array[j]){
        temp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = temp;
      }

  return array[count / 2];
}

void rc_statist(irform_t *ir){
  int i;
  int count;
  int *work;

  // analyze T
  work = (int *)calloc(ir->length, sizeof(int));
  for(i = 0; i < ir->length; i++)
    work[i] = ir->son[i];
  ir->t = rc_center(work, ir->length);

  // analyze H length
  count = 0;
  for(i = 0; i < ir->length; i++){
    if(ir->t * 2 < ir->soff[i]){
      work[count++] = ir->soff[i];
    }
  }
  ir->h = rc_center(work, count);

 // analyze L length
  count = 0;
  for(i = 0; i < ir->length; i++){
    if(ir->t * 2 >= ir->soff[i]){
      work[count++] = ir->soff[i];
    }
  }
  ir->l = rc_center(work, count);

  free(work);
}
