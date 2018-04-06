#include <processus.h>
#include <stdio.h>
#include <malloc.c>
#include <cpu.h>
#include <stdbool.h>
#include "fileMessage.h"
#include "horloge.h"
#include "mem.h"

// These variables definitions were originally in the header file
// but as the start.c file also includes that header, the compiler
// was complaining (double definitions). For the moment they're not
// used outside but it is necessary to address that problem if they
// are to be used in another file
int nextPID = 1;
int freePID = NBPROC;

processus *procs[NBPROC + 1];
link procsPrioQueue = LIST_HEAD_INIT(procsPrioQueue);
//link dyingProcessesQueue = LIST_HEAD_INIT(dyingProcessesQueue);
processus *dyingProcessesQueue;
processus *sleepingProcs;

/*
 * Primitive to properly finish a process
 */
void exitFunction(int retval){

	// Add the currently active process to the dying queue
	zombifyProc(active->pid);
	active->retval = retval;

	if (active->parent != NULL && active->parent->state == BLOQUE_FILS &&
		  (active->parent->expectedChild == active->pid || active->parent->expectedChild < 0)) {
		active->parent->state = ACTIVABLE;
		queue_add(active->parent, &procsPrioQueue, processus, queueLink, prio);
	}

	// Perhaps oversimplified election of the next process
	processus *prevProc = active;
	processus *nextProc = queue_out(&procsPrioQueue, processus, queueLink);
	nextProc->state = ACTIF;
	active = nextProc;
	ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
}

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

	// Put the function pointer, termination function pointer and the
	// argument on the top of the queue
	*(current--) = (uint32_t)arg;
	//*(current--) = (uint32_t)exitFunction;
	*(current--) = (uint32_t)ret_exit;
  *(current) = (uint32_t)pt_func;

	// Set the process' fields with the appropiate values
	sprintf(newProc->nom, "%s", process_name);
	newProc->state = ACTIVABLE;
	newProc->prio = prio;
	newProc->regs.esp = (uint32_t)current;
	newProc->pile = pile;
	newProc->dyingProcsLink = NULL;
	newProc->nextSleepingProcs = NULL;
	newProc->expectedChild = 0;

	if (active->pid == 0) {	// IDLE process is active
		newProc->parent = NULL;
		newProc->children = NULL;
		newProc->nextSibling = NULL;
	} else {
		newProc->parent = active;
		newProc->children = NULL;
		if (newProc->parent->children != NULL) {
			newProc->nextSibling = newProc->parent->children;
		}
		newProc->parent->children = newProc;
	}


	// Add the process to the priority queue if there is enough
	// available space
	if(freePID == 0) {
		return -1;
	} else {
		procs[nextPID] = newProc;
		newProc->pid = nextPID;
    nextPID++;
		freePID--;

		if (newProc->prio > active->prio) {
			schedulePID(newProc->pid);
		} else {
			queue_add(newProc, &procsPrioQueue, processus, queueLink, prio);
		}

		return newProc->pid;
	}
}

int kill(int pid) {

	if (procs[pid] == NULL || pid == active->pid)
	/* Invalid pid */
		return -1;


	processus *killedProc = procs[pid];

	if (killedProc->state == ACTIVABLE) {
		queue_del(killedProc, queueLink);
		killedProc->state = ZOMBIE;
	}
	killedProc->retval = 0;

	if (killedProc->parent != NULL && killedProc->parent->state == BLOQUE_FILS &&
		  (killedProc->parent->expectedChild == killedProc->pid || killedProc->parent->expectedChild < 0)) {
		killedProc->parent->state = ACTIVABLE;
		queue_add(killedProc->parent, &procsPrioQueue, processus, queueLink, prio);
	}

	zombifyProc(killedProc->pid);

	return 0;
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

void preparingContextSwitch(){
	if (active->state == ACTIF) {
		// Put the active process in the priority queue so it has the
		// opportunity of being chosen again
		active->state = ACTIVABLE;
		queue_add(active, &procsPrioQueue, processus, queueLink, prio);
	}

	// Properly killing all the processes in the dying queue
	processus *currentProc, *prevProc;
	/* prevProc is the sentinel */
	prevProc = dyingProcessesQueue;
	currentProc = prevProc->dyingProcsLink;
	while(currentProc != NULL){;
		/* We can't kill a process(ess) if his/her parent is in a wait method */
		if (currentProc->parent == NULL) {
			prevProc->dyingProcsLink = currentProc->dyingProcsLink;
			freeProcessus(currentProc->pid);
		}else{
			prevProc = currentProc;
		}
		currentProc = prevProc->dyingProcsLink;
	}
    //Wake up sleeping processus
    unsigned long current_time = current_clock();

		//Reveille tous les processus qu'il faut et les ajoute dans la file de priorite
		struct processus* ptr = sleepingProcs;
		struct processus* previous_ptr = sleepingProcs;
		while (ptr != NULL) {
			if (ptr->sleep_time < current_time) {
				ptr->state = ACTIVABLE;
				queue_add(ptr, &procsPrioQueue, processus, queueLink, prio);
				if (ptr == sleepingProcs) {//If the first process We need to move the head.
					sleepingProcs = ptr->nextSleepingProcs;
				}
				else{
					previous_ptr->nextSleepingProcs = ptr->nextSleepingProcs;
				}
			}
			previous_ptr = ptr;
			ptr = ptr->nextSleepingProcs;
		}
}

/**
 * Function called by the system clock interruption or any event
 * that changes the priority of a process in order to succesfully
 * share the processor time
 */
void schedule(){
	preparingContextSwitch();

	// Perhaps oversimplified election of the next process.
	// But what if its state is not ACTIVABLE?
	processus *prevProc = active;
	processus *nextProc = queue_out(&procsPrioQueue, processus, queueLink);
	nextProc->state = ACTIF;
	active = nextProc;
	ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
}

/**
 * Function to activate a specific process
 */
void schedulePID(int pid){
	if (procs[pid] != NULL) {	// Not supposed to happen but checking anyway
		preparingContextSwitch();

		processus *prevProc = active;
		processus *nextProc = procs[pid];
		nextProc->state = ACTIF;
		active = nextProc;
		ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
	}
}


void initProc(void){
	for (size_t i = 0; i < NBPROC + 1; i++) {
		procs[i] = NULL;
	}

	processus *idle = (processus*)malloc(sizeof(processus));

	idle->pid = 0;
	idle->state = ACTIF;
	idle->prio = 1;
	sprintf(idle->nom, "idle");
	active = idle;

	processus *sentinel = (processus*)malloc(sizeof(processus));
	sentinel->dyingProcsLink = NULL;
	dyingProcessesQueue = sentinel;

	//start(proc1, "proc1", 512, 5, NULL);
	//start(proc3, "proc3", 512, 10, NULL);
}

int32_t mon_pid(void){
	return active->pid;
}

char *mon_nom(void){
	return active->nom;
}

int idle(){
	while(1){
		sti();
        hlt();
        cli();
	}
	return 0;
}

int proc1(){
	unsigned long i;
	while(1){
		printf("A\n");
		for(i = 0; i < 5000000; i++){
			schedule();
		}
	}
	return 1;
}

void proc2(void){
	unsigned long i;
	while(1){
		printf("B");
		sti();
		for(i = 0; i < 5000000; i++){
		}
	}
}

int proc3(){
	printf("C\n");
	return 1;
}

int getpid(void){
	return active->pid;
}

int getprio(int pid){
	if (procs[pid] == NULL)
		return -1;

	return procs[pid]->prio;
}

int chprio(int pid, int newprio){
	if (procs[pid] == NULL || (newprio < 1 && newprio > MAXPRIO))
		return -1;

	int prevPrio = procs[pid]->prio;
	procs[pid]->prio = newprio;
	if (procs[pid]->state == ACTIVABLE) {
		queue_del(procs[pid], queueLink);
		queue_add(procs[pid], &procsPrioQueue, processus, queueLink, prio);
	}
	schedule();
	return prevPrio;
}

int waitpid(int pid, int *retvalp) {
	/* This function returns pid if one valid process is found
	 * -1 if the pid is invalid
	 */
	bool end = false;
	processus *nextChild = active->children;
	active->expectedChild = pid;

	/* Loop to wait a child proccess to be ended */
	while(!end){
		/* If pid < 0 we must wait until any child ends */
		if (pid < 0){
			nextChild = active->children;
			while(nextChild != NULL){
				if(nextChild->state == ZOMBIE){
					pid = nextChild->pid;

					if (retvalp != 0) {
						*retvalp = procs[pid]->retval;
					}

					end = true;
				} else {
					nextChild = nextChild->nextSibling;
				}
			}
		}	else {
			/* Case where we search for a particular pid;
			 * if the pid is not valid, we return a negative number
			 */
			if(procs[pid] == NULL || procs[pid]->parent->pid != active->pid){
				return -1;
			} else if(procs[pid]->state == ZOMBIE){
				if (retvalp != 0) {
					*retvalp = procs[pid]->retval;
				}
				end = true;
			}
		}

		if(!end){
			active->state = BLOQUE_FILS;
			schedule();
		}

	}
	return pid;


}

void zombifyProc(int pid){
	if(procs[pid]->state != ZOMBIE){
		procs[pid]->state = ZOMBIE;
		procs[pid]->dyingProcsLink = dyingProcessesQueue->dyingProcsLink;
		dyingProcessesQueue->dyingProcsLink = procs[pid];
	}
}

void freeProcessus(int pid){
	free(procs[pid]->pile);
	free(procs[pid]);
	/* After freeing the procs array position it has to be set to NULL papapa */
	procs[pid] = NULL;
}
