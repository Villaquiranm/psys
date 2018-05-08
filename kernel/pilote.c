#include "pilote.h"
#include "fileMessage.h"
#include "stddef.h"
#include "cpu.h"
#include "kbd.h"
#include "console.h"
#include "mem.h"

#define TAILLE_TAMPON 100

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
  if (position < length) {
    numMessages--;
  }
  return position;
}

int cons_write(const char *str, long size){
  console_putbytes(str,size);
  printf("\n");
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
  if (estPleine(fid)) {
    //do beep

  }else{
    for (int i = 0; string[i] != '\0';i++) {
      char actuel = string[i];
      if ((actuel >= 32) && (actuel < 127)) {
        if (echo) {
          printf("%c",actuel);
        }
        psend(fid, actuel); //Il faut les envoyer bien sur.
      }
      else if (actuel == 127){// character BackSpace
        if (echo) {
          printf("\b");
          printf(" ");
          printf("\b");
        }
        last_char(fid);
      }
      else if (actuel == 13){//Caracter Enter We need to unblock the read line here.
        char s = 0xA;
        if (echo) {
          printf("%c",s);
        }
        psend(fid, actuel); //Il faut les envoyer bien sur.
        numMessages++;
        //char bola[10] ;
        //unsigned long length = cons_read(bola, 10); //Uncomment to test
        //cons_write(bola, length);
      }
      else if (actuel < 33) { //Characteres de control, sont affiches comme sa
        if (echo) {
          printf("^%c",actuel + 64);
        }
      psend(fid,'^');
      psend(fid, actuel+64);
      }
    }
  }
}

void it_clavier(){
  if (!initialized) {
    fid = pcreate(TAILLE_TAMPON);
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
char * createBuffer(int length){
  return mem_alloc(sizeof(char*) * length);
}
