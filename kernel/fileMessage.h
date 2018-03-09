#include "../shared/queue.h"

typedef struct queue{
  int* message;
  int capacite;
  int numberMessages;   //combien de messages on a
  link process_send;
  link process_receive;
} QUEUE;
