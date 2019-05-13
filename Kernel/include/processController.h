#ifndef _PROCESS_CONTROLLER_
#define _PROCESS_CONTROLLER_

#include <stdint.h>

void initializeProcessRegister();
uint8_t noProcessRunning();
void removeFromRegister(uint8_t pid);
uint8_t createProcessWithPriority(char * description,int priority,  uint64_t functionPointer);
void ps();
void exitProcess(uint8_t pid);
void testStackBuilder(uint64_t functionPointer, uint8_t pid, int priority);
uint8_t getPID(char * description);

#endif
