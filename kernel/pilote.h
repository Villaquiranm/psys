#ifndef PILOTE_H
#define PILOTE_H
#include "stdbool.h"
unsigned long cons_read(char * string, unsigned long length);
int cons_write(const char *str, long size);
void cons_echo(bool on);
void keyboard_data(char * string);
void kdb_leds(unsigned char leds);
void it_clavier();
#endif
