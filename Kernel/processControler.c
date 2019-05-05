#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>

#define MAX_PROCESSES_QTY 50

extern uint64_t printValuesFromStack(uint64_t pointer); // THIS MUST BE REMOVED

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
createProcessWithPriority(char * description,int priority,  uint64_t functionPointer){
  processListNode * newProcess=(processListNode *)allocate(sizeof(*newProcess));
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET)); // Offset equals to stack size
  newProcess->description=description;
  if(processID==MAX_PROCESSES_QTY){
      ncPrint("You cannot run more processes");
      free((uint64_t)memoryBlock);
      free((uint64_t)newProcess);
  }
  newProcess->pid=processID++;
  newProcess->memoryBlock=memoryBlock;
  newProcess->priority=priority;
  addToRegister(newProcess);
  //buildStack(memoryBlock+OFFSET, functionPointer); // memoryBlock+OFFSET represents the beginning of the stack
  //addProcessToScheduler(priority, newProcess->pid, newProcess->memoryBlock);
}


// THIS MUST BE REMOVED 
void
testStackBuilder(uint64_t functionPointer){
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET));
  buildStack(memoryBlock+OFFSET, functionPointer);
  //ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-0x008));
  ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-64));
  ncNewline();
  ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-128));
  ncNewline();
  ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-192));
  ncNewline();
  ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-256));
  ncNewline();
  //uint64_t aux = *(memoryBlock+OFFSET-0x008); 
  //ncPrintHex(aux);
  //ncPrintHex(*(memoryBlock+OFFSET-0x0012));
  //ncPrinthex(*(memoryBlock+OFFSET-0x0016));
}
