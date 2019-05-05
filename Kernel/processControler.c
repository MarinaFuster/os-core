#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>

extern void buildStack(uint64_t stackStartingPoint, uint64_t functionPointer);
typedef struct processListNode{
    char* description;
    uint8_t pid;
    int priority;
    uint64_t memoryBlock;
    struct processListNode * next;
}processListNode;

typedef struct processList{
    processListNode * first;
    uint8_t size;
}processList;


static int processID=1;
static processList * processRegister=0;

void initializeProcessRegister(){
  processRegister=(processList *)allocate(sizeof(*processRegister));
  processRegister->first=0;
  processRegister->size=0;
}

void printRegister(){
  processListNode * current=(processRegister->first);
  while(current!=0){
    ncPrintDec(current->pid);
    current=(current->next);
  }
}

uint8_t noProcessRunning(){
  return processRegister->size==0;
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
createProcessWithPriority(uint64_t size,char * description,int priority,  uint64_t functionPointer){
  processListNode * newProcess=(processListNode *)allocate(sizeof(*newProcess));
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET)); // Offset equals to stack size
  newProcess->description=description;
  newProcess->pid=processID++;
  newProcess->memoryBlock=memoryBlock;
  newProcess->priority=priority;
  addToRegister(newProcess);
  buildStack(memoryBlock+OFFSET, functionPointer); // memoryBlock+OFFSET represents the beginning of the stack
  addProcessToScheduler(priority, newProcess->pid, newProcess->memoryBlock);
}

/*


void ps(){
  if (processRegister->first==0){
    ncPrint("There is no process running");
    ncNewline();
  }
  else {
    ncPrint("Process description");
    ncTab();
    ncPrint("PID");
    ncNewline();
    recursivePs(processRegister->first);
  }

  return;
}

void recursivePs(processListNode* node){
  if (node==0){
    return;
  }

  ncPrint(node->process.description);
  ncTab();
  ncPrintDec(node->process.pid);
  ncNewline();
  recursivePs(node->next);
}

*/
