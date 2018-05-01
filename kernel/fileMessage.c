#include "mem.h"
#include "fileMessage.h"
#include "stddef.h"


//les variables globales
QUEUE* queues[NBQUEUE]={[0 ... NBQUEUE-1] = NULL};
int numberQueues = 0;
extern link procsPrioQueue;

extern link procsPrioQueue;

extern struct processus* active; //TODO: à vérifier, éventuellement erreur
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
    newQueue->write = newQueue->message; //write et read pointent vers le début du vecteur de messages
    newQueue->read = newQueue->message;
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
  while ((plink_it = queue_out(&queues[fid]->process_send.head, PLINK, head)) != NULL) {
    plink_it->actuel->state = ACTIVABLE;
    queue_add(plink_it->actuel, &procsPrioQueue, processus, queueLink, prio);
    mem_free(plink_it, sizeof(PLINK));
  }
  while ((plink_it = queue_out(&queues[fid]->process_receive.head, PLINK, head)) != NULL) {
    plink_it->actuel->state = ACTIVABLE;
    queue_add(plink_it->actuel, &procsPrioQueue, processus, queueLink, prio);
    mem_free(plink_it, sizeof(PLINK));
  }

  numberQueues--;
  //on libere les messages et la structure et met NULL dans queues[fid]
  if(queues[fid]->message != NULL) {
    mem_free(queues[fid]->message, sizeof(int) * queues[fid]->capacite);
  }
  //mem_free(&queues[fid]->process_send.head, sizeof(link));
  mem_free(queues[fid], sizeof(QUEUE));
  queues[fid] = NULL;
  return 0;
}


void updateWritePointer(int fid, int capacite){
  if (queues[fid]->write == &(queues[fid]->message[capacite-1]) ){ //If Write points to the end of the vector
    queues[fid]->write = queues[fid]->message;// moves to first memory position.
  }
  else{
      queues[fid]->write++;// move to next memory position.
  }
}

void updateReadPointer(int fid, int capacite){
  if (queues[fid]->read == &(queues[fid]->message[capacite-1]) ){  //If Read points to the end of the vector
    queues[fid]->read = queues[fid]->message;// moves to first memory position.
  }
  else{
      queues[fid]->read++;// move to next memory position.
  }
}

//psend : dépose un message dans une file
int psend(int fid, int message){

  //valider FID
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL){
    return -1;
  }

  int nbMsgs = queues[fid]->numberMessages;
  int capacite = queues[fid]->capacite;

  //défensive, ce cas ne doit jamais arriver
  if(nbMsgs > capacite || nbMsgs < 0){
    return -1;
  }

  //si la file est vide
  //et que des processus sont bloqués en attente de message,
  //alors le processus le plus ancien dans la file parmi les plus prioritaires est débloqué et reçoit ce message
  if(nbMsgs == 0){
    // Send message
    *(queues[fid]->write) = message;
    queues[fid]->numberMessages++;
    updateWritePointer(fid, capacite);

    //2-Get first processus and give him execution time.
    if (!queue_empty(&queues[fid]->process_receive.head)) {// There is a processus to unblock?
        PLINK * processus_to_unblock = queue_out(&queues[fid]->process_receive.head, PLINK, head);
        processus_to_unblock->actuel->state = ACTIVABLE;
        queue_add(processus_to_unblock->actuel, &procsPrioQueue, processus, queueLink, prio);
        mem_free(processus_to_unblock, sizeof(PLINK));
        //ctx_sw(&active->regs ,&processus_to_unblock->actuel->regs)  Giving execution time to unblocked processus
        //Just realized that it'is not necessary because l'ordonnanceur will do it.
    }

  }

  //si la file est pleine,
  //le processus appelant passe dans l'état bloqué sur file pleine
  //jusqu'à ce qu'une place soit disponible dans la file pour y mettre le message.
  //Il est possible également, qu'après avoir été mis dans l'état bloqué sur file pleine,
  //le processus soit remis dans l'état activable par un autre processus ayant exécuté preset ou pdelete.
  //Dans ce cas, la valeur de retour de psend est strictement négative.
  else{
      if(nbMsgs == capacite){// Done.
        //printf("File pleine: %d messages\n", queues[fid]->numberMessages);
        PLINK* processus_bloque = (PLINK*)mem_alloc(sizeof(PLINK));
        processus_bloque->actuel = active;
        active->state = BLOQUE_IO;
        processus_bloque->prio = processus_bloque->actuel->prio;
        queue_add(processus_bloque,&queues[fid]->process_send.head, PLINK, head, prio);
        schedule();
        if (queues[fid] == NULL) {//
            return -1;
        }
    }

  //Ici la file n'est pas pleine et aucun processus n'est bloqué en attente de message.
  //Le message est alors déposé directement dans la file.

        //printf("La file a %d messages. On va ecrire.\n", queues[fid]->numberMessages);
        *(queues[fid]->write) = message;
        queues[fid]->numberMessages++;
        updateWritePointer(fid, capacite);
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
    processus_bloque->actuel = active;
    processus_bloque->prio = 0;
    active->state = BLOQUE_IO;
    queue_add(processus_bloque,&queues[fid]->process_receive.head, PLINK, head, prio);
    schedule();

    //Ici on a redonné la main au proc, donc il doit y avoir des messages à lire
    if (queues[fid] == NULL) {
        return -1;
    }

  }

  //il y a un message à lire
    *message = *(queues[fid]->read) ;
    queues[fid]->numberMessages--;
    updateReadPointer(fid, capacite);

    //si la file était pleine, débloque un processus
    if(nbMsgs == capacite){
      if (!queue_empty(&queues[fid]->process_send.head)) { // There is a processus to unblock?
          printf("Scheduler: THERE IS a process blocked to write\n");
          PLINK * processus_to_unblock = queue_out(&queues[fid]->process_send.head, PLINK, head);
          processus_to_unblock->actuel->state = ACTIVABLE;
          queue_add(processus_to_unblock->actuel, &procsPrioQueue, processus, queueLink, prio);
          mem_free(processus_to_unblock, sizeof(PLINK));
          //ctx_sw(&active->regs ,&processus_to_unblock->actuel->regs)  Giving execution time to unblocked processus
          //Just realized that it'is not necessary because l'ordonnanceur will do it.
      }else
        printf("Scheduler: no processes blocked to write\n");
    }

  return 0;
}


//preset : réinitialise une file
int preset(int fid){
    if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
        return -1;
    PLINK * plink_it;
    queues[fid]->numberMessages = 0; //Reset numberMessages
    while ((plink_it = queue_out(&queues[fid]->process_send.head, PLINK, head)) != NULL) {
      queue_add(plink_it->actuel, &procsPrioQueue, processus, queueLink, prio);
      mem_free(plink_it, sizeof(PLINK));
    }
    while ((plink_it = queue_out(&queues[fid]->process_receive.head, PLINK, head)) != NULL) {
      queue_add(plink_it->actuel, &procsPrioQueue, processus, queueLink, prio);
      mem_free(plink_it, sizeof(PLINK));
    }
    return 0;
}


//pcount : renvoie l'état courant d'une file
int pcount(int fid, int *count){

  int processusEnAttente=0;
  PLINK * plink_it;

  //l'indice de queue doit etre valide
  if(fid < NBQUEUE && queues[fid]!=NULL){

    if(count==NULL){
      return -1;
    }

    //si la queue est vide
    //on a des processus bloques en attendant pour lire des messages
    if(queues[fid]->numberMessages == 0){
        queue_for_each(plink_it, &queues[fid]->process_receive.head, PLINK, head){
            processusEnAttente++; //Count all processus bloques sur file vide.
        }
      *count = -processusEnAttente;
    }
    else{
      //si la queue est pleine
      //on a des processus bloques en attendant pour ecrire des messages
      if(queues[fid]->numberMessages == queues[fid]->capacite){
        queue_for_each(plink_it, &queues[fid]->process_send.head, PLINK, head){
            processusEnAttente++; //Count all processus bloques sur file pleine.
        }
        *count = queues[fid]->numberMessages + processusEnAttente;
      }
    }
    return 0;
  }
  else
    return -1;
}
int last_char(int fid){
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
      return -1;
  if (queues[fid]->numberMessages > 0) {
    queues[fid]->numberMessages--;
    if (queues[fid]->write == (queues[fid]->message) ){ //If Write points to the begining of the vector
      queues[fid]->write = &queues[fid]->message[queues[fid]->capacite-1];// moves to last position.
    }
    else{
        queues[fid]->write--;// move to next memory position.
    }
  }
  return 0;
}
