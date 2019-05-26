#ifndef _PROCESS_CONTROLLER_
#define _PROCESS_CONTROLLER_

#include <stdint.h>

uint8_t createProcessWithPriority(char * description,int priority,  uint64_t functionPointer);
void exitProcess(uint8_t pid);
uint8_t getPID(char * description);
uint8_t getProcessSTDIN(uint8_t pid);
uint8_t getProcessSTDOUT(uint8_t pid);
uint8_t noProcessRunning();
void initializeProcessRegister();
void ps();
void removeFromRegister(uint8_t pid);
void testStackBuilder(uint64_t functionPointer, uint8_t pid, int priority);

#endif
