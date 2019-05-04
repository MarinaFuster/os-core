#ifndef __SCHEDULER__
#define __SCHEDULER__

//int removeProcess(int pid);
//int removeFromDequeueRec(dequeueNode* node, int pid);
void initializeScheduler();
void addProcessToScheduler(int priority, uint8_t pid);
void printLista(); // desp sacarlo
int removeProcess(int priority, uint8_t pid);

#endif
