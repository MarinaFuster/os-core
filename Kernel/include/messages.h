#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

uint64_t pipeCreate(uint8_t id);
uint64_t pipeOpen(uint8_t id);
void pipeClose(uint8_t id);
void writeIntoPipe(uint8_t id, char message[MESSAGE_LENGTH]);
void readFromPipe(uint8_t id, char buffer[MESSAGE_LENGTH*10]);

#endif