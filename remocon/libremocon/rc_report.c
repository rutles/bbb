// rc_report.c
// libremocon member function

#include <remocon.h>

void rc_report(irform_t *ir){
  int i;

  printf("Format: ");
  printf(ir->lon < RC_FORMAT ? "IEHA, " : "NEC, ");
  printf(ir->multi ? "multi shot.\n" : "single shot.\n");
  printf("Leader ON: %dusec, OFF: %dusec.\n", ir->lon, ir->loff);
  printf("Signal T: %dusec, ", ir->t);
  printf("H: %dusec, ", ir->h);
  printf("L: %dusec.\n\n", ir->l);

  if(ir->multi)
    printf("1st shot ");

  printf("Signal count: %dbytes.\n", ir->count1);
  for(i = 0; i < ir->count1 - 1; i++){
    printf("%02X", ir->code1[i]);
    printf((i + 1) % 8 ? ", " : "\n");
  }
  printf("%02X.\n\n", ir->code1[i]);

  if(ir->multi){
    printf("Gap: %dusec.\n", ir->gap);
    printf("Interval: %dusec.\n\n", ir->interval);

    printf("2nd shot ");
    printf("Signal count: %dbytes.\n", ir->count2);
    for(i = 0; i < ir->count2 - 1; i++){
      printf("%02X", ir->code2[i]);
      printf((i + 1) % 8 ? ", " : "\n");
    }
    printf("%02X.\n\n", ir->code2[i]);
  }
}
