#include <string.h>
#include <stdio.h>
#include "processus.h"
#include "stdint.h"
#include "segment.h"
#include "cpu.h"
#include "queue.h"
#include "mem.h"
#include "syscalls.h"
#include "fileMessage.h"
#include "horloge.h"
#include "syscalls.h"
#include "string.h"
#include "div64.h"

typedef unsigned long long uint_fast64_t;
typedef unsigned long uint_fast32_t;

void traitant_appels_systeme(uint32_t args[]){

    printf("traitant_appels_systeme\n");

    uint32_t numAppel = args[0];

    switch (numAppel) {

        case CHPRIO: chprio(args[1], args[2]);
            break;
        /*case CONS_WRITE: cons_write(args[1], args[2]);
            break;
        case CONS_READ: cons_read(args[1], args[2]);
            break;
        case CONS_ECHO: cons_echo(args[1]);
            break;
        case EXIT: exit(args[1]);
            break;  */
        case GETPID:  getpid();
            break;
        case GETPRIO: getprio(args[1]);
            break;
        case KILL: kill(args[1]);
            break;
        case PCOUNT: pcount(args[1], (int*)args[2]);
            break;
        case PCREATE: pcreate(args[1]);
            break;
        case PDELETE: pdelete(args[1]);
            break;
        case PRECEIVE: preceive(args[1], (int*)args[2]);
            break;
        case PRESET: preset(args[1]);
            break;
        case PSEND: psend(args[1], (int)args[2]);
            break;
        case CLOCK_SETTINGS: clock_settings((unsigned long *)args[1], (unsigned long *)args[2]);
            break;
        case CURRENT_CLOCK: current_clock();
            break;
        case WAIT_CLOCK: wait_clock((unsigned long) args[1]);
            break;
        case START: //start((const char *)args[1], (unsigned long)args[2], (int)args[3], (void *)args[4]);
            break;
        case WAITPID: waitpid((int) args[1], (int *)args[2]);
            break;
        case SYS_INFO: //sys_info();
            break;

            /*
        case STRCMP: strcmp((const char *)args[1], (const char *)args[2]);
            break;
        case STRLEN: strlen((const char *)args[1]);
            break;
        case STRNCPY: strncpy((char *)args[1], (const char *)args[2], (unsigned) args[3]);
            break;
        case MEMSET: memset((void *)args[1], (int) args[2], (size_t) args[3]);
            break;
            

        case SAFE_PRINTF: safe_printf(const char *format, ...); //TODO
            break;
        case CONS_GETS: cons_gets((char *)args[1], (unsigned long) args[2]);
            break;
        case ASSERT_FAILED: assert_failed((const char *)args[1], (const char *)args[2], (int) args[3]);
            break;

        case RANDSHORT: randShort();
            break;
        case SETSEED: setSeed((uint_fast64_t) args[1]);
            break;
        case RAND: rand();
            break;
        case DIV64: div64((unsigned long long) args[1], (unsigned long long) args[2], (unsigned long long *)args[3]);
            break;
        case TEST_IT: test_it();
            break; */
        default:
            printf("Appel systeme inconnu\n");
    }

}
