#include "pilote.h"
#include "fileMessage.h"
#include "stddef.h"
#include "cpu.h"
#include "kbd.h"
bool echo = true;
bool initialized = false;
int fid;
int numMessages = 0;

unsigned long cons_read(char * string, unsigned long length){
  while (numMessages < 1) {
    
  }
  if (length == 0) {
    return 0;
  }
  unsigned long position = 0;
  while (string[position] != 13 && position < length) {
    position++;
    if (string[position] == 13) {
      numMessages--;
    }
  }
  return position;
}
int cons_write(const char *str, long size){
  //I supose that we will use fileMessage
  for (int i = 0; i < size ;i++) {
    psend(fid , str[i]);
  }
  return 0;
}
void cons_echo(bool on){
  if (on) {
    echo = true;
  }
  else{
    echo = false;
  }
}
void keyboard_data(char * string){
  for (int i = 0; string[i] != '\0';i++) {
    char actuelChar = string[i];
    if (echo){
      if (actuelChar == 9 || (actuelChar > 31 && actuelChar < 127)) {//Characteres affichables
        printf("%s",string);
      }
      else if (actuelChar == 13){//Caracter Enter We need to unblock the read line here.
        char s = 0xA;
        printf("%c",s);
        numMessages++;
      }
      else if (actuelChar == 127){// character BackSpace
        printf("\b");
        printf(" ");
        printf("\b");
      }
      else if (actuelChar < 33) { //Characteres de control, sont affiches comme sa
        printf("^%c",actuelChar + 64);
      }
    }
    //psend(fid, actuelChar); //Il faut les envoyer bien sur.
  }
}
void it_clavier(){
  if (!initialized) {
    fid = pcreate(100);
    printf("Creating file de message FID : %d\n",fid);
    initialized = true;
  }
  do_scancode(inb(0x60));
  outb(0x20, 0x20);
  outb(0x20, 0x20);
}
void kdb_leds(unsigned char leds){
  outb(0x60, 0xED);
  outb(0x60, leds);
}
