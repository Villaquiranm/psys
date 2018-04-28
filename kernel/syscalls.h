#define CHPRIO 0 ok
#define CONS_WRITE 1 ok
#define CONS_READ 2 ok
#define CONS_ECHO 3 ok
#define EXIT 4 ok
#define GETPID 5 ok
#define GETPRIO 6 ok
#define KILL 7 ok
#define PCOUNT 8 ok
#define PCREATE 9 ok
#define PDELETE 10 ok
#define PRECEIVE 11 ok
#define PRESET 12 ok
#define PSEND 13 ok
#define CLOCK_SETTINGS 14 ok
#define CURRENT_CLOCK 15
#define WAIT_CLOCK 16
#define START 17
#define WAITPID 18
#define SYS_INFO 19
#define STRNCMP 20
#define STRLEN 21
#define STRNCPY 22
#define MEMSET 23
#define SAFE_PRINTF 24
#define CONS_GETS 25
#define ASSERT_FAILED 26
#define RANDSHORT 27
#define SETSEED 28
#define RAND 29
#define DIV64 30
#define TEST_IT 31

void traitant_appels_systeme(uint32_t args[]);
void traitant_IT_49();
