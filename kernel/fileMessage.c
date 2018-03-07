#define NBQUEUE 20

QUEUE* queues[NBQUEUE];
int numberQueues = 0;
struct processus* procBloque;

//pcreate : crée une file de messages
int pcreate(int count){

  QUEUE* newQueue;
  int* queue;

  if(numberQueues <= (NBQUEUE-1)){
    numberQueues++;
    newQueue = (QUEUE*)malloc(sizeof(QUEUE)*count);
    queue = (int*)malloc(sizeof(int)*count);
    newQueue->message = queue;
    newQueue->capacite = count;
    newQueue->message = 0;
  }
}

//pdelete : détruit une file de messages


//psend : dépose un message dans une file


//preceive : retire un message d'une file


//preset : réinitialise une file


//pcount : renvoie l'état courant d'une file
