#include <stdint.h>
#include <syscalls.h>
#include <scheduler.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <stdio.h> // We use this just for size of

#define HUNGRY 1
#define THINKING 2
#define EATING 3

extern uint64_t mutex_lock(uint64_t * mutexState);

typedef struct pids{
  uint8_t pid;
  uint8_t state;
  struct pids * next;
  struct pids * prev;
} pids;

typedef struct mutexNode{
  uint64_t state;
  uint8_t mutexID;
  struct mutexNode * next;
  pids * listOfPids;
} mutexNode;

static mutexNode * first=0;
static  int mutexIDCounter=1;

mutexNode * getMutex(uint8_t mutexID){
  mutexNode * current=first;
  while(current!=0){
    if(current->mutexID==mutexID)
      return current;
    current=current->next;
  }
  return current;
}

void removeFromMutex(uint8_t mutexID, uint8_t pid){
  mutexNode * mutex=getMutex(mutexID);
  pids* current=(mutex->listOfPids);

  if(current==NULL){
    return;
  }

  if(!(inMutex(1, pid))) {
    return;
  }

  while((current->pid)!=pid){   // ARREGLAR SI NO ENCUENTRA PID
    current=current->next;
  }
  if( ((current->next)->pid)==(current->pid) ){ // Hay uno solo
    mutex->listOfPids=0;

    return;
  }
  else{   // Hay mas de uno

    ncPrint("El PID a remover es: ");
    ncPrintDec(pid);
    ncNewline();
    ncPrint("Cuyo valor izquierdo es ");
    ncPrintDec(current->prev->pid);
    ncPrint(" y el valor derecho es ");
    ncPrintDec(current->next->pid);
    ncNewline();


    (current->prev)->next=current->next;
    (current->next)->prev=current->prev;
    ncNewline();
    ncPrint("El valor izquierdo del pid: ");
    ncPrintDec((current->next)->pid);
    ncPrint(" es ");
    ncPrintDec(((current->next)->next)->pid);
    ncNewline();
    ncPrint("El valor derecho es ");
    ncPrintDec(((current->next)->prev)->pid);
    ncNewline();
    ////////// LIBERAR MEMORIA!!!! ///////////
  }

  //if(((mutex->listOfPids)->pid)==pid){
  //  mutex->listOfPids=current->next;
  //}
}

uint8_t mutexLock(uint8_t mutexID, uint8_t callingPID){

    mutexNode * mutex=getMutex(mutexID);
    int wasLocked=mutex_lock(&(mutex->state));

    //Need some help with this linking it to the list of pids.....
    if(wasLocked){
      blockedState(callingPID);
      while(isBlocked((callingPID))){
        ;
      }
    }
    else{
      unblockedState(callingPID); //unnecesary??!?!?!
    }
    return 1;
}

uint8_t mutexUnlock(int mutexID){
  //I want to unlock the next pid
  mutexNode * mutex=getMutex(mutexID);
  mutex->state=UNLOCKED;
  //problema: que vaya siempre al mismo pid

  pids * first = mutex->listOfPids; //Going to the end
  pids * firstNext = first->next; //Going to be the first one
  unblockedState(firstNext->pid);
  mutex->listOfPids=firstNext;

  return 1;
}

mutexNode * destroyMutexRec(mutexNode * current, uint8_t mutexID){
  if(current==0)
    return current;
  if(current->mutexID==mutexID){
    mutexNode * toReturn=current->next;
    free((uint64_t)current);
    return toReturn;
  }
  current->next=destroyMutexRec(current->next, mutexID);
  return current;
}

void destroyMutex(uint8_t mutexID){
  first=destroyMutexRec(first, mutexID);
}

void add(mutexNode * newMutex, uint8_t callingPid){
  newMutex->listOfPids=(pids *)allocate(sizeof(pids*));
  (newMutex->listOfPids)->pid=callingPid;
  (newMutex->listOfPids)->state=THINKING;
  (newMutex->listOfPids)->next=newMutex->listOfPids;
  (newMutex->listOfPids)->prev=newMutex->listOfPids;
  newMutex->next=first;
  first=newMutex;
}

uint8_t initMutex(uint8_t callingPid){
  mutexNode * newMutex=(mutexNode *)allocate(sizeof(*newMutex));
  newMutex->state=UNLOCKED;
  newMutex->mutexID=mutexIDCounter;
  add(newMutex,callingPid);
  mutexIDCounter++;

  return newMutex->mutexID;
}
//This method is to vinculate the mutex with all the processes connected to
//given mutex
uint8_t connectToMutex(uint8_t mutexID, uint8_t callingPid){
  mutexNode * mutex=getMutex(mutexID);
  if(mutex==NULL){
    ncPrint("Mutex id does not exist");
    return 0; //Mutex does not exist
  }
  //Agrego el pid al final

  if((mutex->listOfPids)==NULL){
    pids * newPidNode = (pids *)allocate(sizeof(pids*));
    newPidNode->pid=callingPid;
    newPidNode->state=THINKING;
    //add the pid node to the first position of the list of nodes
    newPidNode->prev=newPidNode;
    newPidNode->next=newPidNode;
    (mutex->listOfPids)=newPidNode;

    return 1;
  }

  if( (mutex->listOfPids->pid)==((mutex->listOfPids)->next)->pid){
    //Tengo uno solo
    pids * newPidNode = (pids *)allocate(sizeof(pids*));
    newPidNode->pid=callingPid;
    newPidNode->state=THINKING;
    //add the pid node to the first position of the list of nodes
    newPidNode->prev=mutex->listOfPids;
    newPidNode->next=mutex->listOfPids;
    (mutex->listOfPids)->next=newPidNode;
    (mutex->listOfPids)->prev=newPidNode;
    return 1;
  }

  pids * current = (mutex->listOfPids)->next;
  while((current->next)->pid != (mutex->listOfPids)->pid){
    current=current->next;
  }
  pids * newPidNode = (pids *)allocate(sizeof(pids*));
  newPidNode->pid=callingPid;
  newPidNode->state=THINKING;
  //add the pid node to the first position of the list of nodes
  newPidNode->next=mutex->listOfPids;
  newPidNode->prev=current;
  current->next=newPidNode;
  (mutex->listOfPids)->prev=newPidNode;
  return 1;
}

uint8_t inMutex(uint8_t mutexID, uint8_t pid){
  int found=0;
  mutexNode * mutex=getMutex(mutexID);
  pids* current=(mutex->listOfPids)->next;
  pids* first=mutex->listOfPids;

  if(first->pid==pid)
    found=1;

  while((first->pid!=current->pid)&&(found==0)){
    if(current->pid==pid){
      found=1;
    }
    current=current->next;
  }

  if (found==1){
    return 1;
  }

  return 0;
}

uint8_t inMutexCheckFront(uint64_t ans, uint8_t pid){
  uint8_t * aux=ans;
  *aux=inMutex(1, pid);
  return 1;
}


uint8_t checkPhi(uint8_t mutexID, uint8_t pid){
  mutexNode * mutex=getMutex(mutexID);
  pids* current=(mutex->listOfPids);

  if(!(inMutex(1, pid))) {
    return 1;
  }

  ncPrint("Philosopher ");
  ncPrintDec(pid);
  ncPrint(" is hungry");
  ncNewline();

  int j=0;
  while(j<500000000)
      j++;

  while((current->pid)!=pid){
    current=current->next;
  }

  ncPrint("Soy el filosofo con pid: ");
  ncPrintDec(pid);
  ncNewline();
  ncPrint("Mi valor derecho es: ");
  ncPrintDec((current->next)->pid);
  ncNewline();
  ncPrint("Y mi valor izquierdo es: ");
  ncPrintDec((current->prev)->pid);
  ncNewline();

  if ( (((current->next)->state)!=EATING) && (((current->prev)->state)!=EATING) ){
    current->state=EATING;
    ncPrint("Philosopher ");
    ncPrintDec(pid);
    ncPrint(" is currently eating");
    ncNewline();
    return 1;
  }
  return 0;
}

uint8_t changePhiState(uint8_t mutexID,uint8_t pid, uint8_t state){
  mutexNode * mutex=getMutex(mutexID);
  pids* current=(mutex->listOfPids);

  if(!(inMutex(1, pid))) {    // Bocho a todos los que no esten en la mesa
    return 1;
  }

  while((current->pid)!=pid){
    current=current->next;
  }


  if(current->state==EATING){
    ncPrint("Philosopher ");
    ncPrintDec(pid);
    ncPrint(" is putting the chopsticks down");
    ncNewline();
  }
  current->state=state;
  return 1;
}

uint8_t checkCircularList(uint8_t mutexID){
  mutexNode * mutex=getMutex(mutexID);

  if(mutex==0){
    ncPrint("Error, no existe ese mutex");
    ncNewline();
    return 0;
  }

  if(mutex->listOfPids==NULL){
    return 0;
  }

  pids* first=(mutex->listOfPids);

  ncPrintDec(first->pid);

  pids* current=(mutex->listOfPids)->next;
  while((current->pid)!=first->pid){
    ncPrintDec(current->pid);
    current=current->next;
  }
  ncNewline();
  return 0;
}
