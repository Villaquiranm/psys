#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <stdint.h>
#include <queue.h>

#define SIZEPILE 512
#define NBPROC 30
#define MAXPRIO 256

extern void ctx_sw(uint32_t* pointeur1, uint32_t* pointeur2);
extern void ret_exit(void);

int start(int (*pt_func)(void*), const char *process_name, unsigned long ssize, int prio, void *arg);
int waitpid(int pid, int *retvalp);
void exitFunction(int retval);
int kill(int pid);
void context_switch(void);
void ordonnance(void);
int32_t mon_pid(void);
char *mon_nom(void);
void initProc(void);
int idle();
int proc1();
void proc2(void);
int proc3();
int getpid(void);
int getprio(int pid);
int chprio(int pid, int newprio);


#endif
