#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mem.h"


char* read_cmd_line(char** cmd_line) {
  int buffer_size = CMD_LINE_BUFFER_SIZE;
  int current_pos = 0; // indice utilisée pour parcourir toute la ligne

  char* buffer = (char*) mem_alloc(sizeof(char)*buffer_size);
  if(buffer == NULL) {
    printf("Allocation error \n");
  }

//TODO: a verifier si c'est correcte
  while(**cmd_line != '\n') {
    //On saute les espaces vides
    while(**cmd_line == ' ') {
      cmd_line++;
    }
    buffer[current_pos] = **cmd_line;
    current_pos++;
    cmd_line++;
  }
  buffer[current_pos] = '\0';

  if(current_pos >= buffer_size) { 
    printf("buffer overflow\n" );
  }
  return buffer;
}

  char **split_cmd_line(char* line) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    int current_pos = 0;
    char **tokens = mem_alloc(buffer_size * sizeof(char*));
    char *token;


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
    return tokens;
  }

  char* extraire_cmd(char **tokens) {
    return tokens[0];
  }

  char** extraire_param(char **tokens) {
    char** new_tokens = tokens + 1;
    return new_tokens;
  }

  int builtin_commande(char *cmd, char **param) {
      if(strcmp(cmd, "exit") == 0) {
        exit(0);
      } else if(strcmp(cmd, "cd") == 0) {
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
/*
  void prompt_msg(char* msg) {
    char hostName[MAX_LENGTH_HN];
    char pathName[MAX_LENGTH_PN];

  }
  */
