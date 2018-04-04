#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "console.h"
#include <processus.h>
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
	void * myblock = malloc(MALLOC_SIZE);

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
	printf("%d\n",i);

	printf("teste\n");
	initProc();

	//execute_tests();

	//*mal = 10;

	//printf("Mal = %d", *mal);

	idle();

	return;
}
