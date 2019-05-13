#include <stdint.h>
#include <processController.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <stdio.h>
#include <naiveConsole.h>
#include <time.h>

extern void _cli();
extern void _sti();
extern void _loadProcess(uint64_t rsp);

#define ACTIVE 0
#define READY 1
#define BLOCK 2

typedef struct dequeueNode{
    uint8_t pid;
    uint8_t state;
    uint64_t stackPointer;
    struct dequeueNode * next;
}dequeueNode;

typedef struct{
    dequeueNode * first;
    dequeueNode * last;
    uint32_t size;
}dequeue;

/*
  Unica cola con lista de prioridades
*/
static int first=1;
static dequeue * priorityQueue=0;
static int empty = 1;

void initializeScheduler(){
  priorityQueue=(dequeue *)allocate(sizeof(dequeue));
  priorityQueue->first=0;
  priorityQueue->last=0;
  priorityQueue->size=0;
}

void printLista(){ // desp sacarlo
  ncPrintDec(priorityQueue->size);
  dequeueNode * current=priorityQueue->first;
  while(current!=0){
    ncPrintHex((uint64_t)current);
    current=current->next;
  }
  ncNewline();
  ncNewline();
}

/*Recursive function*/
dequeueNode* removeFromDequeueRec(dequeueNode* node, int pid, int quantity){
  if(quantity==0)
    return node;
  if(node->next==0 && node->pid!=pid){
    priorityQueue->last=node;
    return node;
  }
  if((node->pid)==pid){
    dequeueNode * to_return=node->next;
    free((uint64_t)node);
    quantity--;
    (priorityQueue->size)--;
    return removeFromDequeueRec(to_return, pid, quantity);
  }
  node->next=removeFromDequeueRec(node->next, pid, quantity);
  return node;
}

/*Wrapper function*/
int removeProcess(int priority, uint8_t pid) {

  if(priorityQueue->first==0)
    return 0;
  if(priorityQueue->first==priorityQueue->last){
    if(priorityQueue->first->pid==pid){
      free((uint64_t)priorityQueue->first);
      priorityQueue->first=0;
      priorityQueue->last=0;
      priorityQueue->size=0;
      return 1;
    }
    return 0;
  }
  priorityQueue->first = removeFromDequeueRec(priorityQueue->first,pid,3-priority);
  return 0;
}

void addToRoundRobin(dequeueNode * dNode){

  if(priorityQueue->first==0){
    priorityQueue->first=dNode;
    priorityQueue->last=dNode;
  }
  else{
    (priorityQueue->last)->next=dNode;
    priorityQueue->last=dNode;
  }
  (priorityQueue->size)++;
}

void addProcessToScheduler(int priority, uint8_t pid, uint64_t rsp){
  for(int i=priority; i<3; i++){
    dequeueNode * dNode=(dequeueNode *)allocate(sizeof(dequeueNode));
    dNode->pid=pid;
    dNode->state=ACTIVE;
    dNode->stackPointer=rsp;
    dNode->next=0;
    addToRoundRobin(dNode);
  }
  empty=0;
}

uint64_t getStackPointer(){
  return (priorityQueue->first)->stackPointer;
}

void loadNext(){
  _cli();

  if(!(priorityQueue->first==priorityQueue->last)){
    do{
      (priorityQueue->last)->next=priorityQueue->first;
      priorityQueue->last=(priorityQueue->last)->next;
      priorityQueue->first=(priorityQueue->first)->next;
      (priorityQueue->last)->next=0;
    }while((priorityQueue->first)->state==BLOCK);
  }

  ncPrint("user@localhost > ");
  _loadProcess(priorityQueue->first->stackPointer);

  _sti();
}

uint64_t contextSwitching(uint64_t rsp) {
  if(empty){
    return rsp;
  }
  if(first){
    first=0;
    return (priorityQueue->first)->stackPointer;
  }
  timer_handler();

  (priorityQueue->first)->stackPointer=rsp;  // Guardo en nodo el nuevo SP del P1

  if(!(priorityQueue->first==priorityQueue->last)){
    _cli();
    do{
      (priorityQueue->last)->next=priorityQueue->first;
      priorityQueue->last=(priorityQueue->last)->next;
      priorityQueue->first=(priorityQueue->first)->next;
      (priorityQueue->last)->next=0;
    }while((priorityQueue->first)->state==BLOCK);
    _sti();
  }

  return (priorityQueue->first)->stackPointer;

}

int blockProcess(dequeueNode * current, uint8_t pid){
  if(current==NULL)
    return 0;
  if(current->pid==pid){
    current->state=BLOCK;
    return 1;
  }
  blockProcess(current->next, pid);
  return 0;
}

int unblockProcess(dequeueNode *current, uint8_t pid){
  if(current==NULL)
    return 0;
  if(current->pid==pid){
    current->state=ACTIVE;   // We are going to share mutex just among 2 processes. If not, this should be READY.
    return 1;
  }
  unblockProcess(current->next, pid);
  return 0;
}

int blockedState(uint8_t pid){
  return blockProcess(priorityQueue->first, pid);
}

int unblockedState(uint8_t pid){
  return unblockProcess(priorityQueue->first, pid);
}

int isBlocked(uint8_t pid){
  dequeueNode * current=priorityQueue->first;

  while(current!=0 && current->pid!=pid){
    current=current->next;
  }
  if(current!=0)
    return (current->state)==BLOCK;
  
  return ACTIVE;
}
