#include "mem.h"
#include "fileMessage.h"
#include "stddef.h"

#define NBQUEUE 20

QUEUE* queues[NBQUEUE]={[0 ... NBQUEUE-1] = NULL};
int numberQueues = 0;
struct processus* procBloque;

//renvoie la premiere indice vide. Si pleine, renvoie -1
int searchEmptyQueue(){
  for(int i = 0; i <= NBQUEUE-1; i++) {
    if(queues[i] == NULL) {
      return i;
    }
  }
  return -1;
}


//pcreate : crée une file de messages
int pcreate(int count){

  QUEUE* newQueue;
  int* queue;
  int freeIndex = searchEmptyQueue();
  if(count > 0 && freeIndex>=0){
    numberQueues++;
    newQueue = (QUEUE*)mem_alloc(sizeof(QUEUE));
    queue = (int*)mem_alloc(sizeof(int)*count);
    newQueue->message = queue;
    newQueue->capacite = count;
    newQueue->message = 0;
    queues[freeIndex] = newQueue;
    return freeIndex;
  } else {
    return -1;
  }
}

//pdelete : détruit une file de messages
int pdelete(int fid) {

  //verifie le fid
  if(fid<0 || fid>NBQUEUE-1 || queues[fid]==NULL)
    return -1;
  numberQueues--;

  //fait passer dans l'état activable tous les processus, s'il en existe, qui se trouvaient bloqués sur la file
  //Les processus libérés auront une valeur strictement négative comme retour de psend ou preceive.
  if (!queue_empty(&queues[fid]->process_send)) {
      //queue_for_each() changer l'état de chaque processus
  }else if (!queue_empty(&queues[fid]->process_receive)) {
      //queue_for_each() changer l'état de chaque processus
  }

  //on libere les messages et la structure et met NULL dans queues[fid]
  if(queues[fid]->message != NULL) {
    mem_free(queues[fid]->message,sizeof(int)*(queues[fid]->capacite));
  }
  mem_free(queues[fid],sizeof(QUEUE*));
  queues[fid]=NULL;
  return 0;

}

//psend : dépose un message dans une file


//preceive : retire un message d'une file


//preset : réinitialise une file


//pcount : renvoie l'état courant d'une file
