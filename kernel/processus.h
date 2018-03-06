#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <stdint.h>

#define SIZEPILE 512
#define NBPROC 30

extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2);

int numberprocessus;

enum reg_type{
	EBX,
	ESP,
	EBP,
	ESI,
	EDI
};

enum etats{
	ACTIF,
	ACTIVABLE,
	BLOQUE_SEMAPHORE,
	BLOQUE_IO,
	BLOQUE_FILS,
	ENDORMI,
	ZOMBIE
};

struct processus{
	int pid;
	char nom[10];
	int etat; //1 elu
	uint32_t regs[5];
	uint32_t pile[SIZEPILE];
	void(*func)(void);
	unsigned long reveille;
	int priorite;
};

struct processus *actif;
struct processus procs[NBPROC];
struct processus *lastProcessus;

int32_t cree_processus(void (*code)(void), char *nom);
void context_switch(void);
void ordonnance(void);
int32_t mon_pid(void);
char *mon_nom(void);
void initProc(void);
void idle(void);
void proc1(void);
void proc2(void);


#endif
