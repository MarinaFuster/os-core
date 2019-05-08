#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>
#include "interrupts.h"

#define MAX_PROCESSES_QTY 50

extern uint64_t printValuesFromStack(uint64_t pointer); // THIS MUST BE REMOVED
extern uint64_t buildStack(uint64_t stackStartingPoint,uint64_t wrapperFunction, uint64_t functionPointer, uint64_t pid, uint64_t priority);
extern void _loadProcess(uint64_t rsp);
extern void _hlt();
extern void _cli();
extern void _sti();

typedef struct processListNode{
    char* description;  
    uint8_t pid;
    int priority;
    uint64_t memoryBlock;
    struct processListNode * next;
}processListNode;

typedef struct processList{
    processListNode * first;
}processList;


static int processID=1;
static int empty=1;
static processList * processRegister=0;

void initializeProcessRegister(){
  
  processRegister=(processList *)allocate(sizeof(*processRegister));
  processRegister->first=0;
}

void psProcesses(){
  processListNode * current=(processRegister->first);
  while(current!=0){
    ncPrintDec(current->pid);
    ncPrint("     ");
    ncPrint(current->description);
    ncNewline();
    current=(current->next);
  }
}

void ps(){
  ncPrint("PID   ");
  ncPrint("Description");
  ncNewline();
  ncPrint("------------------");
  ncNewline();

  psProcesses();
}

uint8_t noProcessRunning(){
  return empty;
}

void 
addToRegister(processListNode * newProcess){
  newProcess->next=processRegister->first;
  processRegister->first=newProcess;
}

processListNode * removeFromRegisterRec(processListNode * current, uint8_t pid){
  if(current==0)
    return current;
  if(current->pid==pid){
    removeProcess(current->priority, current->pid);
    return current->next;
  }
  current->next=removeFromRegisterRec(current->next,pid);
  return current;
}

void 
removeFromRegister(uint8_t pid){
  processRegister->first=removeFromRegisterRec(processRegister->first,pid);
}

void
wrapperFunction(void(*functionPointer)(), uint8_t pid, int priority){
  functionPointer();
  _cli();
  removeFromRegister(pid);
  removeProcess(priority,pid);
  _sti();
  loadNext();
}

uint8_t
createProcessWithPriority(char * description, int priority,  uint64_t functionPointer){
  processListNode * newProcess=(processListNode *)allocate(sizeof(*newProcess));
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET)); // Offset equals to stack size
  newProcess->description=description;
  if(processID==MAX_PROCESSES_QTY){
      ncPrint("You cannot run more processes");
      free((uint64_t)memoryBlock);
      free((uint64_t)newProcess);
      return 0;
  }
  newProcess->pid=processID++;
  newProcess->memoryBlock=memoryBlock;
  newProcess->priority=priority;
  addToRegister(newProcess);
  uint64_t rsp=buildStack(memoryBlock+OFFSET, (uint64_t)wrapperFunction, (uint64_t)functionPointer, (uint64_t)newProcess->pid, (uint64_t)priority);
  addProcessToScheduler(priority, newProcess->pid, rsp);
  empty=0;
  return newProcess->pid;
}

// We know that that pid exists
processListNode *
getProcess(uint8_t pid){
  processListNode * current=processRegister->first;
  while(current!=0){
    if(current->pid==pid)
      return current;
    current=current->next;
  }
  return 0;
}

void
exitProcess(uint8_t pid){
  processListNode * process=getProcess(pid);
  removeFromRegister(pid);
  removeProcess(pid, process->priority);
}

/* Tested !
  This function is meant to test if the virgin stack was created correctly
*/
void
testStackBuilder(uint64_t functionPointer, uint8_t pid, int priority){
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET));
  buildStack(memoryBlock+OFFSET, (uint64_t)wrapperFunction, (uint64_t)functionPointer, (uint64_t)pid, (uint64_t)priority);
  for(int i=1; i<21; i++){
    ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-8*i)); 
    ncNewline();
  }
}
