#include "stdint.h"

#define TAILLE_PILE 1024
#define MAXIMUM_PROCESSUS 4

enum  etat_type{
	ELU,
	ACTIVABLE,
	BLOQUE_SEM,
	BLOQUE_IO,
	BLOQUE_FILS,
	ENDORMI,
	ZOMBIE
};

struct process{
	int pid;
	char nom[100];
	int etat;
	uint32_t registre[5]; //il s’agira tout simplement d’un tableau d’entiers,puisqu’on manipule des registres 32 bits, et vous avez besoin de 5 cases
	uint32_t pile[TAILLE_PILE]; //définie comme un tableau d’entiers d’une taille fixée par une constante
	void(*func)(void);
	int reveille;
	int priorite;
};
typedef struct process PROCESSUS;

//vai atribuir valores constantes em ordem
enum reg_type{
	EBX,	//0
	ESP,	//1
	EBP,	//2
	ESI,	//3
	EDI	//4
};

PROCESSUS* actif;

void initProcs();
void idle();
void proc1();
void proc2();
void proc3();
char* mon_mon();
int32_t mon_pid();
void ordonnance();
int32_t cree_processus(void (*code)(void), char *nom);
