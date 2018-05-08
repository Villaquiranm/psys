#ifndef INTERPRETCMD_H_
#define INTERPRETCMD_H_


#define CMD_LINE_BUFFER_SIZE 50
#define TOKEN_BUFFER_SIZE 50
#define TOKEN_DELIMITERS "\t\r\n\a"

extern void exit(int retval);

typedef struct cmd_lines {
  char** cmd_line; //la ligne de commande : qui est composée d'une commande qui suit des paramètres
  int nb_param; //nombre de paramètres
} CMD_LINES;

//une fonction qui lit et retourne la ligne de commande
char *read_cmd_line(char** cmd_line);
//classifier les tokens
char **split_cmd_line(char *line);
//commencer un processus
//int launch_processus(char **args);

void welcomeScreen();

void type_prompt();
int miniShell();


#endif
