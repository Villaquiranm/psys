#include "mem.h"
#include "fileMessage.h"
#include "stddef.h"


//les variables globales
QUEUE* queues[NBQUEUE]={[0 ... NBQUEUE-1] = NULL};
int numberQueues = 0;

extern struct processus* actif; //TODO: à vérifier, éventuellement erreur
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
    newQueue->write = (int)newQueue->message;
    newQueue->read = (int)newQueue->message;
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
int psend(int fid, int message){

  //valider FID
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
    return -1;

  int nbMsgs = queues[fid]->numberMessages;
  int capacite = queues[fid]->capacite;

  //défensive, ce cas ne doit jamais arriver
  if(nbMsgs > capacite || nbMsgs < 0)
    return -1;

  //si la file est vide
  //et que des processus sont bloqués en attente de message,
  //alors le processus le plus ancien dans la file parmi les plus prioritaires est débloqué et reçoit ce message
  if(nbMsgs == 0){
    // Send message
    queues[fid]->write = message;
    queues[fid]->numberMessages++;
    if (queues[fid]->write == queues[fid]->message[capacite-1]){
      queues[fid]->write = (int)queues[fid]->message;// move to first memory position.
    }
    else{
        queues[fid]->write++;// move to next memory position.
    }
    //2-Get first processus and give him execution time.
    //TODO know if there is a process to unblock
    PLINK * processus_to_unblock = queue_out(&queues[fid]->process_send.head, PLINK, head);
    processus_to_unblock->actuel->etat = ACTIVABLE;
    //ctx_sw(&actif->regs ,&processus_to_unblock->actuel->regs)  Giving execution time to unblocked processus
  }

  //si la file est pleine,
  //le processus appelant passe dans l'état bloqué sur file pleine
  //jusqu'à ce qu'une place soit disponible dans la file pour y mettre le message.
  //Il est possible également, qu'après avoir été mis dans l'état bloqué sur file pleine,
  //le processus soit remis dans l'état activable par un autre processus ayant exécuté preset ou pdelete.
  //Dans ce cas, la valeur de retour de psend est strictement négative.
  else if(nbMsgs == capacite){// Done.
    PLINK* processus_bloque = (PLINK*)mem_alloc(sizeof(PLINK));
    processus_bloque->actuel = actif;
    actif->etat = BLOQUE_IO;
    processus_bloque->priorite = 0;
    queue_add(processus_bloque,&queues[fid]->process_send.head, PLINK, head, priorite);
  }

  //Sinon, la file n'est pas pleine et aucun processus n'est bloqué en attente de message.
  //Le message est alors déposé directement dans la file.
  else{// Done
    queues[fid]->write = message;
    queues[fid]->numberMessages++;
    if (queues[fid]->write == queues[fid]->message[capacite-1]){
      queues[fid]->write = (int)queues[fid]->message;// move to first memory position.
    }
    else{
        queues[fid]->write++;// move to next memory position.
    }
  }

  return 0;
}


//preceive : retire un message d'une file
int preceive(int fid,int *message){

  //valider FID
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
    return -1;

  int nbMsgs = queues[fid]->numberMessages;
  int capacite = queues[fid]->capacite;

  //défensive, ce cas ne doit jamais arriver
  if(nbMsgs > capacite || nbMsgs < 0)
    return -1;

  //si la file est vide, bloque le proc
  if(nbMsgs == 0){
    PLINK* processus_bloque = (PLINK*)mem_alloc(sizeof(PLINK));
    processus_bloque->actuel = actif;
    processus_bloque->priorite = 0;
    actif->etat = BLOQUE_IO;
    queue_add(processus_bloque,&queues[fid]->process_receive.head, PLINK, head, priorite);
  }

  //sinon, il y a un message à lire
  else{
    *message = queues[fid]->read ;
    queues[fid]->numberMessages--;
    if (queues[fid]->read == queues[fid]->message[capacite-1]){  //Write%capacite;
      queues[fid]->read = (int)queues[fid]->message;// move to first memory position.
    }
    else{
        queues[fid]->read++;// move to next memory position.
    }

    //si la file était pleine, débloque un processus
    if(nbMsgs == capacite){
      //TODO know if there is a process to unblock
      PLINK * processus_to_unblock = queue_out(&queues[fid]->process_receive.head, PLINK, head);
      processus_to_unblock->actuel->etat = ACTIVABLE;
      //ctx_sw(&actif->regs ,&processus_to_unblock->actuel->regs)  Giving execution time to unblocked processus

    }

  }

  return 0;
}

/*
//preset : réinitialise une file
int preset(int fid){
  return -1;
}


//pcount : renvoie l'état courant d'une file
int pcount(int fid, int *count){

  return -1;
}
*/
