#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>
#include "interrupts.h"

#define MAX_PROCESSES_QTY 60

#define STDIN 0
#define STDOUT 1

extern uint64_t printValuesFromStack(uint64_t pointer); // THIS MUST BE REMOVED
extern uint64_t buildStack(uint64_t stackStartingPoint,uint64_t wrapperFunction, uint64_t functionPointer, uint64_t pid, uint64_t priority);
extern void _loadProcess(uint64_t rsp);
extern void _hlt();
extern void _cli();
extern void _sti();

typedef struct processListNode{
    char * description;
    uint8_t pid;
    uint8_t standardIO[2];
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
    ncPrint("       ");
    ncPrintDec(current->standardIO[STDIN]);
    ncPrint("         ");
    ncPrintDec(current->standardIO[STDOUT]);
    ncPrint("          ");
    ncPrintDec(current->priority);
    ncPrint("      ");
    uint8_t state=getState(current->pid);
    if(state==ACTIVE)
      ncPrint("Active    ");
    else if(state==READY)
      ncPrint("Ready     ");
    else if(state==BLOCK)
      ncPrint("Blocked   ");
    else
      ncPrint("Unknown state ");
    ncPrint("");
    ncPrint(current->description);
    ncNewline();
    current=(current->next);
  }
}

void ps(){
  ncNewline();
  ncPrint("PID   ");
  ncPrint("STDIN   ");
  ncPrint("STDOUT  ");
  ncPrint("   Priority");
  ncPrint("   State");
  ncPrint("     Description");
  ncNewline();
  ncPrint("----------------------------------------------------------");
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

processListNode * newProcessNode(uint8_t pid, char * desc, uint64_t address, int priority, uint8_t stdin, uint8_t stdout){
  processListNode * newProcess=(processListNode *)allocate(sizeof(*newProcess));
  newProcess->pid=pid;
  newProcess->description=desc;
  newProcess->memoryBlock=address;
  newProcess->priority=priority;
  newProcess->standardIO[STDIN]=stdin;
  newProcess->standardIO[STDOUT]=stdout;
  return newProcess;
}

/* Arguments:
    - description of the process
    - priority of the process
    - functionPointer in order to build the new stack with the entry_point stored there
    - filedToRedirect is an indicator of standardIO file descriptor that I need to redirect
      0 -> redirects standard input
      1 -> redirects standard output
      2 -> does not redirect
    - newFiled is the new file descriptor that will replace some of the above
*/
uint8_t
createProcessWithPriority(char * description, int priority,  uint64_t functionPointer, uint8_t filedToRedirect, uint8_t newFiled){
  if(processID==MAX_PROCESSES_QTY){
      ncPrint("You cannot run more processes");
      ncNewline();
      return 0;
  }
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(PAGE_SIZE)); // PAGE_SIZE equals to stack size (our decision)
  uint8_t pid=processID++;
  uint8_t stdin=0, stdout=1;

  if(filedToRedirect==STDIN)
      stdin=newFiled;
  else if(filedToRedirect==STDOUT)
      stdout=newFiled;

  processListNode * newProcess=newProcessNode(pid, description, memoryBlock, priority, stdin, stdout);

  addToRegister(newProcess);
  uint64_t rsp=buildStack(memoryBlock+PAGE_SIZE, (uint64_t)wrapperFunction, (uint64_t)functionPointer, (uint64_t)newProcess->pid, (uint64_t)priority);
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

void exitProcess(uint8_t pid){
  processListNode * process=getProcess(pid);
  removeFromRegister(pid);
  removeProcess(pid, process->priority);
}

int equalstrings(const char * s1, const char * s2){
  int i=0;
  for(; s1[i]!='\0' && s2[i]!='\0'; i++){
    if(s1[i]!=s2[i])
      return 0; // False
  }
  if(s1[i]=='\0' && s2[i]=='\0');
    return 1; // True
  return 0; // False
}

uint8_t getPID(char * description){
  processListNode * current=processRegister->first;
  while(current!=0){
    if(equalstrings((current->description),description)==1) // True
      return current->pid;
    current=current->next;
  }
  return 0;
}

/* Tested !
  This function is meant to test if the virgin stack was created correctly
*/
void
testStackBuilder(uint64_t functionPointer, uint8_t pid, int priority){
  uint64_t memoryBlock=(uint64_t)allocate(sizeof(PAGE_SIZE));
  buildStack(memoryBlock+PAGE_SIZE, (uint64_t)wrapperFunction, (uint64_t)functionPointer, (uint64_t)pid, (uint64_t)priority);
  for(int i=1; i<21; i++){
    ncPrintHex(printValuesFromStack(memoryBlock+PAGE_SIZE-8*i));
    ncNewline();
  }
}

uint8_t getProcessSTDIN(uint8_t pid){
  return (getProcess(pid))->standardIO[STDIN];
}

uint8_t getProcessSTDOUT(uint8_t pid){
  return (getProcess(pid))->standardIO[STDOUT];
}

void setProcessSTDIN(uint8_t pid, uint8_t stdin_filed){
  processListNode * process=getProcess(pid);
  process->standardIO[STDIN]=stdin_filed;
}

void setProcessSTDOUT(uint8_t pid, uint8_t stdout_filed){
  processListNode * process=getProcess(pid);
  process->standardIO[STDOUT]=stdout_filed;
}

void upgradePriority(uint8_t pid){
  processListNode * node = getProcess((uint8_t)pid);
  if((node->priority)==0)
    return;
  upgradePriorityFromScheduler(pid);
  (node->priority)--;
}
void downgradePriority(uint8_t pid){
  processListNode * node = getProcess((uint8_t)pid);
  if((node->priority)==2)
    return;
  downgradePriorityFromScheduler(pid,node->priority);
  (node->priority)++;
}
