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
extern void * memalign(size_t alignment, size_t n);

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{
	//Creates the block of the memory between 64Mo and 256Mo
	malloc_addblock((void *)0x4000000, 0xBFFFFFC);

	unsigned long quartz;
	unsigned long ticks;

	//TODO VERIFIER CE QU'ON DOIT FAIRE AVEC LES POINTERS quartz ET ticks
	clock_settings(&quartz, &ticks);
	efface_ecran();
//	call_debugger();

	init_traitant_IT(32, traitant_IT_32);

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
