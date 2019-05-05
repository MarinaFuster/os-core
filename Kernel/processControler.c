#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>

extern void buildStack(uint64_t stackStartingPoint, uint64_t functionPointer);
extern uint64_t printValuesFromStack(uint64_t pointer);

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

// Tested !
void initializeProcessRegister(){
  processRegister=(processList *)allocate(sizeof(*processRegister));
  processRegister->first=0;
  processRegister->size=0;
}

// Tested !
uint8_t noProcessRunning(){
  return processRegister->size==0;
}

// Tested !
void 
addToRegister(processListNode * newProcess){
  //(processRegister->size)++; //HUGE PROBLEM HERE
  newProcess->next=processRegister->first;
  processRegister->first=newProcess; // Adds always at the front
}

// Tested !
processListNode * removeFromRegisterRec(processListNode * current, uint8_t pid){
  if(current==0)
    return current;
  if(current->pid==pid){
    removeProcess(current->priority, current->pid);
    (processRegister->size)--;
    return current->next;
  }
  current->next=removeFromRegisterRec(current->next,pid);
  return current;
}

void 
removeFromRegister(uint8_t pid){
  processRegister->first=removeFromRegisterRec(processRegister->first,pid);
}

// In case we have reached 20 processes we need free PIDs
uint64_t
getFreeID(){
  int i=1;
  int free_pid=0;
  for(int i; i<=50 && free_pid!=1; i++){
    int keep_going=1;
    processListNode * current=processRegister->first;
    while(current!=0 && keep_going){
      if(current->pid==i){
        keep_going=0;
      }
    }
    if(keep_going)
      return free_pid;
  }
  return 0;
}

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
  ncPrintHex(printValuesFromStack(memoryBlock+OFFSET-50000));
  ncNewline();
  //uint64_t aux = *(memoryBlock+OFFSET-0x008); 
  //ncPrintHex(aux);
  //ncPrintHex(*(memoryBlock+OFFSET-0x0012));
  //ncPrinthex(*(memoryBlock+OFFSET-0x0016));
}


// Tested !
void
createProcessWithPriority(char * description, int priority, uint64_t functionPointer){

  processListNode * newProcess=(processListNode *)allocate(sizeof(*newProcess));
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(OFFSET)); // Offset equals to stack size
  newProcess->description=description;
  newProcess->pid=processID++;
  newProcess->memoryBlock=memoryBlock;
  newProcess->priority=priority;
  addToRegister(newProcess);
  //buildStack(memoryBlock+OFFSET, functionPointer); // memoryBlock+OFFSET represents the beginning of the stack
  
  //addProcessToScheduler(priority, newProcess->pid, newProcess->memoryBlock);
}

// Tested !
void registerPs(){
  processListNode * current=(processRegister->first);
  while(current!=0){
    ncPrintDec(current->pid);
    ncPrint("  ");
    ncPrint(current->description);
    ncNewline();
    current=(current->next);
  }
}

void ps(){
  if (noProcessRunning()){
    ncPrint("There are no processes running");
    ncNewline();
  }
  else {
    ncPrint("PID");
    ncPrint("   ");
    ncPrint("Process description");
    ncNewline();
    ncPrint("-------------------------------------");
    ncNewline();
    registerPs(processRegister->first);
  }
  return;
}
