#include "stdbool.h"
#include "stdint.h"

void printDroite(char* contenu);
void tic_PIT();
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));
void masque_IRQ(uint32_t num_IRQ, bool masque);
void traitant_IT_32();
void dors(uint32_t nbr_secs);
int nbr_secondes();
