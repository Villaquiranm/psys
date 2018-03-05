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

typedef struct regs{
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
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
	int etat; //1 elu
	int prio;
	regs regs;
	uint32_t *pile;
	link queueLink;
} processus;

struct processus *actif;
struct processus procs[NBPROC+1];
link procsPrioQueue = LIST_HEAD_INIT(procsPrioQueue);
struct processus *lastProcessus;
//--------------------------------------------------------

/*
 * Start primitive to create a new process
 */
int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg) {
	// Create a pointer to a new process structure with the
	// appropiate size
	processus *newProc = (processus*)malloc(sizeof(processus));

	// Allocate the required space for the execution stack plus the
	// function pointer, termination function pointer and the argument
  uint32_t *pile = malloc(ssize + 3);
  uint32_t *current = (pile + ssize + 3);

	// Set the process' fields with the appropiate values
	sprintf(newProc->nom, "%s", process_name);
	newProc->etat = ACTIVABLE;
	newProc->prio = prio;
	newProc->regs.esp = (uint32_t)current;
	newProc->pile = pile;

	// Put the function pointer, termination function pointer and the
	// argument on the top of the queue
  *(current--) = (uint32_t)pt_func;
  *(current--) = (uint32_t)NULL;
  *(current--) = (uint32_t)arg;

	// Add the process to the priority queue if there is enough
	// available space
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

/**
 * Function called by the system clock interruption or any event
 * that changes the priority of a process in order to succesfully
 * share the processor time
 */
void schedule(){
	// Put the active process in the priority queue so it has the
	// opportunity of being chosen again
	actif->etat = ACTIVABLE;
	queue_add(actif, &procsPrioQueue, processus, queueLink, prio);

	processus *prevProc = actif;

	// Simplistic election of the next process.
	// But what if its state is not ACTIVABLE?
	processus *nextProc = queue_out(&procsPrioQueue, processus, queueLink);
	nextProc->etat = ACTIF;
	actif = nextProc;
	ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
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

	//queue_add(idle, &procsPrioQueue, processus, queueLink, prio);

	start(proc1, "proc1", 512, 5, NULL);

	//actif = &procs[0];
	actif = idle;
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
			schedule();
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
			schedule();
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
