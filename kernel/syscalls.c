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

void traitant_appels_systeme(uint32_t args[]){

    printf("traitant_appels_systeme\n");

    uint32_t numAppel = args[0];

    switch (numAppel) {


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

    }

}
