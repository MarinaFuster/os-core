#include <stdint.h>
#include <naiveConsole.h>
#include <processController.h>
static int processID=1;

// Hay que chequear que no haya problemas con esta inicializacion
// Asi como con otras. Revisar codigo!
static processList * processRegister=0;

// ESTA FUNCION TIENE QUE DESAPARECER UNA VEZ TERMINADO TODO!
void
function_just_meant_to_remove_process_controler_warnings(){
    processRegister=0;
    return;
}

void ps (){
  if (processRegister->first==0){
    ncPrint("There is no process running");
    ncNewline();
  }
  else {
    ncPrint("Process description");
    ncTab();
    ncPrint("PID");
    ncNewline();
    recursive_ps(processRegister->first);
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
  recursive_ps(node->next);
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
createProcess(uint64_t size,char * description){
	return createProcessWithPriority(size,description, 2);
}

int
createProcessWithPriority(uint64_t size,char * description,int priority){
	processDescriptor pd;
	processListNode pnode;
	pd.pid=processID++;
	pd.address=allocate(size);
	pnode.process=pd;
	if(processRegister==0){
		pnode.next=0;
		processRegister.first=pnode;
		processRegister.size=1;
	}else{+
		pnode.next=processRegister.first;
		processRegister.first=pnode;
		processRegister.size++;
	}
	addProcessToScheduler(processID-1, priority,description);

	return processID-1;
}