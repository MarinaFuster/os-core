#include <stdint.h>
#include <syscalls.h>
#include <scheduler.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <stdio.h> // We use this just for size of

extern uint64_t mutex_lock(uint64_t * mutexState);

typedef struct mutexNode{
  uint64_t state;
  uint8_t mutexID;
  struct mutexNode * next;
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

    if(wasLocked){
      //ncPrint(" El mutex estaba bloqueado entonces su estado es ");
      //ncPrintDec(mutex->state);
      blockedState(callingPID);
    }
    else{
      unblockedState(callingPID);
      //ncPrint(" Ahora el estado del mutex es ");
      //ncPrintDec(mutex->state);
    }
    return 1;
}

uint8_t mutexUnlock(int mutexID, uint8_t otherPID){
  //ncPrint(" Quiero desbloquear el mutex... ");
  mutexNode * mutex=getMutex(mutexID);
  mutex->state=UNLOCKED;
  //ncPrintDec(mutex->state);
  unblockedState(otherPID);

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

void add(mutexNode * newMutex){
  newMutex->next=first;
  first=newMutex;
}

uint8_t initMutex(){
  mutexNode * newMutex=(mutexNode *)allocate(sizeof(*newMutex));
  newMutex->state=UNLOCKED;
  newMutex->mutexID=mutexIDCounter;
  add(newMutex);
  mutexIDCounter++;

  return newMutex->mutexID;
}

