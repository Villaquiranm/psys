#include <string.h>
#include <stdio.h>
#include "processus.h"
#include "stdint.h"
#include "segment.h"
#include "cpu.h"
#include "queue.h"
#include "mem.h"
#include "syscalls.h"

void traitant_appels_systeme(uint32_t args[]){

    printf("traitant_appels_systeme\n");

    uint32_t numAppel = args[0];

    switch (numAppel) {
        case PCREATE: pcreate(args[2]);
            break;
    }

}
