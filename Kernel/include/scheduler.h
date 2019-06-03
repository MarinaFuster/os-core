#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <stdint.h>

#define ACTIVE 0
#define READY 1
#define BLOCK 2

//int removeProcess(int pid);
//int removeFromDequeueRec(dequeueNode* node, int pid);
void initializeScheduler();
void addProcessToScheduler(int priority, uint8_t pid, uint64_t memoryBlock);
void printLista(); // desp sacarlo
int removeProcess(int priority, uint8_t pid);
uint64_t contextSwitching(uint64_t rsp);
uint64_t getStackPointer();
void loadNext();

int unblockedState(uint8_t pid);
int blockedState(uint8_t pid);
int isBlocked(uint8_t pid);

uint8_t getState(uint8_t pid);
uint8_t getRunningPID();

void upgradePriorityFromScheduler(uint64_t pid);
void downgradePriorityFromScheduler(uint64_t pid,int oldPriority);

#endif
