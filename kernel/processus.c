#include <stdint.h>
#include <stdio.h>
#include <processus.h>
#include <cpu.h>

int32_t cree_processus(void (*code)(void), char *nom){
	struct processus newProc;
	int i = numberprocessus;

	if(i < NBPROC){
		newProc.pid = i;
		sprintf(newProc.nom, "%s", nom);
		newProc.etat = ACTIVABLE;
		newProc.pile[SIZEPILE-1] = (uint32_t) code;
		newProc.reveille = 0;

		procs[i] = newProc;

		procs[i].regs[ESP] = (uint32_t) &procs[i].pile[SIZEPILE-1];


		numberprocessus ++;
		return procs[i].pid;
	} else {
		return -1;
	}
}

void context_switch(void){
	//Change context and call the scheduler here in the future
}

void initProc(void){

	numberprocessus = 0;

	procs[numberprocessus].pid = numberprocessus;
	procs[numberprocessus].etat = ACTIF;
	sprintf(procs[numberprocessus].nom, "idle");

	numberprocessus += 1;

	cree_processus(proc1, "proc1");
	cree_processus(proc2, "proc2");

	actif = &procs[0];


}

int32_t mon_pid(void){
	return actif->pid;
}

char *mon_nom(void){
	return actif->nom;
}

void idle(void){
	unsigned long i;
	while(1){
		printf("IDLE\n");
		sti();
		for(i = 0; i < 50000000; i++);
		cli();
	}
}

void proc1(void){
	unsigned long i;
	while(1){
		printf("A\n");
		sti();
		for(i = 0; i < 50000000; i++);
		cli();
	}
}

void proc2(void){
	unsigned long i;
	while(1){
		printf("B");
		sti();
		for(i = 0; i < 5000000; i++){
			cli();
		}
	}
}
void ordonnance(){
		if(actif->pid == 0){
			actif = &procs[1];
			ctx_sw(procs[0].regs, procs[1].regs);
		}
		else{
			actif = &procs[0];
			ctx_sw(procs[1].regs, procs[0].regs);
		}
}
