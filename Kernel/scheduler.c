#include <stdint.h>
#include <processController.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <stdio.h>
#include <naiveConsole.h>

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
static dequeue * priorityQueue=0;


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
    dNode->state=0;
    dNode->stackPointer=rsp;
    dNode->next=0;
    addToRoundRobin(dNode);      
  }    
}

uint64_t getStackPointer(){
  return (priorityQueue->first)->stackPointer;
}

uint64_t contextSwitching(uint64_t rsp) {
  
  (priorityQueue->first)->stackPointer=rsp;  // Guardo en nodo el nuevo SP del P1

  //Reordeno priorityQueue (el proximo proceso es el nuevo first, y el anterior first ahora va al fondo)
  (priorityQueue->last)->next=priorityQueue->first;
  priorityQueue->last=(priorityQueue->last)->next;
  priorityQueue->first=(priorityQueue->first)->next;
  (priorityQueue->last)->next=0;

  return (priorityQueue->first)->stackPointer;

}
