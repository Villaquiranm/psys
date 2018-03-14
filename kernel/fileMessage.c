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
    INIT_LINK(&newQueue->process_send.head);
    INIT_LINK(&newQueue->process_receive.head);
    numberQueues++;
    queues[freeIndex] = newQueue;
    return freeIndex;
  } else {
    return -1;
  }
}

//pdelete : détruit une file de messages
int pdelete(int fid) {
  //verifie le fid
  if(fid < 0 || fid > NBQUEUE-1 || queues[fid] == NULL)
    return -1;
  //fait passer dans l'état activable tous les processus, s'il en existe, qui se trouvaient bloqués sur la file
  //Les processus libérés auront une valeur strictement négative comme retour de psend ou preceive.
  //if (!queue_empty(&queues[fid]->process_send)) {
      //queue_for_each() changer l'état de chaque processus
//  }else if (!queue_empty(&queues[fid]->process_receive)) {
      //queue_for_each() changer l'état de chaque processus
  //}
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
  if(nbMsgs == 0 /*TODO && il y a des procs bloques*/){

  }

  //si la file est pleine,
  //le processus appelant passe dans l'état bloqué sur file pleine
  //jusqu'à ce qu'une place soit disponible dans la file pour y mettre le message.
  //Il est possible également, qu'après avoir été mis dans l'état bloqué sur file pleine,
  //le processus soit remis dans l'état activable par un autre processus ayant exécuté preset ou pdelete.
  //Dans ce cas, la valeur de retour de psend est strictement négative.
  else
  if(nbMsgs == capacite){

  }

  //Sinon, la file n'est pas pleine et aucun processus n'est bloqué en attente de message.
  //Le message est alors déposé directement dans la file.
  else{
    queues[fid]->message[nbMsgs] = message;
    queues[fid]->numberMessages++;
  }

  return 0;
}


//TODO la liste de messages ne peut pas être un vecteur, parce qu'on doit lire
//l'élement le plus ancien e le rétirer
//idée: utiliser l'API de link une fois qu'elle est comprise
//ou créer une liste enchaînée pour les messages


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

  }

  //sinon, il y a un message à lire
  else{

    //lit le message et le supprime de la file
    *message = 0;//TODO pas 0, getOldestMessageAndDeleteIt;

    //si la file était pleine, débloque un processus
    if(nbMsgs == capacite){

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
