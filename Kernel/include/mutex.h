#ifndef __MUTEX__
#define __MUTEX__

#define LOCKED 1
#define UNLOCKED 0

uint8_t initMutex();
void destroyMutex(uint8_t mutexID);
uint8_t mutexUnlock(int mutexID, uint8_t callingPID);
uint8_t mutexLock(uint8_t mutexID, uint8_t callingPID);

#endif
