#include <string.h>
#include <stdio.h>
#include "horloge.h"
#include "proc.h"
#include "stdint.h"
#include "segment.h"
#include "cpu.h"

#define TABLE_VECTEURS 0x1000
#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

int ms, second, minute, heure;
int totalSeconds;

void traitant_IT_32();

/*void printDroite(char* contenu){

	char* aux;
	int sizeContenu=0;
	int i;

	aux=contenu;
	while((*aux)!='\0'){
		sizeContenu++;
		aux++;
	}

	aux=contenu;
	for(i=0; i<sizeContenu; i++){
		ecrit_car(0,(80-sizeContenu+i),(*aux),0,0,15);
		aux++;
	}
}*/

void tic_PIT(){

	outb(0x20, 0x20);

	char horaire[9];

	ms+=(1000/CLOCKFREQ);

	if(ms==1000){
		second++;
		totalSeconds++;
		ms=0;

		if(second==60){
			minute++;
			second=0;

			if(minute==60){
				heure++;
				minute=0;
			}
		}

		sprintf(horaire,"%02d:%02d:%02d",heure,minute,second);
		//printDroite(horaire);
	}

	ordonnance();
}

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){

	uint32_t traitantFaible;
	uint32_t traitantFort;
	uint32_t *adresseN = (uint32_t*)(TABLE_VECTEURS + 2*4*num_IT);

	/*uint32_t premiereMot;
	uint32_t deuxiemeMot;*/

	traitantFaible = (uint32_t)traitant;
	traitantFaible <<= 16;
	traitantFaible >>= 16;

	traitantFort = (uint32_t)traitant;
	traitantFort >>= 16;
	traitantFort <<= 16;

	/*premiereMot = (uint32_t)KERNEL_CS;
	premiereMot <<= 16;
	premiereMot = premiereMot | traitantFaible;
	*adresseN = premiereMot;

	adresseN++;

	deuxiemeMot = traitantFort;
	deuxiemeMot = deuxiemeMot | (uint32_t)0x8E00;

	*adresseN = deuxiemeMot;*/

	*adresseN = (uint32_t)KERNEL_CS;
	*adresseN <<= 16;
	*adresseN = *adresseN | traitantFaible;

	adresseN++;

	*adresseN = traitantFort;
	*adresseN = *adresseN | (uint32_t)0x8E00;

	ms=0;
	heure=0;
	minute=0;
	second=0;

	outb(0x34,0x43);
	outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
	outb((uint8_t)(QUARTZ/CLOCKFREQ >> 8), 0x40);

	masque_IRQ(0,false);
}

void masque_IRQ(uint32_t num_IRQ, bool masque){

	uint8_t tableBool;
	int masqueShift = masque;
	int masqueOne = 1;

	tableBool = inb(0x21);

	if(masque==true){
		masqueShift <<= num_IRQ;
		tableBool = tableBool | masqueShift;
	}
	else{
		masqueOne <<= num_IRQ;
		masqueOne = ~masqueOne;
		tableBool = tableBool & masqueOne;
	}

	outb(tableBool, 0x21);
}

void dors(uint32_t nbr_secs){
	(*actif).reveille=totalSeconds+nbr_secs;
	(*actif).etat=ENDORMI;
	ordonnance();
}

int nbr_secondes(){
	return totalSeconds;
}
