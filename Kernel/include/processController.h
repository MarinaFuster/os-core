#ifndef _PROCESS_CONTROLLER_
#define _PROCESS_CONTROLLER_

#include <stdint.h>

void initializeProcessRegister();
uint8_t noProcessRunning();
void removeFromRegister(uint8_t pid);
void createProcessWithPriority(char * description,int priority,  uint64_t functionPointer);
void ps();
void testStackBuilder(uint64_t functionPointer);

#endif
