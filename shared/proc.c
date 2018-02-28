#include "proc.h"
#include "horloge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern PROCESSUS table_proc[MAXIMUM_PROCESSUS];

int nombre_proc = 0;

void ctx_sw(uint32_t* previous_proc, uint32_t* next_proc);

//faire une table définie statiquement par une constante
//elle a l'ED qui représent les processus activables
//pointeur sur le processus élu

/*void changementContexte(){


	//elle doit faire un round robin
	//sauvegarder les contexte du proc qui laisse
	//mettre le nouvel contexte
}*/

void initProcs(){

	//cree_processus(idle, "idle");
	
	table_proc[0].etat=ELU;
	table_proc[0].pid = 0;
	sprintf(table_proc[0].nom, "%s","idle");
	nombre_proc++;
	
	cree_processus(proc1, "proc1");
	cree_processus(proc2, "proc2");
	cree_processus(proc3, "proc3");

	actif = &table_proc[0];

	table_proc[0].etat=ELU;	

}

/*void idle(){
	ordonnance();
}

void proc1(){
	printf("olar");
}*/

int32_t mon_pid(){
	return (int32_t)(*actif).pid;
}

char* mon_nom(){
	return (char*)(*actif).nom;
}

/*void idle(){
	
	for(;;){
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		sti();
		hlt();
		cli();
	}

}

void proc1(){

	for(;;){
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		sti();
		hlt();
		cli();
	}
}

void proc2(){

	for(;;){
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		sti();
		hlt();
		cli();
	}
}

void proc3(){

	for(;;){
		printf("[%s] pid = %i\n", mon_nom(), mon_pid());
		sti();
		hlt();
		cli();
	}
}*/

void idle(){
	
	for(;;){
		ordonnance();
		sti();
		hlt();
		cli();
	}

}

void proc1(){
	for (;;){
		printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),mon_nom(), mon_pid());
		dors(2);
	}
}

void proc2(){
	for (;;){
		printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),mon_nom(), mon_pid());
		dors(3);
	}
}

void proc3(){
	for (;;){
		printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),mon_nom(), mon_pid());
		dors(5);
	}
}

void ordonnance(){

	while(1){
		if( (*actif).etat == ENDORMI ){	//dors

			if(nbr_secondes() >= (*actif).reveille ){
				(*actif).etat = ACTIVABLE;
				break;
			}
			else{
				if( (*actif).pid != (nombre_proc-1) ){
					actif++;
				}
				else{
					actif = &table_proc[0];
				}
			}
		}
		else{	//activable
			break;
		}
	}


	if( (*actif).etat != ENDORMI ){

		if( (*actif).pid != (nombre_proc-1) ){ //je ne suis pas le dernier processus

			PROCESSUS *old;
			PROCESSUS *new;

			old = actif;

			(*actif).etat = ACTIVABLE;
			(actif++);
			(*actif).etat = ELU;
		
			new = actif;

			ctx_sw((*old).registre, (*new).registre);

		}
		else{	//je suis le dernier processus

			PROCESSUS *old;

			old = actif;
			(*actif).etat = ACTIVABLE;
			table_proc[0].etat = ELU;

			actif = &table_proc[0];

			//endereço da função no inicio da pilha do processo
			//table_proc[0].pile[1023] = (uint32_t)idle;
			//endereço da pilha do processo na pilha de registrador
			//table_proc[0].registre[ESP] = (uint32_t)&table_proc[0].pile[1023];

			ctx_sw((*old).registre, table_proc[0].registre);
		}
	}
}

int32_t cree_processus(void (*code)(void), char *nom){

	int pid;

	nombre_proc++;

	if(nombre_proc > MAXIMUM_PROCESSUS){
		nombre_proc--;
		return -1;
	}
	else{
		pid = nombre_proc-1;
		table_proc[pid].pid = pid;
		sprintf(table_proc[pid].nom, "%s",nom);
		table_proc[pid].etat = ACTIVABLE;
		table_proc[pid].func = code;
		table_proc[pid].reveille = 0;

		//endereço da função no inicio da pilha do processo
		table_proc[pid].pile[1023] = (uint32_t)table_proc[pid].func;
		//endereço da pilha do processo na pilha de registrador
		table_proc[pid].registre[ESP] = (uint32_t)&table_proc[pid].pile[1023];

		return pid;
	}
}
	
/*

	ctx_sw(addresse anterieur de processus, addresse nouvel de processus)
	envoyer le pointeur de reg
	


    .text
    .globl ctx_sw
# Structure de la pile en entree :
#   %esp + 4 : adresse de l'ancien contexte
#   %esp + 8 : adresse du nouveau contexte
ctx_sw:
    # sauvegarde du contexte de l'ancien processus
    movl 4(%esp), %eax  //salva na memoria do processo os valores dos registradores físicos
    movl %ebx, (%eax)	//prendre 4 octets en commençant pour eax
    movl %esp, 4(%eax)  //prendre les prochaines 4 octets (eax+4) //ESP É UM REGISTRADOR QUE APONTA PRO TOPO DA PILHA, POIS PERDEMOS O PC 
    movl %ebp, 8(%eax)  //meme chose. tous ont 4 octets (32 bits. c'est le motive pour qu'on a mis 5 entier de 32 bits en registre)
    movl %esi, 12(%eax)
    movl %edi, 16(%eax)
    # restauration du contexte du nouveau processus
    movl 8(%esp), %eax  //coloca nos registradores os valoes que estao na memoria do processo
    movl (%eax), %ebx
    movl 4(%eax), %esp
    movl 8(%eax), %ebp
    movl 12(%eax), %esi
    movl 16(%eax), %edi
    # on passe la main au nouveau processus
    ret
*/
