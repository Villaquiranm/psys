#include <stdint.h>
#include <stdio.h>
#include <malloc.c>
#include <processus.h>

// These variables definitions were originally in the header file
// but as the start.c file also includes that header, the compiler
// was complaining (double definitions). For the moment they're not
// used outside but it is necessary to address that problem if they
// are to be used in another file
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
//--------------------------------------------------------

int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg) {
  processus *newProc = (processus*)malloc(sizeof(processus));
	//link linkProc = LIST_HEAD_INIT(linkProc);

  void *pile = malloc(ssize+3);
  void *current = pile+ssize+3;

  current = pt_func;
	current--;
  current = NULL;
	current--;
  current = arg;
	current--;

	newProc->prio = prio;
	//newProc->queueLink = linkProc;

	sprintf(newProc->nom, "%s", process_name);

	newProc->etat = ACTIVABLE;
	if(freePID == 0) {
		return -1;
	} else {
		newProc->pid = nextPID;
		queue_add(newProc, &procsPrioQueue, processus, queueLink, prio);
    nextPID++;
		freePID--;
		return newProc->pid;
	}


}

/**
 * Test function to dequeue and print all processes
 */
void dequeue_all_processes(void){
	processus *nextProc;
	while(0 == queue_empty(&procsPrioQueue)){
		nextProc = queue_out(&procsPrioQueue, processus, queueLink);
		printf("%s : %d\n", nextProc->nom, nextProc->prio);
	}
}

void context_switch(void){
	//Change context and call the scheduler here in the future
}

void initProc(void){
	processus *idle = (processus*)malloc(sizeof(processus));

	idle->pid = 0;
	idle->etat = ACTIF;
	idle->prio = 1;
	sprintf(idle->nom, "idle");

	queue_add(idle, &procsPrioQueue, processus, queueLink, prio);

	start(proc1, "proc1", 512, 5, NULL);

	actif = &procs[0];
	dequeue_all_processes(); // Testing queue
}

int32_t mon_pid(void){
	return actif->pid;
}

char *mon_nom(void){
	return actif->nom;
}

int idle(){
	unsigned long i;
	while(1){
		printf("IDLE\n");
		for(i = 0; i < 5000000; i++){
			//ctx_sw(procs[0].regs, procs[1].regs); //change to context_switch when done
		}
	}
	return 0;
}

int proc1(){
	unsigned long i;
	while(1){
		printf("A\n");
		for(i = 0; i < 5000000; i++){
			//ctx_sw(procs[1].regs, procs[0].regs); //change to context_switch when done
		}
	}
	return 1;
}

void proc2(void){
	unsigned long i;
	while(1){
		printf("B");
		for(i = 0; i < 5000000; i++){
			//ctx_sw();
		}
	}
}

int getpid(void){
	return actif->pid;
}

int getprio(int pid){
	return actif->prio;
  return pid; //Temporary trivial statement to avoid compiler warning
}

int chprio(int pid, int newprio){
	//if prio and pid are valid
	//int oldprio = processus[pid].prio;
	//processus[pid].prio = newprio;
	//Reorganize the priority queue
	return 0;
  return pid + newprio; //Temporary trivial statement to avoid compiler warning
}
