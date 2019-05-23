#include "include/stdlib.h"
#include "include/commands.h"
#define NULL 0


extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void testMemory(){

    void * bigger_than_memory_block=malloc(0x6000000);
    if(bigger_than_memory_block==NULL)
        printf("Test 1... OK\n");
    else
        printf("Test failed: bigger memory block allowed\n");

    void * null_memory_block = malloc(0);
    if(null_memory_block==NULL)
        printf("Test 2... OK\n");
    else 
        printf("Test failed: pointer assigned to 0 size memory block\n");

    void * almost_all_memory_block=malloc(0x5F00000);
    if(almost_all_memory_block!=(void*)0x0)
        printf("Test 3... OK\n");
    else
        printf("Test failed: should be able to fit into the memory block\n");
    void * exceeds_memory_block=malloc(0x610000);
    if(exceeds_memory_block==0)
        printf("Test 4... OK\n");
    else
        printf("Test failed: there is no enough space. Memory block should not be assigned!\n");
    free(almost_all_memory_block);

    void * first_block=malloc(0x2000);
    void * reference_address=first_block;

    if(first_block!=(void*)0x0)
        printf("Test 5... OK\n");
    else
        printf("Test failed: first block should fit into the memory block\n");
    void * second_block=malloc(0x2000);
    if(second_block==(void*)(reference_address+0x2000))
        printf("Test 6... OK\n");
    else
        printf("Wrong pointer assignment for second block\n");
    void * third_block=malloc(0x2000);
    if(third_block==(void*)(reference_address+0x4000))
        printf("Test 7... OK\n");
    else
        printf("Wrong pointer assignment for third block\n");
    void * fourth_block=malloc(0x2000);
    if(fourth_block==(void*)(reference_address+0x6000))
        printf("Test 8... OK\n");
    else
        printf("Wrong pointer assignment for fourth block\n");
    
    free(second_block);
    void * meant_to_be_after_fourth_block=malloc(0x27FF);
    if(meant_to_be_after_fourth_block==(void *)(reference_address+0x8000))
        printf("Test 9... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be after fourth block\n");
    void * meant_to_be_after_first_block=malloc(0x1000);
    if(meant_to_be_after_first_block==(void *)(reference_address+0x2000))
        printf("Test 10... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be after first block\n");
    
    free(third_block);
    void * meant_to_be_perfect_fit=malloc(0x1000);
    if(meant_to_be_perfect_fit==(void *)(reference_address+0x3000))
        printf("Test 11... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be perfect fit\n");

    free(first_block);
    free(meant_to_be_after_first_block);
    free(meant_to_be_perfect_fit);
    free(fourth_block);
    free(meant_to_be_after_fourth_block);   

}

void testSharedMemory(){
    uint64_t shm=shmCreate(1);
    uint64_t shm_b=shmOpen(1);
    uint64_t shm_c=shmCreate(1);

    if(shm==shm_b)
        printf("Test 1... OK\n");
    if(shm==shm_c)
        printf("Test 2... OK\n");

    char * buffer=(char *)shm;
    *buffer='x';
    *(buffer+1)='y';
    *(buffer+2)='z';
    *(buffer+3)='\0';
    printf(buffer);
    printf("   ");
    
    uint64_t second_shm=shmOpen(1);
    char * second_buffer=(char *)second_shm;
    *second_buffer='a';
    *(second_buffer+1)='b';
    *(second_buffer+2)='c';
    printf(buffer);
    printf("\n");

    shmClose(1);
}

void testProcessA(){
    for(int i=0;i<500;i++){
        int j=0;
        while(j<500000)
            j++;
        printf(" a ");
    }
}

void testProcessB(){
    for(int i=0;i<500;i++){
        int j=0;
        while(j<500000)
            j++;
        printf(" b ");
    }
}

void testMutexC(){
    uint8_t pid=0;
    getPID("testmutexc",&pid);
    uint8_t mutex=0;
    initMutex(&mutex,pid);
    printf("%d\n",mutex);
    uint64_t shm=shmCreate(2);
    int * number=(int *)shm;
    *number=0;
    for(int i=0;i<1000;i++){
        int j=0;
        while(j<5000000)
            j++;
        
        mutexLock(mutex,pid); // mutexID -- callingPID
        (*number)++;
        mutexUnlock(mutex); // mutexID 
    }
    printf("Done with test mutex C!\n");
}

void testMutexD(){
    uint8_t pid=0;
    getPID("testmutexd",&pid);
    uint64_t shm=shmOpen(2);
    int * number=(int *)shm;
    //connect to mutex (mutedID = 0)
    connectMutex(0, pid);
    for(int i=0;i<1000;i++){
        int j=0;
        while(j<5000000)
            j++;

        // MutexID==1 just testing with one mutex    
        mutexLock(0,pid); // mutexID -- callingPID
        (*number)++;
        mutexUnlock(0); // mutexID 
    }
    printf("Done with test mutex D!\n");

    destroyMutex(0); // mutex==1
}

void testMutexE(){
    uint64_t shm=shmOpen(2);
    int * number=(int *)shm;
    if((*number)==2000)
        printf("Sum was done perfectly!\n");
    else
        printf("Syncronization not working\n");
}

void testBlock(){
    uint8_t pid=0;
    getPID("testprocessa",&pid);
    if(pid==0)
        printf("Process a is not running!!\n");
    else{
        block(pid);
        printf("Process blocked!\n");
    }
}

void testUnblock(){
    uint8_t pid=0;
    getPID("testprocessa",&pid);
    if(pid==0)
        printf("Process a is not running!!\n");
    else{
        unblock(pid);
        printf("Process unblocked!\n");
    }
}

// For now it would be used to test that things are working fine
void testPipeF(){
    printf("Starting test!...\n");

    uint8_t filed=0;
    createPipe(1,&filed);
    openPipe(1,&filed);
    closePipe(1);

    createPipe(1,&filed); // This is supposed to have filed=2
    if(filed==2)
        printf("Test 1 OK...\n");

    char messageBuffer[15]={0};

    for(int k=0;k<5;k++){
        int i=0;    
        for(;i<14;i++){
            messageBuffer[i]='a'+k;
        }
        messageBuffer[i]='\n';
        int j=0;
        while(j<500000000)
            j++;
        write(filed,messageBuffer,15,3); // Attention to PIDs
        printf("Just finished writing\n");
    }
    printf("Done\n");

}

void testPipeG(){

    uint8_t filed=0;
    openPipe(1,&filed);
     if(filed==2)
        printf("Test 1 OK...\n");

    char readingBuffer[150]={0};
    int i=0;
    while(i<5){
        read(filed,readingBuffer,150,3); // Attention to PIDs !! 
        printf("\nBuffer result is... ");
        printf(readingBuffer);
        i++;
    }
    printf("\nPerfect!\n");
    closePipe(1);

}