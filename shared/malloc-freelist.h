#include "stddef.h"
#include "stdint.h"
#include "queue.h"

typedef struct {
    link node;
    size_t size; //maybe use int
    char * block; //adresse physique
} alloc_node_t;

#define ALLOC_HEADER_SZ offsetof(alloc_node_t, block)

void malloc_addblock(void *addr, size_t size);
