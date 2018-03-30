#ifndef HORLOGE_H
#define HORLOGE_H
#include "stdbool.h"
#include "stdint.h"
#include "fileMessage.h"


#define CLOCKFREQ 100
#define QUARTZ 0x1234DD
#define SECOND_IN_TICKS CLOCKFREQ*1000000
PLINK sleeping_queue; 

void clock_settings(unsigned long *quartz, unsigned long *ticks);
unsigned long current_clock();
void wait_clock(unsigned long clock);
void printDroite(char* contenu);
void tic_PIT();
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));
void masque_IRQ(uint32_t num_IRQ, bool masque);
void traitant_IT_32();
int nbr_secondes();
unsigned long current_clock();
#endif
