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
void traitant_IT_49();

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
/*
int chprio(int pid, int newprio);
void cons_write(const char *str, unsigned long size);
#if defined CONS_READ_LINE
unsigned long cons_read(char *string, unsigned long length);
#elif defined CONS_READ_CHAR
int cons_read(void);
#endif
void cons_echo(int on);
void exit(int retval);
int getpid(void);
int getprio(int pid);
int kill(int pid);
#if defined WITH_SEM
int scount(int sem);
int screate(short count);
int sdelete(int sem);
int signal(int sem);
int signaln(int sem, short count);
int sreset(int sem, short count);
int try_wait(int sem);
int wait(int sem);
#elif defined WITH_MSG
int pcount(int fid, int *count);
int pcreate(int count);
int pdelete(int fid);
int preceive(int fid,int *message);
int preset(int fid);
int psend(int fid, int message);
*/
#define PCREATE 0

void traitant_appels_systeme(uint32_t args[]){

    printf("traitant_appels_systeme\n");

    uint32_t numAppel = args[0];

    switch (numAppel) {
        case PCREATE: pcreate(args[2]);
            break;
    }

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

    if(num_IT==32){
        ms=0;
        heure=0;
        minute=0;
        second=0;

        outb(0x34,0x43);
        outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
        outb((uint8_t)(QUARTZ/CLOCKFREQ >> 8), 0x40);
    }

	masque_IRQ(0,false);
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
