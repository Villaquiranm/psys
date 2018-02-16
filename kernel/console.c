#include "string.h"
#include "cpu.h"
#include "inttypes.h"
#include "console.h"

#define MEM_VIDEO 0xB8000
#define NB_LIG 25
#define NB_COL 80
#define NOIR 0
#define BLANC 15
#define ESPACE 32
#define PORT_CMD_VIDEO 0x3D4
#define PORT_DATA_VIDEO 0x3D5

//Ecran: 25 lignes et 80 col
//MEM_VIDEO 0xB8000 -> 0xB9999

//Global variables
uint32_t lig = 1;
uint32_t col = 0;
uint16_t fond = 0;
uint16_t couleur_c = 15;
uint16_t cling = 0;

uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
  return (uint16_t*)(MEM_VIDEO + 2*(lig*NB_COL + col));
}

void ecrit_car(uint32_t lig, uint32_t col, char c) {
  uint16_t *position = ptr_mem(lig, col);
  *position = (cling << 8 | fond << 4 | couleur_c) << 8 | c;

}

void efface_ecran(void){
  /* Fonction qui ecrit un espace en blanc sur un fond noir dans chaque position de memoire,
  pour efface l'ecran;
  Chaque position somme 2 a position actuel */
  int lig, col;
  for(col = 0; col < 80; col++) {
    for(lig = 0; lig < 25; lig++){
      ecrit_car(lig, col, ' ');
    }
  }
}

void place_curseur(uint32_t lig, uint32_t col){


  /* Fonction place le curseur a la position donnee  */
  uint16_t pos = col + lig*80;
  uint8_t value;

  value = (uint8_t)pos; //Les bits baisses
  outb(0x0F, PORT_CMD_VIDEO);
  outb(value, PORT_DATA_VIDEO);

  value = (uint8_t)(pos>> 8); //Les bits hautes
  outb(0x0E, PORT_CMD_VIDEO);
  outb(value, PORT_DATA_VIDEO);

}

void defilement(void){
  uint16_t *dest = ptr_mem(1,0);
  uint16_t *src = ptr_mem(2,0);
  memmove(dest,src, 4000);
}


void traite_car(char c){
  /* Traite le caracter: l'écrit en cas de caractère normal,
  ou fait la fonction du caratere special*/
  if(c < 127){
    switch(c){
      case 8:
        //BS \b
        col = col - 1;
        break;

      case 9:
        //HT \t
        //Tabulations: colognes multiples de 8
        //Prochaine multiple de 8 du valeur actuel de la cologne
        col = col + (8 - col%8);

        break;

      case 10:
        //LF \n 10
        lig = lig + 1;
        col = 0;
        break;

      case 12:
        //FF \f
        efface_ecran();
        lig = 1;
        col = 0;
        break;

      case 13:
        //CR \r
        col = 0;
        break;

      default:
        if(col < 79){
          ecrit_car(lig,col,c);
          col = col + 1;
        }else if(col == 79 && lig < 24){
          ecrit_car(lig,col,c);
          lig = lig + 1;
          col = 0;
        }else if(col == 79 && lig == 24){
          defilement();
          ecrit_car(lig,0,c);
          col = 1; //lig = 24;

        }
        break;
    }
  }
  if(lig == 25){
    defilement();
    lig = 24;
  }
  place_curseur(lig, col);

}

void console_putbytes(const char *s, int len) {

        (void)s;
        (void)len;

        int i;
        for(i=0; i<len; i++){
          traite_car(s[i]);
        }
}
