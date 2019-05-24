#include <stdint.h>
#include <stdio.h> // We just use it for size of
#include <naiveConsole.h>
#include "memoryManager.h"
#include "scheduler.h"
#include "inputBuffer.h"

#define MAX_FILE_DESCRIPTOR 20
#define MESSAGE_LENGTH 15 // Small size so it is easy to test
#define READ 0
#define WRITE 1

#define TAB '\t'
#define ENTER '\n'
#define DELETE '\b'

typedef struct pipeNode{
    uint8_t fileDESC[2]; // Automatic assignment done by the OS
    uint8_t id;         // Number that the programmer sets to identify it
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

pipeNode * pipeOpen(uint8_t id){
    pipeNode * current=first;
    while(current!=0){
        if(current->id==id)
            return current;
        current=current->next;
    }
    return 0;
}

int getMinimumFileDescriptor(){
    pipeNode * current=first;
    int keepGoing=1;
    for(int i=2; i<MAX_FILE_DESCRIPTOR; i++){
        while(current!=0 && keepGoing){
            if(current->fileDESC[READ]==i || current->fileDESC[WRITE]==i){
                current=first;
                keepGoing=0;
            }
        }
        if(keepGoing==1)
            return i;
    }
    return -1; // No more pipes allowed
}


// Shared memory blocks are fixed size
uint8_t * pipeCreate(uint8_t id){

    pipeNode * pipe=pipeOpen(id);
    if(pipe)
        return pipe->fileDESC;

    uint8_t readFD=getMinimumFileDescriptor();
    uint8_t writeFD=getMinimumFileDescriptor();
    if(readFD==-1 || writeFD==-1)
        return 0; // There are not enough file descriptors for a new pipe
    
    uint64_t address=allocate(2*PAGE_SIZE); // One page for a pipe
    pipeNode * newNode=(pipeNode *)allocate(sizeof(*newNode));
    newNode->id=id;
    newNode->fileDESC[READ]=readFD;
    newNode->fileDESC[WRITE]=writeFD;
    newNode->address=address;
    newNode->next=0;
    newNode->line=0;
    addPipe(newNode);

    return newNode->fileDESC;

}

void pipeClose(uint8_t id){
    first=deletePipeRec(id,first);
}

void writeIntoPipe(uint8_t filed, char * message,uint8_t otherPID, uint64_t size){

    // STDOUT
    if(filed==1){
          for(int i=0;i<size;i++){
            char c=((char *)message)[i];
            if(c==ENTER)
              ncNewline();
            else if(c==TAB)
              ncTab();
            else if(c==DELETE)
              ncDelete();
            else
              ncPrintChar(c);
          }
          return;
    }
  
    pipeNode * current=first;
    while(current!=0 && current->fileDESC[WRITE]!=filed)
        current=current->next;

    if(current==0)
        return;

    char * cursor=((char *)current->address+(MESSAGE_LENGTH*(current->line)));

    for(int i=0; i<MESSAGE_LENGTH; i++){
        cursor[i]=message[i];
    }
    current->line++;
    unblockedState(otherPID); // Unblocks the process that is waiting for reading
    
}

// You can read up to 10 lines
void readFromPipe(uint8_t filed, char * buffer, uint8_t callingPID, uint64_t size){

    // STDIN
    if(filed==0){
        readFromInputBuffer(size,(char *)buffer);
        return;
    }
    
    pipeNode * current=first;
    while(current!=0 && current->fileDESC[READ]!=filed)
        current=current->next;

    if(current==0)
        return;

    char * cursor=(char *)(current->address);
    
    if(current->line==0){ // There is nothing to read from the pipe
        blockedState(callingPID); // It blocks itself waiting for other process to write something
        while(isBlocked((callingPID))){ //This is meant to wait untile the timer tick interruption
            ;
        }
    }

    for(int i=0; i<MESSAGE_LENGTH*(current->line);i++){
        buffer[i]=cursor[i];
        cursor[i]=0; // Empty
    }
    current->line=0;
}
