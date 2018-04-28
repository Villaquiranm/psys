#include <string.h>
#include <stdio.h>
#include "horloge.h"
#include "processus.h"
#include "stdint.h"
#include "segment.h"
#include "fileMessage.h"
#include "cpu.h"
#include "queue.h"
#include "mem.h"

#define TABLE_VECTEURS 0x1000

int ms, second, minute, heure;
//int totalSeconds;
//PLINK sleeping_queue;
extern struct processus* active; //TODO: à vérifier, éventuellement erreur
extern struct processus* sleepingProcs;


void traitant_IT_32();

unsigned long numberInterruptions;

//renvoie la fréquence du quartz et le nombre d'oscillations entre chaque interruption
void clock_settings(unsigned long *quartz, unsigned long *ticks){
  *quartz = (unsigned long) QUARTZ;
  //TODO vérifier si on doit faire %256
  *ticks = (unsigned long) QUARTZ/ (unsigned long) CLOCKFREQ;
}

//Retourne le nombre d'interruptions d'horloge depuis le démarrage du noyau.
unsigned long current_clock(){
    return numberInterruptions;
}

//Passe le processus dans l'état endormi jusqu'à ce que le nombre d'interruptions horloge passé en paramètre soit atteint ou dépassé
void wait_clock(unsigned long clock){
    // we suppose that sleeping_queue is initialized
    active->state = ENDORMI;
    active->sleep_time = current_clock() + clock; // On affecte l'heure où le processus dois être réveillé
    if (sleepingProcs == NULL) { // On vérifie que si la liste des processus endormi sont vide ou pas
      sleepingProcs = active; //Si la liste est vide, on met directement le processus actif dans la liste
    }else{
      struct processus* ptr = sleepingProcs;
      while (ptr->nextSleepingProcs != NULL) { // On vérifie si le prochain processus endormi est null,
        ptr = ptr->nextSleepingProcs; // sinon on rentre dans la boucle, jusqu'à trouver la queue
      }
      ptr->nextSleepingProcs = active;
      active->nextSleepingProcs = NULL;
    }
    schedule();
}

/*l'acquittement de l'interruption et la partie gérant l'affichage*/
void tic_PIT(){

	outb(0x20, 0x20);
    numberInterruptions++;
	schedule();
}

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){

	uint32_t traitantFaible;
	uint32_t traitantFort;
	uint32_t *adresseN = (uint32_t*)(TABLE_VECTEURS + 2*4*num_IT);

	traitantFaible = (uint32_t)traitant;
	traitantFaible <<= 16;
	traitantFaible >>= 16;

	traitantFort = (uint32_t)traitant;
	traitantFort >>= 16;
	traitantFort <<= 16;

	*adresseN = (uint32_t)KERNEL_CS;
	*adresseN <<= 16;
	*adresseN = *adresseN | traitantFaible;

	adresseN++;

	*adresseN = traitantFort;
	*adresseN = *adresseN | (uint32_t)0x8E00;

	ms=0;
	heure=0;
	minute=0;
	second=0;

	outb(0x34,0x43);
	outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
	outb((uint8_t)(QUARTZ/CLOCKFREQ >> 8), 0x40);

	masque_IRQ(0,false);
  masque_IRQ(1,false);
}

void masque_IRQ(uint32_t num_IRQ, bool masque){

	uint8_t tableBool;
	int masqueShift = masque;
	int masqueOne = 1;

	tableBool = inb(0x21);

	if(masque==true){
		masqueShift <<= num_IRQ;
		tableBool = tableBool | masqueShift;
	}
	else{
		masqueOne <<= num_IRQ;
		masqueOne = ~masqueOne;
		tableBool = tableBool & masqueOne;
	}

	outb(tableBool, 0x21);
}

//int nbr_secondes(){
//	return totalSeconds;
//}
unsigned long getNumberInterruptions(){
    return numberInterruptions;
}
