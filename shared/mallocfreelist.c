#include "mallocfreelist.h"

#define ALIGN_UP(num, align) ((((num) + (align) - 1) ) & ~((align) - 1))

ll_m freelist = { NULL, &(freelist), &(freelist) };
ll_m occupiedlist = { NULL, &(occupiedlist), &(occupiedlist) };

void malloc_addblock(void *addr, size_t size) {
    alloc_node_t *blk;
    ll_m newNode;

    blk = (void *)ALIGN_UP((ptrdiff_t)addr, sizeof(void*));

    blk->size = (ptrdiff_t) addr + size - (ptrdiff_t) blk; //ALLOC_HEADER_SZ ???

    //Add to the end of the freelist
    newNode.next = &freelist;
    newNode.prev = freelist.prev;
    newNode.node = blk;
    freelist.prev->next = &newNode;
    freelist.prev = &newNode;
}

void * fl_malloc(size_t size) {
    void * ptr = NULL;
    alloc_node_t *blk = NULL;
    bool hasBlock = false;
    ll_m *current = &freelist;

    if(size > 0){
        while (!hasBlock) {
            blk = current->node;
            if((blk != NULL) && (blk->size >= size)) {
                ptr = &(blk->block);
                hasBlock = true;
            } else if (current->next == NULL){
                break;
            } else {
                current = current->next;
            }
        }

        if(ptr) {
            if(blk->size - size > MIN_ALOC_SIZE) {
                alloc_node_t *newBlock;
                ll_m newNode;

                newBlock = (alloc_node_t *) ((ptrdiff_t)(&blk->block) + size);
                newBlock->size = blk->size - size;
                blk->size = size;

                //Add to the end of the freelist
                newNode.next = &freelist;
                newNode.prev = freelist.prev;
                newNode.node = newBlock;
                freelist.prev->next = &newNode;
                freelist.prev = &newNode;
            }
            //Delete it from the freelist
            current->next->prev = current->prev;
            current->prev->next = current->next;
            //And put it in the occupiedlist
            current->next = &occupiedlist;
            current->prev = occupiedlist.prev;
            occupiedlist.prev = current;
        }
    }

    return ptr;
}

void fl_free(void * ptr) {
    alloc_node_t *block;
    bool isOccupied = false;
    ll_m *current = &occupiedlist;

    if(ptr) {
        while(!isOccupied) {
            block = current->node;
            if(block->block == ptr) {
                isOccupied = true;
            } else if (current->next == NULL) {
                break;
            } else {
                current = current->next;
            }
        }
        //Delete it from the occupiedlist
        current->next->prev = current->prev;
        current->prev->next = current->next;
        //And put it back in the freelist
        current->next = &freelist;
        current->prev = freelist.prev;
        freelist.prev = current;
    }
}
