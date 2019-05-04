#include <stdint.h>
#include <processController.h>
#include <scheduler.h>


// Chequeen en queue si prefieren dejar el size o ver de hacer el nodo centinela
// Me parecio mas quilombo asi que deje el size, el otro no sabia muy bien como hacerlo.
// Faltarian metodos add, remove, etc de la FIFO

/*
El indice corresponde a la prioridad: 0 (mas importante), 1, 2.
*/
static dequeue * priorityQueues[3]={0};


// ESTA FUNCION TIENE QUE DESAPARECER UNA VEZ TERMINADO TODO!
void
function_just_meant_to_remove_scheduler_warnings(){
    priorityQueues[0]=(dequeue *)0;
    return;
}

int removeProcess(int pid) {
  removeFromController(pid);
  for (int i=0; i<3; i++){
    if(removeFromDequeueRec(priorityQueues[i]->first,pid)){
      return 1;
    };
  }

  return 0;
}


int removeFromDequeueRec(dequeueNode* node, int pid){
  if (node->next==0){
    return 0;
  }

  if (( (node->next)->task.pid)==pid){
    node->next=(node->next)->next;
    return 1;
  }

  return removeFromDequeueRec(node->next, pid);
}
void addProcessToScheduler(int processId, int priority, char * desc){
    taskDescriptor td;
    td.pid=processId;
    td.time=0;
    td.state=0;
    td.description=desc;
    td.stackPointer=0;
    dequeueNode dnode;
    dnode.task=td;
    if(priorityQueues[priority-1]->size==0){
        dnode.next=0;
        priorityQueues[priority-1]->size=1;
    }else{
        dnode.next=priorityQueues[priority-1]->first;
        priorityQueues[priority-1]->size++;
    }
    priorityQueues[priority-1]->first=&dnode;
}
