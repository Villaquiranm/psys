#include <processus.h>
#include <stdio.h>
#include <malloc.c>
#include <cpu.h>
#include <stdbool.h>
#include "fileMessage.h"
#include "horloge.h"
#include "mem.h"
#include <userspace_apps.h>
#include <hash.h>
#include "mallocfreelist.h"

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
const char *states[7];


hash_t apps_table;
#define PAGE_DIR_FLAGS     0x00000007u
extern unsigned pgtab[];
extern unsigned pgdir[];
extern unsigned testaddr;

/*static void fill_pgdir(unsigned pagedir[],
                                unsigned pagetab[],
                                unsigned count)
{
        unsigned int i;
        unsigned pgdir_entry;

        pgdir_entry = (unsigned)pagetab;

        for (i = 0; i < count; (i)++) {
                pagedir[i] = (pgdir_entry + (i) * 0x1000) | PAGE_DIR_FLAGS;
        }
        for (i = count; i < 1024; (i)++) {
                pagedir[i] = 0;
        }
}*/

static void copy_pgdir(unsigned pagedir[],
                       unsigned pagedir_kernel[]) {
    unsigned int i;
    for (i = 0; i < 64; (i)++) {
      pagedir[i] = pagedir_kernel[i];
    }
}

static void init_apps_table() {
  hash_init_string(&apps_table);

  int i = 0;
  while (symbols_table[i].name != NULL) {
    hash_set(&apps_table, (void *)symbols_table[i].name, (void *)&symbols_table[i]);
    i++;
  }
}

static void map_page(unsigned *pdir, unsigned *physaddr, unsigned virtualaddr, unsigned flags){
  unsigned pd_index = virtualaddr >> 22;
  unsigned pt_index = (virtualaddr >> 12) & 0x3FFu;

  // If the entry of the page directory is not yet filled, you need to
  // allocate the page table
  if (pdir[pd_index] == 0) {
    unsigned * new_ptable = memalign(4096, 1024);
    pdir[pd_index] = (unsigned)new_ptable | PAGE_DIR_FLAGS;
  }

  // Get page table
  unsigned *ptable = (unsigned*) (pdir[pd_index] & 0xFFFFF000);
  ptable[pt_index] = ((unsigned)physaddr & 0xFFFFF000) | flags;
}

void * alloc_pages(unsigned *pdir, unsigned virtualaddr, unsigned flags, unsigned size) {
  int nPages = size/4096;
  if (size % 4096 != 0) {
    nPages++;
  }
  void *debut_pages = pagealloc();
  map_page(pdir, (unsigned *) debut_pages, virtualaddr, flags);
  void *page;
  for(int i = 1; i < nPages; i++) {
    page = pagealloc();
    map_page(pdir, (unsigned *) page, virtualaddr + i*0x1000u, flags);
  }
  return debut_pages;
}

void map_app(unsigned *pdir, unsigned virtualaddr, unsigned flags, struct uapps *app) {
  int size = (int)app->end - (int)app->start + 4;
  int nPages = size/4096;

  void *space_app;
  for(int i = 0; i < nPages; i++) {
    space_app = pagealloc();
    map_page(pdir, (unsigned *) space_app, virtualaddr + i*0x1000u, flags);
    MALLOC_COPY(space_app, app->start + i*0x1000u, 4096);
  }

  //Dernière page
  if(size%4096 != 0) {
    space_app = pagealloc();
    map_page(pdir, (unsigned *) space_app, virtualaddr + nPages*0x1000u, flags);
    MALLOC_COPY(space_app, app->start + nPages*0x1000u, size%4096);
  }


}

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
	//ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx, nextProc->pagedir);
    uint32_t *pgdir_addr = (uint32_t *)(nextProc->pagedir);
    uint32_t *pile_kernel = (uint32_t *)(nextProc->pile_kernel + 4096/4 - 1);
    cr3_sw(pgdir_addr, pile_kernel);
    ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
}

/*
 * Start primitive to create a new process
 */
int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg) {
	// Create a pointer to a new process structure with the
	// appropiate size
	processus *newProc = (processus*)mem_alloc(sizeof(processus));
    newProc->pagedir = memalign(4096, 4096);
	// Fill page directory for the first 256MB of memory fill_pgdir(newProc->pagedir, pgtab, 64);
	copy_pgdir(newProc->pagedir, pgdir);
    ssize = ssize + 1;
	// Allocate the required space for the execution stack plus the
	// function pointer, termination function pointer and the argument
    //uint32_t *pile = (uint32_t *)fl_malloc(4096);
    uint32_t *pile = (uint32_t *)pagealloc();


    uint32_t *current = (pile + (4096)/4) - 1;

	// Put the function pointer, termination function pointer and the
	// argument on the top of the queue
	*(current--) = (uint32_t)arg;
	//*(current--) = (uint32_t)exitFunction;
	*(current--) = (uint32_t)exit;
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

int start2(const char *process_name, unsigned long ssize, int prio, void *arg) {
  if (hash_isset(&apps_table, (void *)process_name) != 0) {
    struct uapps *current_app = (struct uapps *)(hash_get(&apps_table, (void *)process_name, 0));

    // Create a pointer to a new process structure with the
    // appropiate size
    processus *newProc = (processus*)mem_alloc(sizeof(processus));
    newProc->pagedir = memalign(4096, 4096);

    // int app_size = (int)current_app->end - (int)current_app->start + 4;

    // Does this block has to be page-aligned?
    // unsigned *space_app = (unsigned *)memalign(4096,app_size);
    // unsigned *space_app = (unsigned *)pagealloc();
    // unsigned *pagedeux;
    // if(app_size % 4096 > 0) {
    //  pagedeux = (unsigned *)pagealloc();
    // }
    //map_page(newProc->pagedir, space_app, 0x40000000, PAGE_DIR_FLAGS);
    //map_page(newProc->pagedir, pagedeux, 0x40001000, PAGE_DIR_FLAGS);
    //MALLOC_COPY(space_app, current_app->start, app_size);

    // Fill page directory for the first 256MB of memory
    copy_pgdir(newProc->pagedir, pgdir);
    ssize = ssize + 1;

    newProc->pile_kernel = (uint32_t *) mem_alloc(4096);
    uint32_t *pile_kernel = (newProc->pile_kernel + 4096/4) - 1;

    map_app(newProc->pagedir, 0x40000000, PAGE_DIR_FLAGS, current_app);

    // Allocate the required space for the execution stack plus the
    // function pointer, termination function pointer and the argument
    //uint32_t *pile = (uint32_t *)memalign(4096,ssize);
    //uint32_t *pile = (uint32_t *)pagealloc();
    //map_page(newProc->pagedir, pile, 0x80000000, PAGE_DIR_FLAGS);
    uint32_t *pile = (uint32_t *) alloc_pages(newProc->pagedir,(unsigned) 0x80000000, PAGE_DIR_FLAGS, 4096);
    //uint32_t *current = (pile + (ssize)/4) - 1;
    uint32_t *current = (pile + 0x1000/4) - 1;
    uint32_t *return_function = pagealloc();
    map_page(newProc->pagedir, return_function, 0xFFFFF000, PAGE_DIR_FLAGS);
    MALLOC_COPY(return_function, ret_user, 28);
    // Put the function pointer, termination function pointer and the
    // argument on the top of the queue
    *(current--) = (uint32_t)arg;
    *(current) = (uint32_t)0xFFFFF000;
    //*(current) = (uint32_t)0x40000000;

    /*
    *(pile_kernel--) = (uint32_t)arg;
    *(pile_kernel--) = (uint32_t)ret_exit;
    *(pile_kernel) = (uint32_t)0x40000000;
    */


    *(pile_kernel--) = (uint32_t)0x40000000;
    //*(pile_kernel--) = (uint32_t)space_app;
    //*(pile_kernel--) = *(current);
    *(pile_kernel--) = (uint32_t)(0x80000000 + 0x1000) - 8;
    *(pile_kernel) = (uint32_t)kernel2user;

    // Set the process' fields with the appropiate values
    sprintf(newProc->nom, "%s", process_name);
    newProc->state = ACTIVABLE;
    newProc->prio = prio;
    newProc->regs.esp = (uint32_t)pile_kernel;
    //newProc->regs.esp = (uint32_t) current;
    newProc->pile = pile;
    //newProc->pile = newProc->pile_kernel;
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

    // Add the process to the priority queue if there is enough available space
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
  } else {
    printf("La fonction %s n'a pas été trouvée\n", process_name);
    return -1;
  }
}

int kill(int pid) {

	if (procs[pid] == NULL || pid == active->pid || procs[pid]->state==ZOMBIE || pid == 0)
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
		if (ptr->sleep_time < current_time) {//delete processus from Sleeping_queue
			ptr->state = ACTIVABLE;
			queue_add(ptr, &procsPrioQueue, processus, queueLink, prio);
			if (ptr == sleepingProcs) {//If the first process We need to move the head.
				sleepingProcs = ptr->nextSleepingProcs;
                ptr->nextSleepingProcs = NULL; // break link
                ptr = sleepingProcs;
                previous_ptr = ptr;
			}
			else{
				previous_ptr->nextSleepingProcs = ptr->nextSleepingProcs;
                ptr->nextSleepingProcs = NULL;
                ptr = previous_ptr->nextSleepingProcs;
			}
		}else{//Not deleting processus
        previous_ptr = ptr;
    		ptr = ptr->nextSleepingProcs;
        }

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
	//ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx, nextProc->pagedir);
    uint32_t *pgdir_addr = (uint32_t *)(nextProc->pagedir);
    uint32_t *pile_kernel = (uint32_t *)(nextProc->pile_kernel + 4096/4 - 1);
    cr3_sw(pgdir_addr, pile_kernel);
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
    uint32_t *pgdir_addr = (uint32_t *)(nextProc->pagedir);
    uint32_t *pile_kernel = (uint32_t *)(nextProc->pile_kernel + 4096/4 - 1);
    cr3_sw(pgdir_addr, pile_kernel);
    ctx_sw(&prevProc->regs.ebx, &nextProc->regs.ebx);
	}
}


void initProc(void){
	for (size_t i = 0; i < NBPROC + 1; i++) {
		procs[i] = NULL;
	}
  init_apps_table();

	processus *idle = (processus*)mem_alloc(sizeof(processus));


	idle->pid = 0;
	idle->state = ACTIF;
	idle->prio = 1;
    idle->pagedir = (unsigned *)0x101000;
	sprintf(idle->nom, "idle");
	active = idle;
  procs[0] = idle;
	processus *sentinel = (processus*)mem_alloc(sizeof(processus));
	sentinel->dyingProcsLink = NULL;
	dyingProcessesQueue = sentinel;
	//start(proc3, "proc3", 512, 10, NULL);
  states[0] ="ACTIF";
  states[1] = "ACTIVABLE";
  states[2] = "BLOQUE_SEMAPHORE";
  states[3] = "BLOQUE_IO";
  states[4] = "BLOQUE_FILS";
  states[5] = "ENDORMI";
  states[6] = "ZOMBIE";
}

int32_t mon_pid(void){
	return active->pid;
}

char *mon_nom(void){
	return active->nom;
}

int idle(){
	while(1){
		//printf("IDLE\n");
		sti();
		for(int i = 0; i < 5000000; i++){
			//schedule();
		}
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
	if (procs[pid] == NULL || (newprio < 1 || newprio > MAXPRIO) || procs[pid]->state==ZOMBIE)
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
	fl_free(procs[pid]->pile);
	mem_free(procs[pid], sizeof(processus));
	/* After freeing the procs array position it has to be set to NULL papapa */
	procs[pid] = NULL;
}
void sys_info(){
  int i = 0;
  while (i <= NBPROC) {
    if (procs[i] != NULL) {
      printf("[%d]\t%-20s\t\t\t%s\n",procs[i]->pid,procs[i]->nom,states[procs[i]->state]);
    }
    i++;
  }
}
