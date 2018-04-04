#include "mallocfreelist.h"
#include "sprintf.c"

#define ALIGN_UP(num, align) ((((num) + (align) - 1) ) & ~((align) - 1))

ll_m freelist = { NULL, &(freelist), &(freelist) };
ll_m occupiedlist = { NULL, &(occupiedlist), &(occupiedlist) };

void malloc_addblock(void *addr, size_t size) {
    alloc_node_t *blk;

    blk = (void *)ALIGN_UP((ptrdiff_t)addr, sizeof(void*));

    blk->size = (ptrdiff_t) addr + size - (ptrdiff_t)blk - sizeof(size_t);

    //Add to the end of the freelist
    ADD_LIST_NEWNODE(freelist, blk)
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
                alloc_node_t *newBlock; //the new free block

                newBlock = (alloc_node_t *) ((ptrdiff_t)(&blk->block) + size);
                newBlock->size = blk->size - size - sizeof(size_t);
                blk->size = size;

                //Add to the end of the freelist
                ADD_LIST_NEWNODE(freelist, newBlock)
            }
            //Delete it from the freelist
            current->next->prev = current->prev;
            current->prev->next = current->next;
            //And put it in the occupiedlist
            ADD_LIST(occupiedlist, current)
        }
    }

    return ptr;
}

void fl_free(void * ptr) {
    alloc_node_t *block;
    bool isOccupied = false;
    ll_m *current = occupiedlist.next;

    if(ptr) {
        while(!isOccupied && current != NULL) {
            block = current->node;
            if(&(block->block) == ptr) {
                isOccupied = true;
            } else if (current->next == NULL) {
                break;
            } else {
                current = current->next;
            }
        }
        if(isOccupied) {
            free(ptr);
            //Delete it from the occupiedlist
            current->next->prev = current->prev;
            current->prev->next = current->next;
            current->node->block = NULL;
            //Try to merge blocks
            ADD_LIST(freelist, current)
            merge_blocks(current);
        }
    }
}

bool merge_blocks(ll_m *block) {
    ll_m *current = freelist.prev;
    bool blockMerge = false;

    while(current->node != NULL) {
        if((void *)((int)&(block->node->block) + block->node->size) == (current->node)) { //if block is immediately before current
            block->node->size = current->node->size + block->node->size + sizeof(size_t);
            block->node->block = NULL;
            blockMerge = true;
            //Delete current from freelist
            current->next->prev = current->prev;
            current->prev->next = current->next;
            //free current
            free(current);
        } else if ((void *)((int)&(current->node->block) + current->node->size) == (block->node)) { //if block comes immediately after current
            current->node->size = current->node->size + block->node->size + sizeof(size_t);
            current->node->block = NULL;
            //Delete it from freelist
            block->next->prev = block->prev;
            block->prev->next = block->next;
            //free block
            free(block);
            block = current;
            blockMerge = true;
        }
        current = current->prev;
    }

    return blockMerge;
}
