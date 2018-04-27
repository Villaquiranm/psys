#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "console.h"
#include <processus.h>
#include "horloge.h"
#include "prueba.h"
#include "tests.h"
#include <mallocfreelist.h>

#define MALLOC_SIZE (4 * 1024 * 1024) //4MB

extern void * malloc(size_t n);

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{
	/*void * myblock = malloc(MALLOC_SIZE);

	int i;
	malloc_addblock(myblock, MALLOC_SIZE);
	//int *mal = fl_malloc(50);;
	efface_ecran();
//	call_debugger();

	int *b1 = fl_malloc(10);
	*b1 = (50);

	int *b2 = fl_malloc(10);
	*b2 = 10;

	int *b3 = fl_malloc(10);
	*b3 = 10;

	printf("B1 = %d\n", *b1);
	printf("B2 = %d\n", *b2);
	printf("B3 = %d\n", *b3);
	//fl_free(b3);
	//printf("FREE B3!!\n");
	//printf("NEW B3 = %d\n", *b3);
	fl_free(b1);
	printf("NEW B1 = %d\n", *b1);
	fl_free(b2);
	b2 = fl_malloc(10);
	*b2 = 30;
	printf("NEW B2 = %d\n", *b2);
	fl_free(b3);
	fl_free(b2);
	i = 10;
	printf("teste\n");
	printf("Factoriel de %d:",i);

	i = fact(i);
	printf("%d\n",i);*/

	unsigned long quartz;
	unsigned long ticks;

	//TODO VERIFIER CE QU'ON DOIT FAIRE AVEC LES POINTERS quartz ET ticks
	clock_settings(&quartz, &ticks);
	efface_ecran();
//	call_debugger();

	init_traitant_IT(32, traitant_IT_32);
	init_traitant_IT(49, traitant_IT_49);

	printf("teste\n");
	initProc();
  masque_IRQ(32,false);
  //initFile();

	execute_tests();

	//*mal = 10;

	//printf("Mal = %d", *mal);

	idle();
	//démasquer les interruptions externes
	//sti();

	return;
}
