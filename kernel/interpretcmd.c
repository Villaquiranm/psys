#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mem.h"
#include "pilote.h"

int compare(const char* chaine1, const char* chaine2)
{   unsigned int i=0;
    if( strlen(chaine1) != strlen(chaine2) )
        return faux;
    for(i=0;i<strlen(chaine1);i++)
        if( chaine1[i] != chaine2[i])
            return faux;
    return vrai;
}

 int builtin_command(char* cmd, char** param) {
    if(compare(cmd, "echo") == vrai) {
      printf("Je suis la commande echo\n");
      if (param[0][0] == '\"') {
        while( **param != '\"'){
          param ++;
          if(**param == '\0') {
            printf("la paramètre n'est pas reconnu!");
            break;
          }
        }
      }
    } else {
      printf("La commande n'est pas reconnue!\n");
    }
    return -1;
  }

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
    // passe tous les autres tokens
    while(token != NULL) {
      printf("%s\n", token);
      tokens[current_pos] = token;
      current_pos++;
      if(current_pos >= buffer_size) {
        printf("buffer overflow\n" );
      }
      token = strtok(NULL, TOKEN_DELIMITERS);
    }
    *length = current_pos;
    printf("%lu\n", *length); //pour test
    return tokens;
  }


   char* extraire_cmd(char **tokens) {
     printf("%s\n",tokens[0] ); //pour tester
    return tokens[0];
  }

   char** extraire_param(char **tokens) {
    char** new_tokens = tokens + 1;
    return new_tokens;
  }

  int builtin_commande(char *cmd, char **param) {
      if(strcmp(cmd, "exit") == 0) {
        return -1;
      }

      else if(strcmp(cmd, "cd") == 0) {
        char *cd_path = NULL;
        if(param[0][0] == '~'){
          cd_path = mem_alloc(sizeof(char)*strlen("/home/user/"));
          if(cd_path == NULL) {
            printf("allocation error\n");
          }
          strcpy(cd_path,"/home/user/");
        } else {
          cd_path = mem_alloc(strlen(param[1]+1));
          if(cd_path == NULL) {
            printf("allocation error\n");
          }
          strcpy(cd_path, param[1]);
        }
      }

      return 0;
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
    char * buffer = createBuffer(CMD_LINE_BUFFER_SIZE);
    unsigned long nb_wd = 0;
    //unsigned long size_cmd_line;
    char **tokens;
    welcomeScreen();
    type_prompt();
    /*unsigned long length =*/ cons_read(buffer, CMD_LINE_BUFFER_SIZE);
    //cons_write(buffer, length);
    //printf("%lu\n",length);
    tokens = split_cmd_line(buffer, &nb_wd);
    extraire_cmd(tokens);
    /*
    while(tokens != NULL) {
      printf("%s\n", *tokens++);
    }*/

    return 0;
  }
