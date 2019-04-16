#include <stdint.h>

#define MEMORY_SIZE 0x5F5800 
#define OFFSET 0x400
/* 6102 pages */

#define UNSUCCESSFUL -1
#define SUCCESSFUL 1

static void * const memoryStartingPoint = (void*)0x900000;

typedef struct{
    uint8_t occupied;
    uint8_t isBeggining;
    uint64_t size;
}memoryDescriptor;

// Preguntar el lunes por la direccion de memoria que tira
// Compararla con la direccion de memoria si inicializamos en 1
// Chequear si la zona donde se inicializa es zona que este pisando algun binario
static memoryDescriptor memoryBlock[MEMORY_SIZE/OFFSET]={0};

int
bestFitAlgorithm(int blocks){

    int bestPossibleIndex=-1;
    int bestSize=0;
    int currentPossibleSize=0;
    int i;
    
    for(i=0; i<MEMORY_SIZE/OFFSET; i++){

        if(currentPossibleSize!=0){
            if(!memoryBlock[i].occupied)
                currentPossibleSize+=1;
            else{
                if(currentPossibleSize==blocks){ /* I cant find a better fit */
                    return i;
                }
                if(currentPossibleSize>blocks && currentPossibleSize < bestSize){
                        bestSize=currentPossibleSize;
                        bestPossibleIndex=i;
                }
                currentPossibleSize=0;
            }
        }

        if(!memoryBlock[i].occupied)
            currentPossibleSize+=1;    
    }

    return bestPossibleIndex;
}

/* Returns null if memory cannot be allocated, else returns memory address */
uint64_t
allocate(uint64_t size){

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

    return memoryStartingPoint+(index*OFFSET);

}

/* Returns -1 if free process fails */
uint64_t
free(uint64_t pointer){
    uint64_t index = (pointer-(uint64_t)memoryStartingPoint)/OFFSET;

    if(!memoryBlock[index].isBeggining)
        return UNSUCCESSFUL;

    for(int i=0; i<memoryBlock[index].size; i++){
        memoryBlock[index+i].occupied=0;
        memoryBlock[index+i].isBeggining=0;
        memoryBlock[index+i].size=0;
    }
    return SUCCESSFUL;
}