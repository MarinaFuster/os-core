#ifndef __SCHEDULER__
#define __SCHEDULER__

typedef struct{
    uint32_t pid;
    uint8_t state;
    uint64_t time; // Tiempo transcurrido
    uint64_t stackPointer; // No se si esto era necesario al final. Veremos en la implementacion
    char * description;
}taskDescriptor;

typedef struct dequeueNode{
    taskDescriptor task;
    struct dequeueNode * next;
}dequeueNode;

typedef struct{
    dequeueNode * first;
    uint32_t size;
}dequeue;


int removeProcess(int pid);
int removeFromDequeueRec(dequeueNode* node, int pid);
void addProcessToScheduler(int processId, int priority, char * desc);

#endif
