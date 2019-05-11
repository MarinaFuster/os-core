#ifndef __SHM__
#define __SHM__

#include <stdint.h>

uint64_t shmCreate(uint8_t id);
uint64_t shmOpen(uint8_t id);
void shmClose(uint8_t);

#endif
