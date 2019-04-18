#include <stdint.h>

#define MEMORY_SIZE 0x5F58000 
#define OFFSET 0x1000
/* 6102 pages */

#define UNSUCCESSFUL -1
#define SUCCESSFUL 1

static void * const memoryStartingPoint = (void*)0x900000;

typedef struct{
    uint8_t occupied;
    uint8_t isBeggining;
    uint64_t size;
}memoryDescriptor;

static memoryDescriptor memoryBlock[MEMORY_SIZE/OFFSET]={{0}};

int
bestFitAlgorithm(int blocks){

    int bestPossibleIndex=-1;
    int bestSize=MEMORY_SIZE;
    int currentPossibleSize=0;
    int currentPossibleIndex=-1;
    int i;
    
    for(i=0; i<MEMORY_SIZE/OFFSET; i++){

        if(currentPossibleSize!=0){
            if(memoryBlock[i].occupied==0)
                currentPossibleSize+=1;
            else{
                if(currentPossibleSize==blocks){ /* I cant find a better fit */
                    return currentPossibleIndex;
                }
                if(currentPossibleSize>blocks && currentPossibleSize < bestSize){
                        bestSize=currentPossibleSize;
                        bestPossibleIndex=currentPossibleIndex;
                }
                currentPossibleSize=0;
            }
        }
        else{
            if(memoryBlock[i].occupied==0){
                currentPossibleSize+=1;
                currentPossibleIndex=i;
            }
        }
    }

    if(currentPossibleSize>=blocks && bestPossibleIndex==-1)
        bestPossibleIndex=currentPossibleIndex;

    return bestPossibleIndex;
}

/* Returns null if memory cannot be allocated, else returns memory address */
uint64_t
allocate(uint64_t size){

    if(size==0)
        return 0;
    if(size>MEMORY_SIZE)
        return 0;

    int blocks = size/OFFSET;
    if(size%OFFSET != 0)
        blocks+=1;

    int index=bestFitAlgorithm(blocks);
    if(index==-1)
        return 0;

    memoryBlock[index].isBeggining=1;
    memoryBlock[index].occupied=1;
    memoryBlock[index].size=blocks;

    int i;
    for(i=1;i<blocks;i++){
        memoryBlock[index+i].occupied=1;
        memoryBlock[index+i].isBeggining=0;
        memoryBlock[index+i].size=0;
    }

    return (uint64_t)memoryStartingPoint+(index*OFFSET);

}

/* Returns -1 if free process fails */
uint64_t
free(uint64_t pointer){
    
    uint64_t index = (pointer-(uint64_t)memoryStartingPoint)/OFFSET;
    uint32_t blocks=memoryBlock[index].size;

    if(!memoryBlock[index].isBeggining)
        return UNSUCCESSFUL;
    
    for(int i=0; i<blocks; i++){
        memoryBlock[index+i].occupied=0;
        memoryBlock[index+i].isBeggining=0;
        memoryBlock[index+i].size=0;
    }
    return SUCCESSFUL;
}