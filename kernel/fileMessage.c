#include "mem.h"
#include "fileMessage.h"
#include "stddef.h"


//les variables globales
QUEUE* queues[NBQUEUE]={[0 ... NBQUEUE-1] = NULL};
int numberQueues = 0;
//struct processus* procBloque;

//renvoie la premiere indice vide. Si pleine, renvoie -1
int searchEmptyPosition(){
  for(int i = 0; i <= NBQUEUE-1; i++) {
    if(queues[i] == NULL) {
      return i;
    }
  }
  return -1;
}


//pcreate : crée une file de messages
int pcreate(int count) {
  QUEUE* newQueue;
  int freeIndex = searchEmptyPosition();
  if(count > 0 && freeIndex>=0){
    newQueue = (QUEUE*)mem_alloc(sizeof(QUEUE));
    newQueue->message = (int*)mem_alloc(sizeof(int) * count);
    newQueue->capacite = count;
    newQueue->numberMessages = 0;
    //initialize processuslink in the queue.
    INIT_LIST_HEAD(&newQueue->process_send.head);
    INIT_LIST_HEAD(&newQueue->process_receive.head);
    numberQueues++;
    queues[freeIndex] = newQueue;
    return freeIndex;
  } else {
    return -1;
  }
}

//pdelete : détruit une file de messages
int pdelete(int fid) {

  PLINK* plink_it;

  //verifie le fid
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
    return -1;
  //fait passer dans l'état activable tous les processus, s'il en existe, qui se trouvaient bloqués sur la file
  //Les processus libérés auront une valeur strictement négative comme retour de psend ou preceive.
  if (!queue_empty(&queues[fid]->process_send.head)) {
      queue_for_each(plink_it, &queues[fid]->process_send.head, PLINK, head){
        plink_it->actuel->etat = ACTIVABLE; //changer l'état de chaque processus}
      }
  }else if (!queue_empty(&queues[fid]->process_receive.head)) {
      queue_for_each(plink_it, &queues[fid]->process_receive.head, PLINK, head){
        plink_it->actuel->etat = ACTIVABLE; //changer l'état de chaque processus}
      }
  }

  numberQueues--;
  //on libere les messages et la structure et met NULL dans queues[fid]
  if(queues[fid]->message != NULL) {
    mem_free(queues[fid]->message, sizeof(int) * queues[fid]->capacite);
  }
  mem_free(queues[fid], sizeof(QUEUE));
  queues[fid] = NULL;
  return 0;

}

//psend : dépose un message dans une file


//preceive : retire un message d'une file


//preset : réinitialise une file


//pcount : renvoie l'état courant d'une file
