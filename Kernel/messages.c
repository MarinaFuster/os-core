#include <stdint.h>
#include <stdio.h> // We just use it for size of

typedef struct pipeNode{
    uint8_t filed;
    uint64_t address; // Of the actual shared memory 
    struct pipeNode * next;
}pipeNode;

static pipeNode * first=0;

void
addPipe(pipeNode * newNode){ // Always adds at the beginning
    newNode->next=first;
    first=newNode;
}

pipeNode * deletePipeRec(uint8_t filed, pipeNode * current){
    if(current==0)
        return current;
    if(current->filed==filed){
        pipeNode * aux=current->next;
        free(current->address);
        free((uint64_t)current);
        return aux;
    }
    current->next=deletePipeRec(filed,current->next);
    return current;
}

// Shared memory blocks are fixed size
uint64_t
pipeCreate(uint8_t filed){
    
    uint64_t address=allocate(4096);
    pipeNode * newNode=(pipeNode *)allocate(sizeof(*newNode));
    newNode->filed=filed;
    newNode->address=address;
    newNode->next=0;
    addPipe(newNode);
    return address;

}

uint64_t
pipeOpen(uint8_t filed){
    pipeNode * current=first;
    while(current!=0){
        if(current->filed==filed)
            return current->address;
        current=current->next;
    }
    return 0;
}

void
pipeClose(uint8_t filed){
    first=deletePipeRec(filed,first);
}

void 
writeIntoPipe(uint8_t filed)
