#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H
#include "queue.h"
#include "processus.h"
#include "stdbool.h"
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


//pcreate : crée une file de messages, alloue une file de capacité égale à la valeur de count
int pcreate(int count);

//pdelete : détruit une file de messages
int pdelete(int fid);
//La primitive pcount lit la quantité de données et de processus en attente sur la file fid.
int pcount(int fid, int *count);
//La primitive preceive lit et enlève le premier (plus ancien) message de la file fid.
int preceive(int fid,int *message);
//La primitive preset vide la file identifiée par la valeur de fid et fait passer dans l'état activable ou actif (selon les priorités) tous les processus
int preset(int fid);
//La primitive psend envoie le message dans la file identifiée par fid.
int psend(int fid, int message);
//eliminates last character
int last_char(int fid);
//File pleine;
bool estPleine(int fid);
#endif
