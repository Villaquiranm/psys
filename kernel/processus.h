#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <stdint.h>
#include <queue.h>


#define SIZEPILE 512
#define NBPROC 30
#define MAXPRIO 256

//extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2, uint32_t* pagedir_addr);
extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2);
extern void cr3_sw(uint32_t* pagedir_addr, uint32_t* pile_kernel);
extern void ret_exit(void);
extern void exit(int retval);
extern void ret_user(int retval);
extern void kernel2user(uint32_t* pileuser, uint32_t* codeprocessus);
void return_user(void);

typedef struct regs{
	uint32_t ebx, esp, ebp, esi, edi;
} regs;

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
	enum etats state; //1 elu
	int prio;
	int retval;
	int expectedChild;
	regs regs;
	uint32_t *pile;
	uint32_t *pile_kernel;
	struct processus *parent, *children;
	struct processus *nextSibling;
	struct processus *dyingProcsLink;
	struct processus *nextSleepingProcs;
	link queueLink; // la structure link est définie dans le fichier queue.h, qui est une liste chaînée
    unsigned long sleep_time; // lors que le sleep_time est dépassé, il faut réveiller le processus endormi
	unsigned *pagedir;
} processus;

processus *active;

int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg);
int start2(const char *process_name, unsigned long ssize, int prio, void *arg);
int waitpid(int pid, int *retvalp);
void exitFunction(int retval);
int kill(int pid);
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
void sys_information();

/*TODO: Fonction suivante à créer qui prend en paramètre le code de
la fonction à exécuter et renvoie le pid du processus créé, ou -1 en cas d'erreur */

//int cree_processus(void (*code)(void));



#endif
