#include "fileMessage.h"
#include "processus.h"
#include "queue.h"
#include "mem.h"
#include "horloge.h"
#include "prueba.h"
#include "cpu.h"
#include "pilote.h"

QUEUE* newQueue;

void initFile(){
     int a = pcreate(3);
     printf("Creating file : %d\n",a);
     start(&ecriteur,"Ecriteur", 1024, 10, NULL);
     //start(&eliminateur,"Eliminateur", 1024, 10, NULL);
     //start(&cons,"Consommateur", 1024, 10, NULL);

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
        int count;
        int error = preceive(0,&message);
        if (error >= 0) {
            printf("J'ai recu le message : %d\n",message);
        }else{
            return 0; //printf("Consomateur : erreur\n");
        }
        count = pcount(0, &count);
        printf("Pcount: %d\n",count);
        wait_clock(100);
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
            printf("Producteur : erreur\n");
        }
        wait_clock(10);
        //schedule();
    }
    return 0;
}
int eliminateur(){
  while (1) {
    wait_clock(11);
    preset(0);
    printf("Resetting fileMessage : 0....\n");
    //wait_clock(10);
    //printf("deleting fileMessage : 0 ....\n");
    //pdelete(0);
    //wait_clock(10);
    //int a = pcreate(3);
    //printf("Creating fileMessage : %d ....\n",a);
  }
  return 0;
}
int ecriteur(){
  char string[10];
  while (1) {
    unsigned long length = cons_read(string, 10);
    cons_write(string, length);
  }
  return 0;
}
