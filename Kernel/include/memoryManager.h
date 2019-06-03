#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

uint64_t allocate(uint64_t size);
uint64_t free(uint64_t pointer);
void initializeBuddyMemory();
void printOccupiedMemory();

#define PAGE_SIZE 0x1000

#endif