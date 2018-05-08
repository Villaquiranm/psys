#ifndef INTERPRETCMD_H_
#define INTERPRETCMD_H_


#define CMD_LINE_BUFFER_SIZE 50
#define TOKEN_BUFFER_SIZE 50
#define TOKEN_DELIMITERS " "

#define vrai  1
#define faux  0

extern void exit(int retval);

typedef struct cmd_lines {
  char** cmd_line; //la ligne de commande : qui est composée d'une commande qui suit des paramètres
  int nb_param; //nombre de paramètres
} CMD_LINES;

/* Structure returned by parsecmd() */
struct cmdline {
	char *err;	/* If not null, it is an error message that should be
			   displayed. The other fields are null. */
	char *in;	/* If not null : name of file for input redirection. */
	char *out;	/* If not null : name of file for output redirection. */
        int   bg;       /* If set the command must run in background */
	char ***seq;	/* See comment below */
};

/* Field seq of struct cmdline :
A command line is a sequence of commands whose output is linked to the input
of the next command by a pipe. To describe such a structure :
A command is an array of strings (char **), whose last item is a null pointer.
A sequence is an array of commands (char ***), whose last item is a null
pointer.
When the user enters an empty line, seq[0] is NULL.
*/


//une fonction qui lit et retourne la ligne de commande
char *read_cmd_line(char** cmd_line);
//classifier les tokens
char **split_cmd_line(char *line, unsigned long length);
//commencer un processus
//int launch_processus(char **args);

void welcomeScreen();

void type_prompt();
int miniShell();


#endif
