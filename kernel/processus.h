#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <stdint.h>
#include <queue.h>

#define SIZEPILE 512
#define NBPROC 30
#define MAXPRIO 256

extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2);

int nextPID = 1;
int freePID = NBPROC;

enum reg_type{
	EBX = 0,
	ESP = 1,
	EBP = 2,
	ESI = 3,
	EDI = 4
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

typedef struct processus{
	int pid;
	char nom[10];
	int etat; //1 elu
	int prio;
	void *pile;
	link queueLink;
} processus;

struct processus *actif;
struct processus procs[NBPROC+1];
link procsPrioQueue = LIST_HEAD_INIT(procsPrioQueue);
struct processus *lastProcessus;

int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg);
void context_switch(void);
void ordonnance(void);
int32_t mon_pid(void);
char *mon_nom(void);
void initProc(void);
int idle();
int proc1();
void proc2(void);
int getpid(void);
int getprio(int pid);
int chprio(int pid, int newprio);


#endif
