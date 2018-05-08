#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mem.h"
#include "pilote.h"
#include "console.h"

char * buffer;
static int compare(const char* chaine1, const char* chaine2)
{   unsigned int i=0;
    if( strlen(chaine1) != strlen(chaine2) )
        return faux;
    for(i=0;i<strlen(chaine1);i++)
        if( chaine1[i] != chaine2[i])
            return faux;
    return vrai;
}
void select_color(){
  int choix = 0;
  printf("\n\t============================================\n");
  printf("\t               Select color\n");
  printf("\t--------------------------------------------\n");
  printf("\t   0 : Black     1 : Blue      2 : Green       \n");
  printf("\t   3 : Cyan      4 : Red       5 : Magenta     \n");
  printf("\t   6 : Brown     7 : Gray      8 : Dark Gray   \n");
  printf("\t   9 : BlueII    10: GreenII   11: CyanII       \n");
  printf("\t   12: RedII     13: MagentaII 14: Yellow       \n");
  printf("\t                 15: White                      \n");
  printf("\t============================================\n");
  printf("\n\n");
  printf("Veuillez choisir un couleur en tapant le numero correspondant:");
  switch (cons_read(buffer, CMD_LINE_BUFFER_SIZE)) {
    case 1:
      choix += buffer[0]-'0';
    break;
    case 2:
      choix += (buffer[0]-'0')*10 + buffer[1]-'0';
    break;
  };
  if (choix >= 0 && choix <= 15) {
      change_couleur(choix);
  }
}


 int builtin_command(char* cmd, char** param) {
    if(compare(cmd, "color") == vrai) {
      //printf("Je suis la commande echo\n");
      /*if (param[0][0] == '\"') {
        //printf("je commence a lire! Le contenu de echo est suivant:\n");
        int i = 1;
        while(param[0][i] != '\"') {
        printf("%c",param[0][i]);
        i++;
        if(i>50){
          printf("erreur!");
        }
        }
      }*/
      select_color();
    } else if(compare(cmd, "echo") == vrai){
      //printf("Je suis la commande echo\n");
      if(param[0][0] == '\"') {
        //printf("je commence a lire! Le contenu de echo est suivant:\n");
        int i = 1;
        while(param[0][i] != '\"') {
          printf("%c",param[0][i]);
          i++;
          if(i>50){
            printf("erreur!\n");
          }
        }
      }


    } else if(compare(cmd, "help") == vrai) {
        printf("les commandes disponibles:\n");
        printf("        echo : afficher un string entre guillemets");
      }

     else {
       printf("La commande n'est pas reconnue!\n");
      }

    return -1;
  }

//-----------------------------------
/*char **split_cmd_line(char* line, unsigned long length) {
  int number_cmd = 1;
  int current_pos = 0;
  char * token;
  for (unsigned long i = 0; i<length ; i++) {
    if (line[i] ==' ') {
      number_cmd++;
    }
*/
  char **split_cmd_line(char* line, unsigned long* length) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    int current_pos = 0;
    char **tokens = mem_alloc(buffer_size * sizeof(char*));
    char *token;
    *length = 0;

    if(tokens == NULL) {
      printf("Allocation error \n");
    }
    // Retourner le premier token, Null si il y n'a plus de token
    token = strtok(line, TOKEN_DELIMITERS);
    tokens[current_pos] = token;
    //printf("%s\n", tokens[current_pos]);
    //current_pos++;
    // passe tous les autres tokens
    while(token != NULL) {
      tokens[current_pos] = token;
      //printf("%s\n", tokens[current_pos]);
      current_pos++;
      if(current_pos >= buffer_size) {
        printf("buffer overflow\n" );
      }
      token = strtok(NULL, TOKEN_DELIMITERS);
    }
    *length = current_pos;
  //  printf("%lu\n", *length); //pour test
    return tokens;
  }
  /*
  token = strtok(line, TOKEN_DELIMITERS);
  //printf("Soy el token 1: %s\n",token);
  char **tokens = mem_alloc(number_cmd * sizeof(char*));

  while(token != NULL) {
  //  printf("%s\n", token);
    tokens[current_pos] = token;
    current_pos++;
    token = strtok(NULL, TOKEN_DELIMITERS);
    mem_free(tokens,sizeof())
  }
  for (int s = 0 ; s < current_pos; s++) {
    printf("%s\n",tokens[s]);
  }
  return tokens;
}
*/


   char* extraire_cmd(char **tokens) {
    //printf("%s\n",tokens[0] ); //pour tester
    return tokens[0];
  }

   char** extraire_param(char **tokens, unsigned long* length) {
     if(*length ==1) {
       return NULL;
     }
    char** new_tokens = tokens + 1;
    //printf("%s\n", *new_tokens++ );
    return new_tokens;
  }

  void type_prompt() {
      char hostName[] = "yum";
      printf("[Minishell %s@%s]$",hostName,hostName);
      return;
  }

  void welcomeScreen() {
        printf("\n\t============================================\n");
        printf("\t               Simple C Shell\n");
        printf("\t--------------------------------------------\n");
        printf("\t            Projet Systeme -- 2A ENSIMAG:\n");
        printf("\t============================================\n");
        printf("\n\n");
  }

  int miniShell() {
    buffer = createBuffer(CMD_LINE_BUFFER_SIZE);
    unsigned long nb_wd = 0;
    //unsigned long size_cmd_line;
    char **tokens;
    char *cmd;
    char **param;
    welcomeScreen();
    while(true) {
      type_prompt();
      /*unsigned long length =*/ cons_read(buffer, CMD_LINE_BUFFER_SIZE);
      //cons_write(buffer, length);
      //printf("%lu\n",length);
      tokens = split_cmd_line(buffer, &nb_wd);
    //  printf("%lu\n", nb_wd);
      cmd = extraire_cmd(tokens);
      param = extraire_param(tokens, &nb_wd);
      builtin_command(cmd, param);
      printf("\n");
    }
    /*
    while(tokens != NULL) {
      printf("%s\n", *tokens++);
    }*/

    return 0;
  }
