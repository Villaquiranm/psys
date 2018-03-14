#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H
#include "queue.h"
#include "processus.h"
#define NBQUEUE 20

typedef struct plink{
  struct processus * actuel;
  link head;
} PLINK;

typedef struct queue{
  int* message;
  int capacite;
  int numberMessages;   //combien de messages on a
  PLINK process_send;
  PLINK process_receive;
  //link process_block; on a pas besoin parce que si un processus est dans les autres structures il est déjà bloqué.
} QUEUE;

//pcreate : crée une file de messages
int pcreate(int count);

//pdelete : détruit une file de messages
int pdelete(int fid);
#endif
