#ifndef _PROCESS_CONTROLLER_
#define _PROCESS_CONTROLLER_

#include <stdint.h>

uint8_t createProcessWithPriority(char * description,int priority, uint64_t functionPointer, uint8_t filedToRedirect, uint8_t newFiled);

void exitProcess(uint8_t pid);

uint8_t getPID(char * description);

uint8_t noProcessRunning();
void initializeProcessRegister();
void ps();
void removeFromRegister(uint8_t pid);

uint8_t getProcessSTDIN(uint8_t pid);
uint8_t getProcessSTDOUT(uint8_t pid);
void setProcessSTDIN(uint8_t pid, uint8_t stdin_filed);
void setProcessSTDOUT(uint8_t pid, uint8_t stdout_filed);
void upgradePriority(uint8_t pid);
void downgradePriority(uint8_t pid);
void testStackBuilder(uint64_t functionPointer, uint8_t pid, int priority);

#endif
