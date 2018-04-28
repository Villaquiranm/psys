#include "pilote.h"
#include "fileMessage.h"
#include "stddef.h"
#include "stdbool.h"
#include "cpu.h"
#include "kbd.h"
bool echo;
bool initialized = false;
int fid;

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
void keyboard_data(char * string){
  printf("%s\n",string);
}
void it_clavier(){
  /*if (!initialized) {
    fid = pcreate(100);
    printf("Creating file de message FID : %d\n",fid);
    initialized = true;
  }*/
  do_scancode(inb(0x60));
  outb(0x20, 0x20);
  outb(0x20, 0x20);
}
