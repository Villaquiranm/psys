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
    wait_clock(20);
  }
  if (length < 1) {
    return 0;
  }
  int chara;
  unsigned long position = 0;
  preceive(fid, &chara);
  string[position] = (char)(chara);
  while (chara != 13 && position < length) {
    position++;
    preceive(fid, &chara);
    string[position] = (char)(chara);
  }
  if (position > length) {
    numMessages--;
  }
  return position;
}

int cons_write(const char *str, long size){
  for (int i = 0; i < size ;i++) {
    printf("%c", str[i]);
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
  if ((string[0] >= 32) && (string[0] < 127)) {
    if (echo) {
      printf("%c",string[0]);
    }
    psend(fid, string[0]); //Il faut les envoyer bien sur.
  }
  else if (string[0] == 127){// character BackSpace
    if (echo) {
      printf("\b");
      printf(" ");
      printf("\b");
    }
    last_char(fid);
  }
  else if (string[0] == 13){//Caracter Enter We need to unblock the read line here.
    char s = 0xA;
    if (echo) {
      printf("%c",s);
    }
    psend(fid, string[0]); //Il faut les envoyer bien sur.
    numMessages++;
    unsigned long length = cons_read(string, 10);
    cons_write(string, length);
  }
  else if (string[0] < 33) { //Characteres de control, sont affiches comme sa
    if (echo) {
      printf("^%c",string[0] + 64);
    }
  }
}
/*void keyboard_data(char * string){
  for (int i = 0; string[i] != '\0';i++) {
    int var = 0;
    char actuelChar = string[i];

      if (actuelChar == 9 || (actuelChar > 31 && actuelChar < 127)) {//Characteres affichables
        if (echo) {
          printf("%c",actuelChar);
        }
        psend(fid, actuelChar); //Il faut les envoyer bien sur.
      }
      else if (actuelChar == 13){//Caracter Enter We need to unblock the read line here.
        var++;
        printf("%d",var);
        char s = 0xA;
        if (echo) {
          printf("%c",s);
        }
        psend(fid, actuelChar); //Il faut les envoyer bien sur.
        numMessages++;
        //unsigned long length =
         cons_read(string, 10);
        //cons_write(string, length);
      }
      else if (actuelChar == 127){// character BackSpace
        if (echo) {
          printf("\b");
          printf(" ");
          printf("\b");
        }
        last_char(fid);
      }
      else if (actuelChar < 33) { //Characteres de control, sont affiches comme sa
        if (echo) {
          printf("^%c",actuelChar + 64);
        }
      }
  }
}*/
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
