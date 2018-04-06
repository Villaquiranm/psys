#include "fileMessage.h"
#include "processus.h"
#include "queue.h"
#include "mem.h"
#include "horloge.h"
#include "prueba.h"

QUEUE* newQueue;

void initFile(){
     pcreate(2);
     start(&prod,"Producteur", 1024, 10, NULL);
     start(&cons,"Consomateur", 1024, 10, NULL);



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
int cons() {
    for (;;) {
        int message;
        int error = preceive(0,&message);
        if (error >= 0) {
            printf("J'ai resu le message : %d\n",message);
        }else{
            return 0; //printf("Consomateur : erreur\n");
        }
        wait_clock(2);
        //schedule();
    }
    return 0;
}
int prod(){
    int message = 0;
    for (;;) {
        message++;
        int error = psend(0,message);
        if (error >= 0) {
            printf("J'ai deja envoye le message : %d\n",message);
        }else{
            return 0;//printf("Producteur : erreur\n");
        }
        wait_clock(2);
        //schedule();
    }
    return 0;
}
