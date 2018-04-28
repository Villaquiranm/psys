
/*
int chprio(int pid, int newprio);
void cons_write(const char *str, unsigned long size);
#if defined CONS_READ_LINE
unsigned long cons_read(char *string, unsigned long length);
#elif defined CONS_READ_CHAR
int cons_read(void);
#endif
void cons_echo(int on);
void exit(int retval);
int getpid(void);
int getprio(int pid);
int kill(int pid);
#if defined WITH_SEM
int scount(int sem);
int screate(short count);
int sdelete(int sem);
int signal(int sem);
int signaln(int sem, short count);
int sreset(int sem, short count);
int try_wait(int sem);
int wait(int sem);
#elif defined WITH_MSG
int pcount(int fid, int *count);
int pcreate(int count);
int pdelete(int fid);
int preceive(int fid,int *message);
int preset(int fid);
int psend(int fid, int message);
*/
#define PCREATE 0
void traitant_appels_systeme(uint32_t args[]);
void traitant_IT_49();
