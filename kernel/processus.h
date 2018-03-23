#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <stdint.h>
#include <queue.h>

#define SIZEPILE 512
#define NBPROC 30
#define MAXPRIO 256

extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2);
extern void ret_exit(void);

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
	int etat;
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
int idle();
int proc1();
void proc2(void);
int proc3();
int getpid(void);
int getprio(int pid);
int chprio(int pid, int newprio);
void freeProcessus(int pid);
void zombifyProc(int pid);
void schedulePID(int pid);
void schedule();


#endif
