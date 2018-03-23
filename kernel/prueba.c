#include "fileMessage.h"
#include "processus.h"
#include "queue.h"
#include "mem.h"

QUEUE* newQueue;

void initFile(){
      newQueue = (QUEUE*)mem_alloc(sizeof(QUEUE));
      newQueue->message = (int*)mem_alloc(sizeof(int) * 2);
      newQueue->capacite = 2;
      newQueue->numberMessages = 0;
      INIT_LIST_HEAD(&newQueue->process_send.head);
      INIT_LIST_HEAD(&newQueue->process_receive.head);
}
void addProcessus(struct processus * proc, int prio){
    PLINK * ptr = (PLINK *)mem_alloc(sizeof(PLINK));
    ptr->actuel = proc;
    ptr->prio = prio;
    queue_add(ptr, &newQueue->process_send.head, PLINK, head, prio);
}
void showProcessus(){
    PLINK* plink_it;
    queue_for_each(plink_it, &newQueue->process_send.head, PLINK, head){
      printf("Mi prioridad es %d\n",plink_it->prio);
    }
}
