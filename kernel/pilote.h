#ifndef PILOTE_H
#define PILOTE_H

unsigned long cons_read(char * string, unsigned long length);
int cons_write(const char *str, long size);
void cons_echo(int on);
#endif
