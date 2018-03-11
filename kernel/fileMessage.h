#include "queue.h"

#define NBQUEUE 20

typedef struct queue{
  int* message;
  int capacite;
  int numberMessages;   //combien de messages on a
  link process_send;
  link process_receive;
  link process_block;
} QUEUE;

//pcreate : crée une file de messages
int pcreate(int count);

//pdelete : détruit une file de messages
int pdelete(int fid);
