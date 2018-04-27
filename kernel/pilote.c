#include "pilote.h"
#include "fileMessage.h"
#include "stddef.h"
#include "stdbool.h"

bool echo;

unsigned long cons_read(char * string, unsigned long length){
  if (length == 0) {
    return 0;
  }
  unsigned long position = 0;
  while (string[position] != 13 && position < length) {
    position++;
  }
  return position;
}
int cons_write(const char *str, long size){
  //I supose that we will use fileMessage
  int fid = 0;
  for (int i = 0; i < size ;i++) {
    psend(fid , str[i]);
  }
  return 0;
}
void cons_echo(int on){
  if (on == 0) {
    echo = false;
  }
  else{
    echo = true;
  }
}
