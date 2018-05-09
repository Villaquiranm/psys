#ifndef INTERPRETCMD_H_
#define INTERPRETCMD_H_


#define CMD_LINE_BUFFER_SIZE 50
#define HIS_BUFFER_SIZE 50
#define TOKEN_BUFFER_SIZE 50
#define MIX_WORDS_ECO 30
#define TOKEN_DELIMITERS " \t\r\n\a"

#define vrai  1
#define faux  0

extern void exit(int retval);

int miniShell();


#endif
