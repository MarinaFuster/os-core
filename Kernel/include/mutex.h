#ifndef __MUTEX__
#define __MUTEX__

#define LOCKED 1
#define UNLOCKED 0

uint8_t initMutex(uint8_t callingPid);
void destroyMutex(uint8_t mutexID);
uint8_t mutexUnlock(int mutexID);
uint8_t mutexLock(uint8_t mutexID, uint8_t callingPID);
uint8_t connectToMutex(uint8_t mutexID, uint8_t callingPid);
uint8_t checkPhi(uint8_t mutexID, uint8_t pid);
uint8_t changePhiState(uint8_t mutexID,uint8_t pid, uint8_t state);
void removeFromMutex(uint8_t mutexID, uint8_t pid);
uint8_t checkCircularList(uint8_t mutexID);
uint8_t inMutex(uint8_t mutexID, uint8_t pid);

#endif
