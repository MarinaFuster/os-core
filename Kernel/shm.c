#include <stdint.h>
#include <stdio.h> // We just use it for size of
#include "memoryManager.h"

/*
Methods for creating, opening and closin shared memory.
Intead of accessing to an already created piece of memory by name,
we will do so by a number (like a PID of the shared memory).
*/

typedef struct shmNode{
    uint8_t id;
    uint64_t address; // Of the actual shared memory 
    struct shmNode * next;
}shmNode;

static shmNode * first=0;

void
addShm(shmNode * newNode){ // Always adds at the beginning
    newNode->next=first;
    first=newNode;
}

shmNode * deleteShmRec(uint8_t id, shmNode * current){
    if(current==0)
        return current;
    if(current->id==id){
        shmNode * aux=current->next;
        free(current->address);
        free((uint64_t)current);
        return aux;
    }
    current->next=deleteShmRec(id,current->next);
    return current;
}

// Shared memory blocks are fixed size
uint64_t
shmCreate(uint8_t id){

    shmNode * current=first;
    while(current!=NULL){
        if(current->id == id){
            return current->address;
        }
        current=current->next;
    }
    
    uint64_t address=allocate(4096);
    shmNode * newNode=(shmNode *)allocate(sizeof(*newNode));
    newNode->id=id;
    newNode->address=address;
    newNode->next=0;
    addShm(newNode);
    return address;
}

uint64_t
shmOpen(uint8_t id){
    shmNode * current=first;
    while(current!=0){
        if(current->id==id)
            return current->address;
        current=current->next;
    }
    return 0;
}

void
shmClose(uint8_t id){
    first=deleteShmRec(id,first);
}