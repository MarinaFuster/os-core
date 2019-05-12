#include <stdint.h>
#include <stdio.h> // We just use it for size of
#include "memoryManager.h"

#define MESSAGE_LENGTH 15 // Small size so it is easy to test

typedef struct pipeNode{
    uint8_t id;
    uint64_t address; // Of the actual shared memory
    uint8_t line; 
    struct pipeNode * next;
}pipeNode;

static pipeNode * first=0;

void addPipe(pipeNode * newNode){ // Always adds at the beginning
    newNode->next=first;
    first=newNode;
}

pipeNode * deletePipeRec(uint8_t id, pipeNode * current){
    if(current==0)
        return current;
    if(current->id==id){
        pipeNode * aux=current->next;
        free((uint64_t)current->address);
        free((uint64_t)current);
        return aux;
    }
    current->next=deletePipeRec(id,current->next);
    return current;
}

// Shared memory blocks are fixed size
uint64_t pipeCreate(uint8_t id){
    
    uint64_t address=allocate(4096);
    pipeNode * newNode=(pipeNode *)allocate(sizeof(*newNode));
    newNode->id=id;
    newNode->address=address;
    newNode->next=0;
    newNode->line=0;
    addPipe(newNode);
    return address;

}

uint64_t pipeOpen(uint8_t id){
    pipeNode * current=first;
    while(current!=0){
        if(current->id==id)
            return current->address;
        current=current->next;
    }
    return 0;
}

void pipeClose(uint8_t id){
    first=deletePipeRec(id,first);
}

void writeIntoPipe(uint8_t id, char message[MESSAGE_LENGTH]){
    /*
    pipeNode * current=first;
    while(current->id!=id)
        current=current->next;
    char * cursor=((char *)current->address+(MESSAGE_LENGTH*(current->line)));
    for(int i=0; i<MESSAGE_LENGTH; i++){
        *(cursor+i)=message[i];
    }
    current->line++;
    */
}

// You can read up to 10 lines
void readFromPipe(uint8_t id, char buffer[MESSAGE_LENGTH*10]){
    /*
    pipeNode * current=first;
    while(current->id!=id)
        current=current->next;
    char * cursor=(char *)(current->address);
    for(int i=0; i<MESSAGE_LENGTH*(current->line);i++){
        buffer[i]=cursor[i];
    }
    */
}
