#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H
#include "queue.h"
#include "processus.h"
#define NBQUEUE 20

typedef struct plink{
  struct processus * actuel;
  link head;
  int prio;
} PLINK;

typedef struct queue{
  int *message, *read, *write;
  int capacite;
  int numberMessages;   //combien de messages on a
  PLINK process_send;
  PLINK process_receive;
  //link process_block; on en a pas besoin parce que si un processus est dans les autres structures il est déjà bloqué.
} QUEUE;


//pcreate : crée une file de messages
int pcreate(int count);

//pdelete : détruit une file de messages
int pdelete(int fid);
int pcount(int fid, int *count);
int preceive(int fid,int *message);
int preset(int fid);
int psend(int fid, int message);
#endif
