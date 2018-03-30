#ifndef MALLOCFREELIST_H
#define MALLOCFREELIST_H

#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"

#define MIN_ALOC_SIZE 8 //Not sure

typedef struct {
    size_t size;    //maybe use int
    char * block;   //adresse physique
} alloc_node_t;

typedef struct ll_head {
    alloc_node_t *node;
    struct ll_head *prev;
    struct ll_head *next;
} ll_m;

#define ALLOC_HEADER_SZ offsetof(alloc_node_t, block)
#define ADD_LIST(list, block)       ll_m *newNode = (ll_m*)malloc(sizeof(ll_m)); \
                                    newNode->next = &list; \
                                    newNode->prev = list.prev; \
                                    newNode->node = block; \
                                    list.prev->next = newNode; \
                                    list.prev = newNode;

void malloc_addblock(void *addr, size_t size);
void * fl_malloc(size_t size);
void fl_free(void * ptr);

#endif
