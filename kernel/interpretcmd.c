#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mem.h"
#include "pilote.h"
#include "console.h"
#include "tests.h"
//#include "mem_allocfreelist.h"

//les variables globales
char *buffer;
int his_current_pos = 0;
char **buffer_his;
int prechoix = 15;


static int compare(const char* chaine1, const char* chaine2)
{   unsigned int i=0;
    if( strlen(chaine1) != strlen(chaine2) )
        return faux;
    for(i=0;i<strlen(chaine1);i++)
        if( chaine1[i] != chaine2[i])
            return faux;
    return vrai;
}

static void select_color(){

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
  int choix = 0;
  switch (cons_read(buffer, CMD_LINE_BUFFER_SIZE)) {
    case 1:
      choix += buffer[0]-'0';
    break;
    case 2:
      choix += (buffer[0]-'0')*10 + buffer[1]-'0';
    break;
  };
  if (choix >= 0 && choix <= 15) {
    prechoix = choix;
      change_couleur(choix);
  }
}

static void save_cmd(char* cmd_line) {
  int buffer_size = HIS_BUFFER_SIZE;
  int pos = his_current_pos;
  char* tempo = mem_alloc(sizeof(char) * CMD_LINE_BUFFER_SIZE);
  if(his_current_pos >= buffer_size) {
    printf("buffer overflow\n" );
  } else {
      buffer_his[pos] = strcpy(tempo, cmd_line);
      his_current_pos++;
  }
}

static void affiche_history(int pos) {
  for(int i = pos - 1; i >= 0; i--) {
    printf("%d  %s\n", i, buffer_his[i]);
  }
}

static int builtin_command(char* cmd, char** param, unsigned long* param_length) {
      if(compare(cmd, "test") == vrai) {
        save_cmd(buffer);
        execute_tests();
        return 7;
      }
      if(compare(cmd, "ps") == vrai) {
        save_cmd(buffer);
        sys_info();
        return 6;
      }
      if(compare(cmd, "exit") == vrai) {
        save_cmd(buffer);
        exitFunction(1);
        return 5;
      }
      if(compare(cmd, "color") == vrai) {
        save_cmd(buffer);
        select_color();
        return 4;
      } else if(compare(cmd, "echo") == vrai){
        if(param[0][0] == '\"' && *param_length >2) {
          int i = 1;
          while(param[0][i] != '\"') {
            printf("%c",param[0][i]);
            i++;
            if(i>50){
              printf("Erreur: La taille maximun d'un string est limite a 50 caractere!\n");
              break;
            }
          }
        } else {
          for(unsigned long j = 0; j < *param_length - 2; j++) {
             for(unsigned long i = 0; i<strlen(param[j]); i++) {
              printf("%c", param[j][i]);
              if (i > TOKEN_BUFFER_SIZE) {
                break;
              }
            }
            printf(" ");
          }
        }
        printf("\n");
        save_cmd(buffer);
        return 3;
      } else if(compare(cmd, "help") == vrai) {
          printf("les commandes disponibles:\n");
          printf("\techo  : afficher un string entre guillemets\n");
          printf("\tcolor : changer color teste\n");
          printf("\texit  : sortir de la fonction\n");
          printf("\tps    : afficher les processus\n");
          printf("\ttest  : lancer tous les testes\n");

          save_cmd(buffer);
          return 2;
      } else if(compare(cmd, "history") == vrai) {
          save_cmd(buffer);
          affiche_history(his_current_pos);
          return 1;
      }
    return -1;
  }


  char **split_cmd_line(char* line, unsigned long* param_length) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    int current_pos = 0;
    char **tokens = mem_alloc(buffer_size * sizeof(char*));
    char *token;
    *param_length = 0;

    if(tokens == NULL) {
      printf("Allocation error \n");
    }
    // Retourner le premier token, Null si il y n'a plus de token
    token = strtok(line, TOKEN_DELIMITERS);
    tokens[current_pos] = token;
    while(token != NULL) {
      tokens[current_pos] = token;
      current_pos++;
      if(current_pos >= buffer_size) {
        printf("buffer overflow\n" );
      }
      token = strtok(NULL, TOKEN_DELIMITERS);
    }
    *param_length = current_pos;
    return tokens;
  }

   char* extraire_cmd(char **tokens) {
    return tokens[0];
  }

   char** extraire_param(char **tokens, unsigned long* length) {
     if(*length ==1) {
       return NULL;
     }
    char** new_tokens = tokens + 1;
    return new_tokens;
  }

  static void type_prompt() {
      char hostName[] = "BestTeam";
      char domainName[] = "grenoble-inp";
      change_couleur(2);
      printf("[Minishell %s@%s]$ ",hostName,domainName);
      change_couleur(prechoix);
      return;
  }

  static void welcomeScreen() {
    change_couleur(5);

        printf("\t  _____                      _            _      ____   _____ \n");
        printf("\t |  __ \\                    | |          | |    / __ \\ / ____|\n");
        printf("\t | |__) |___ _ __ ___   ___ | | __ _  ___| |__ | |  | | (___    \n");
        printf("\t |  _  // _ \\ '_ ` _ \\ / _ \\| |/ _` |/ __| '_ \\| |  | |\\___ \\  \n");
        printf("\t | | \\ \\  __/ | | | | | (_) | | (_| | (__| | | | |__| |____) |    \n");
        printf("\t |_|  \\_\\___|_| |_| |_|\\___/|_|\\__,_|\\___|_| |_|\\____/|_____/     \n");
        change_couleur(prechoix);
        printf("\n");
        printf("\n\t      ============================================\n");
        printf("\t                      Mini C Shell\n");
        printf("\t      --------------------------------------------\n");
        printf("\t                  Projet Systeme -- 2A ENSIMAG:\n");
        printf("\t      ============================================\n");
        printf("\n\n");

  }


  int miniShell() {
    buffer = createBuffer(CMD_LINE_BUFFER_SIZE);

    unsigned long nb_wd = 0;
    char **tokens;
    char *cmd;
    char **param;

    welcomeScreen();
    buffer_his = (char **) mem_alloc(sizeof(char*)* HIS_BUFFER_SIZE);

    while(true) {
      type_prompt();
      cons_read(buffer, CMD_LINE_BUFFER_SIZE);
      tokens = split_cmd_line(buffer, &nb_wd);
      cmd = extraire_cmd(tokens);
      param = extraire_param(tokens, &nb_wd);
      builtin_command(cmd, param, &nb_wd);
    }
    return 0;
  }
