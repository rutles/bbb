// rc_getButtons.c
// libremocon member function

#include <remocon.h>

int rc_getbuttons(FILE *file, char ***pbutton){
  char *s;
  int count;

  // count buttons
  count = 0;
  fseek(file, 0, SEEK_SET);
  while((s = rc_gettoken(file)) != NULL){
    if(strcmp(s, "button") == 0)
      count++;
  }

  // read buttons
  *pbutton = (char **)calloc(count, sizeof(char *));
  count = 0;
  fseek(file, 0, SEEK_SET);
  while((s = rc_gettoken(file)) != NULL){
    if(strcmp(s, "button") == 0){
      s = rc_gettoken(file);
      (*pbutton)[count++] = strdup(s);
    }
  }

  return count;
}
