#include <stdint.h>
#include <naiveConsole.h>

#define TRUE 1
#define FALSE 0
#define NULL 0

#define STARTING_POINT 0x900000
#define SMALLER_BLOCK 4096 // 4KB
#define PAGES 1024         // That gives us a total memory block of 4096KB
#define MAX_ORDER 10

// When I make a partition on a block I create both leafs.
// One of them is going to return an address, the other one would keep it as NULL
typedef struct treeNode{
    uint8_t order;
    uint8_t occupied;
    uint8_t first;
}memoryDescriptor;


static memoryDescriptor buddyBlock[PAGES]={{0}}; 

int powRec(int x, int y){
    if(y==1)
        return x;
    int result=powRec(x,y/2);
    return (y%2==0)?result*result:(result*result*x);
}

int power(int x, int y){
    if(x==0)
        return 0;
    if(y==0)
        return 1;
    return powRec(x,y);
}

void initializeBuddyMemory(){
    for(int i=0; i<PAGES; i++){
        buddyBlock[i].order=MAX_ORDER; // Memory block starts without partitions
    }
}


// Splits current tree node into two buddies
void createBuddies(int index){
    uint8_t prevOrder=buddyBlock[index].order;
    uint8_t newOrder=prevOrder-1;
    int blocks=power(2,prevOrder);
    for(int i=0; i<blocks; i++){
        buddyBlock[index+i].order=newOrder;
    }
}

// Should decide first the order of block I need. Tested, works good.
int blockOrderFor(uint64_t bytes){
    if(bytes>(PAGES*SMALLER_BLOCK) || bytes==0)
        return -1;
    for(int order=0;order<=MAX_ORDER;order++){
        if(bytes <= ( (int)(power(2,order))*SMALLER_BLOCK) )
            return order;  
    }
    return -1;
}

void * buddySystem(uint8_t order){
    int bestPossibleIndex=-1;
    
    int idx=0, blockOrder=0;
    for(;idx<PAGES && !(buddyBlock[idx].order==order && buddyBlock[idx].occupied==FALSE);){
        
        blockOrder=buddyBlock[idx].order;
        if(blockOrder<=order){
            // I'm sure it is occupied because if not, the for would've ended.
            idx++; // Moving onto the next one
        }
        else if(blockOrder>order){
            if(buddyBlock[idx].occupied==FALSE && (bestPossibleIndex==-1 || buddyBlock[bestPossibleIndex].order>blockOrder) ) // possible candidate
                bestPossibleIndex=idx;
            // Even if it was a good block or bad block, need to increment until the end of the block
            idx=idx+power(2,blockOrder);
        }
    }

    // It means that there was a perfect match!
    if(idx<PAGES){
        
        // Occupies needed blocks...
        buddyBlock[idx].occupied=TRUE;
        buddyBlock[idx].first=TRUE;
        for(int i=1; i<power(2,buddyBlock[idx].order); i++)
            buddyBlock[idx+i].occupied=TRUE;

        // ...and returns address
        return (void *)(uint64_t)(STARTING_POINT+(idx*SMALLER_BLOCK));
    
    }
    else if(bestPossibleIndex==-1){
        // No space available
        return NULL;
    }
    else{
        // I need to split
        while(buddyBlock[bestPossibleIndex].order>order) // I'm always keeping the left buddy
            createBuddies(bestPossibleIndex); 

        buddyBlock[bestPossibleIndex].occupied=TRUE;
        buddyBlock[bestPossibleIndex].first=TRUE;
        for(int i=1; i<power(2,order); i++){
            buddyBlock[bestPossibleIndex+i].occupied=TRUE;
        }

        // Returns address
        return (void *)(uint64_t)(STARTING_POINT+(bestPossibleIndex*SMALLER_BLOCK));
    }
}

// Tested and works fine
void * buddyMalloc(uint64_t bytes){
    int order=blockOrderFor(bytes);
    if(order==-1)
        return NULL;
    return (order==-1) ? NULL : buddySystem(order);
}

void printAllBlocks(){
    for(int i=0; i<PAGES; i++){
        ncPrint("Bloque numero ");
        ncPrintDec(i);
        ncNewline();
        ncPrint("--------------------\n");
        ncPrint("Orden ");
        ncPrintDec(buddyBlock[i].order);
        ncNewline();
        ncPrint("Esta ocupado? ");
        if(buddyBlock[i].occupied==TRUE)
            ncPrint("si\n");
        else
            ncPrint("no\n");
        ncPrint("Es el primero? ");
        if(buddyBlock[i].first==TRUE)
            ncPrint("si\n\n");
        else
            ncPrint("no\n\n");
    }
}

// Merge buddies because they are both available
void mergeBuddies(int leftBuddy, int rightBuddy, int blocks){
    uint8_t newOrder=buddyBlock[leftBuddy].order+1;
    for(int i=0;i<blocks;i++){
        buddyBlock[leftBuddy+i].order=newOrder;
        buddyBlock[rightBuddy+i].order=newOrder;
    }
    if(buddyBlock[leftBuddy].order==MAX_ORDER) // Memory fully merged 
        return;
    
    int newBlocksQty=power(2,newOrder); // I need to check if I can keep merging buddies...
    int buddy;

    // Reusing left and right buddies variables
    if((leftBuddy/newBlocksQty)%2==0){
        buddy=leftBuddy+newBlocksQty;
        rightBuddy=buddy;
    }
    else{
        rightBuddy=leftBuddy;    
        leftBuddy=leftBuddy-newBlocksQty;
        buddy=leftBuddy;
    }

    // Merges buddies recursively...
    if(buddyBlock[buddy].order==newOrder && buddyBlock[buddy].occupied==FALSE)
        mergeBuddies(leftBuddy, rightBuddy, newBlocksQty);
}

// Tested but it should be tested further than this
uint64_t buddyFree(void * address){
    if(address<(void *)STARTING_POINT || address>(void *)(STARTING_POINT+PAGES*SMALLER_BLOCK))
        return -1;

    int index=((uint64_t)address-STARTING_POINT)/SMALLER_BLOCK;
    if(buddyBlock[index].first==FALSE) // I'm trying to access somewhere in the middle
        return -1; 

    buddyBlock[index].first=FALSE;
    int blocks=power(2,buddyBlock[index].order);
    for(int i=0; i<blocks; i++)
        buddyBlock[index+i].occupied=FALSE;
    
    int buddy, rightBuddy, leftBuddy;
    // If (idx/pow(2,order))%2==0 my buddy is the one to my right, else, my buddy is the one to my left
    if((index/blocks)%2==0){
        buddy=rightBuddy=index+blocks;
        leftBuddy=index;
    }
    else{
        buddy=leftBuddy=index-blocks;
        rightBuddy=index;
    }

    // If my buddy is ready to merge...
    if(buddyBlock[buddy].occupied==FALSE && buddyBlock[buddy].order==buddyBlock[index].order)
        mergeBuddies(leftBuddy, rightBuddy, blocks);

    return 0;
}


/* Returns null if memory cannot be allocated, else returns memory address */
uint64_t
allocate(uint64_t size){
    return (uint64_t)buddyMalloc(size);
}

/* Returns -1 if free process fails */
uint64_t
free(uint64_t pointer){
    return buddyFree((void *)pointer);
}

void printOccupiedMemory(){
    for(int i=0;i<PAGES;i++){
        if(buddyBlock[i].occupied && buddyBlock[i].first){
            int blocks=power(2,buddyBlock[i].order);
            ncPrint("Bloque numero ");
            ncPrintDec(i);
            ncPrint(", orden ");
            ncPrintDec(buddyBlock[i].order);
            ncPrint(", ocupa ");
            ncPrintDec(blocks);
            if(blocks==1)
                ncPrint(" bloque.\n");
            else
                ncPrint(" bloques.\n");
        }
    }
}