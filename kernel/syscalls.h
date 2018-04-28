#define CHPRIO 0
#define CONS_WRITE 1
#define CONS_READ 2
#define CONS_ECHO 3
#define EXIT 4
#define GETPID 5
#define GETPRIO 6
#define KILL 7
#define PCOUNT 8
#define PCREATE 9
#define PDELETE 10
#define PRECEIVE 11
#define PRESET 12
#define PSEND 13



void traitant_appels_systeme(uint32_t args[]);
void traitant_IT_49();
