#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "console.h"
#include <processus.h>
#include "horloge.h"
#include "tests.h"
#include "pilote.h"

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{

	unsigned long quartz;
	unsigned long ticks;

	//TODO VERIFIER CE QU'ON DOIT FAIRE AVEC LES POINTERS quartz ET ticks
	clock_settings(&quartz, &ticks);
	efface_ecran();
	kdb_leds(0);
//	call_debugger();

	init_traitant_IT(32, traitant_IT_32);
	init_traitant_IT(33, traitant_IT_33);
	//cons_echo(false);
	printf("teste\n");
	initProc();
	//execute_tests();
	idle();


	return;
}
