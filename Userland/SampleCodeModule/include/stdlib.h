#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

// Arquitecture 2018 functions
void write(int filed, char * buffer, int size, uint8_t otherPID);
void read(int filed, char * buffer, int size, uint8_t callingPID);

char getChar();
void intToString(int n,char * str);
void printf(const char * str, ...);
void putChar(char c);
int strcmp(const char * str1, const char * str2);
int strlen(const char * string);
void strncpy(char * dest, const char * source, int n);

// Process Management
void exec(char * description, int priority, uint64_t functionPointer, uint8_t * pid, uint8_t filedToRedirect);
void getPID(char * description, uint8_t * pid);

// Memory Management
void * malloc(uint64_t size);
void free(void * pointer);

// Shared memory
uint64_t shmCreate(uint8_t id);
uint64_t shmOpen(uint8_t id);
void shmClose(uint8_t id);
void block(uint8_t pid);
void unblock(uint8_t pid);

// Mutexes
void initMutex(uint8_t * mutexID, uint8_t pid);
void destroyMutex(uint8_t mutexID);
void connectMutex(uint8_t mutexID,uint8_t pid);
void mutexLock(uint8_t mutexID, uint8_t callingPID);
void mutexUnlock(uint8_t mutexID);
void checkSides(uint64_t mutexID, uint8_t callingPID);
void changeState(uint64_t mutexID, uint8_t callingPID, uint8_t state);
void mutexRemove(uint64_t mutexID, uint8_t callingPID);
void circleTesting(uint64_t mutexID);

// Messages
void createPipe(uint8_t id, uint8_t * filed);
void openPipe(uint8_t id, uint8_t * filed);
void closePipe(uint8_t id);

#endif
