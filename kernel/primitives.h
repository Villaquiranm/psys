#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

struct processusPrimitif{
	int pid;
	char nom[10];
	int etat; //1 elu
	int prio;
	void *pile;
};

int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg);

#endif
