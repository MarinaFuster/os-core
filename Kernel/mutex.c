#include <stdint.h>
#include <syscalls.h>
#include <scheduler.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <stdio.h> // We use this just for size of

extern uint64_t mutex_lock(uint64_t * mutexState);

typedef struct pids{
  uint8_t pid;
  struct pids * next;
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

uint8_t mutexLock(uint8_t mutexID, uint8_t callingPID){

    mutexNode * mutex=getMutex(mutexID);
    int wasLocked=mutex_lock(&(mutex->state));

    //Need some help with this linking it to the list of pids.....
    if(wasLocked){
      blockedState(callingPID);
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
  (newMutex->listOfPids)->next=0;
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
  if((mutex->listOfPids)->next==NULL){
    //Tengo uno solo
    pids * newPidNode = (pids *)allocate(sizeof(pids*));
    newPidNode->pid=callingPid;
    //add the pid node to the first position of the list of nodes
    newPidNode->next=mutex->listOfPids;
    (mutex->listOfPids)->next=newPidNode;
    return 1;
  }
  
  pids * current = mutex->listOfPids;
  while((current->next)->pid != (mutex->listOfPids)->pid){
    current=current->next;
  }
  pids * newPidNode = (pids *)allocate(sizeof(pids*));
  newPidNode->pid=callingPid;
  //add the pid node to the first position of the list of nodes
  newPidNode->next=mutex->listOfPids;
  current->next=newPidNode;
  return 1;
}