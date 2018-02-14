#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "console.h"

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{
	int i;
	efface_ecran();
//	call_debugger();

	i = 10;
	printf("teste\n");
	printf("Factoriel de %d:",i);

	i = fact(i);
	printf("%d\n",i);

	while(1)
	  hlt();

	return;
}
