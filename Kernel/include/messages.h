#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#define MESSAGE_LENGTH 15

uint8_t * pipeCreate(uint8_t id);
void pipeClose(uint8_t id);
void writeIntoPipe(uint8_t filed, char message[MESSAGE_LENGTH],uint8_t otherPID, uint64_t size);
void readFromPipe(uint8_t filed, char buffer[MESSAGE_LENGTH*10], uint8_t callingPID, uint64_t size);

#endif