#include <stdint.h>
#include <stdio.h>  // We need this in order to use sizeof (just for that)
#include <naiveConsole.h>
#include <processController.h>
#include <memoryManager.h>
#include <scheduler.h>

typedef struct{
    char* description;
    uint8_t pid;
    struct processListNode * next;
}processListNode;

typedef struct {
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

int removeFromController(int pid){
  if (processRegister==0){
    return 0;
  }

  if((processRegister->first->process.pid)==pid){
    processRegister->first=processRegister->first->next;
    return 1;
  }

  removeFromControllerRec(processRegister->first, pid);
  return 1; //TODO: ESTO LO AGREGUE, NO SE SI ERA LA IDEA... POR AHI RETORNAR EL PID
}

int removeFromControllerRec(processListNode* node, int pid){
  if (node->next==0){
    return 0;
  }

  if ((node->next->process.pid)==pid){
    node->next=(node->next->next);
    return 1;
  }

  return removeFromControllerRec(node->next, pid);
}
int
createProcessWithPriority(uint64_t size,char * description,int priority){
  processDescriptor newProcess;
  processDescriptor * pd=&newProcess;   // Puntero a la estructura del proceso
  processListNode node;
	processListNode * pnode=&node;         // Puntero al nodo que contiene la estructura del proceso
	pd->pid=processID++;
	pd->address=allocate(size);
	pnode->process=*pd;
	if(processRegister==0){
		pnode->next=0;
		processRegister->first=pnode;
		processRegister->size=1;
	}else{
		pnode->next=processRegister->first;
		processRegister->first=pnode;
		processRegister->size++;
	}
	addProcessToScheduler(processID-1, priority,description);

	return processID-1;
}
int
createProcess(uint64_t size,char * description){
	return createProcessWithPriority(size,description, 2);
}
*/
