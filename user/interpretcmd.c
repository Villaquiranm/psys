#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include "strtok.h"
//#include <malloc.h>
#include "../lib/mem.h"
//#include "mallocfreelist.h"
//#include "../../kernel/processus.h"

char* read_cmd_line() {
  int buffer_size = CMD_LINE_BUFFER_SIZE;
  int current_pos = 0; // indice utilisée pour parcourir toute la ligne

  char* buffer = (char*) mem_alloc(sizeof(char)*buffer_size);
  if(buffer == NULL) {
    printf("Allocation error \n");
  }

  while(1) {
    char c = getchar();
    if (c == EOF || c == '\n') {
      buffer[current_pos] = '\0';
      return buffer;
    } else {
      buffer[current_pos] = c;
      current_pos ++;
    }

    if(current_pos >= buffer_size) {
      printf("buffer overflow\n" );
    }
    //si la taille de texte dépase à la taille d'un buffer, on va réallouer
    /*
          if(current_pos >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size* sizeof(char*));
            if(tokens == NULL) {
              fprintf(stderr, "Allocation error \n");
            }
          }
    */
  }
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
    token = own_strtok(line, TOKEN_DELIMITERS);
    // passe tous les autres tokens
    while(token != NULL) {
      printf("%s\n", token);
      tokens[current_pos] = token;
      current_pos++;
      if(current_pos >= buffer_size) {
        printf("buffer overflow\n" );
      }

/*
      if(current_pos >= buffer_size) {
        buffer_size += TOKEN_BUFFER_SIZE;
        tokens = realloc(tokens, buffer_size* sizeof(char*));
        if(tokens == NULL) {
          fprintf(stderr, "Allocation error \n");
        }
      }
*/
      token = own_strtok(NULL, TOKEN_DELIMITERS);
    }
  }


  int launch_processus(char **args) {
    int pid, wpid;
    int status;

  }
