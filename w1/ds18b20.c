// ds18b20.c - DS18B20+ handling example
// compile: cc ds18b20.c -o ds18b20 -lbbb
// execute: ./ds18b20

#include <bbb.h>

main(){
  int i;
  char *id;
  int tmp;

  id = w1_setup("BB-W1");
  if(id == NULL){
    perror("w1_setup");
    return;
  }

  //Display ID
  printf("Device ID: %s\n\n", id);

  do{
    tmp = w1_read(id);
    if(tmp == 2147483647){
      perror("w1_read");
      goto exit;
    }

    //Display temperature
    printf("Tmp=%7.3f\n", (float)tmp / 1000);
    sleep(1);
  } while(tmp < 28000);

exit:
  free(id);
}
