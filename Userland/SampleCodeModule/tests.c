#include "include/stdlib.h"
#include "include/commands.h"
#define NULL 0
#define HUNGRY 1
#define THINKING 2
#define EATING 3

extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void testMemory(){

    void * bigger_than_memory_block=malloc(4200000);
    if(bigger_than_memory_block==NULL)
        printf("Test 1... OK\n");
    else
        printf("Test failed: bigger memory block allowed\n");

    void * null_memory_block = malloc(0);
    if(null_memory_block==NULL)
        printf("Test 2... OK\n");
    else
        printf("Test failed: pointer assigned to 0 size memory block\n");


    void * first_block=malloc(4100);
    void * second_block=malloc(2097152);
    void * third_block=malloc(16383);
    printOccupiedMemory();
    free(first_block);
    free(second_block);
    free(third_block);
    free(bigger_than_memory_block);
    free(null_memory_block);
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

void testReadFromA(){
    
    // This was the ideal...
    // but for some reason it was not returning properly
    /*char buffer[1];
    for(int i=0;i<1000;i++){
        int j=0;
        while(j<500000)
            j++;
        *buffer=getChar(); // From standard input
        printf(buffer);
    }*/

    // We want to show that testProcessA was writing on another file descriptor
    uint8_t filed[2]={0};
    openPipe(0,filed); // We are opening the OS pipe. This isn't supposed to be done here!
    
    char buffer[501]={0};
    read(2, buffer, 500, 0); // We are reading what testProcessA left on its stdout...

    printf(buffer); // Printing it...

    closePipe(0); // Closing OS pipe. We are not using it anymore
    // If we want to redirect other processes it will be reopen

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

        mutexLock(1,pid); // mutexID -- callingPID
        printf("C");
        (*number)++;
        mutexUnlock(1); // mutexID
    }
    printf("Done with test mutex C!\n");
}

void testMutexZ(){
    uint8_t pid=0;
    getPID("testmutexz",&pid);
    uint64_t shm=shmOpen(2);
    int * number=(int *)shm;
    connectMutex(1, pid);
    *number=0;
    for(int i=0;i<1000;i++){
        int j=0;
        while(j<5000000)
            j++;

        mutexLock(1,pid); // mutexID -- callingPID
        printf("Z");
        (*number)++;
        mutexUnlock(1); // mutexID
    }
    printf("Done with test mutex Z!\n");
}

void testMutexD(){
    uint8_t pid=0;
    getPID("testmutexd",&pid);
    uint64_t shm=shmOpen(2);
    int * number=(int *)shm;
    connectMutex(1, pid);
    for(int i=0;i<1000;i++){
        int j=0;
        while(j<5000000)
            j++;
        // MutexID==1 just testing with one mutex
        mutexLock(1,pid); // mutexID -- callingPID
        (*number)++;
        printf("D");
        mutexUnlock(1); // mutexID
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

    uint8_t filed[2]={0};
    createPipe(1,filed);
    openPipe(1,filed);
    closePipe(1);

    createPipe(1,filed);
    if(filed[0]==2)
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
        write(filed[1],messageBuffer,10,3); // Attention to PIDs
        printf("Just finished writing\n");
    }
    printf("Done\n");

}

void testPipeG(){

    uint8_t filed[2]={0};
    openPipe(1,filed);
     if(filed[0]==2)
        printf("Test 1 OK...\n");

    char readingBuffer[150]={0};
    int i=0;
    while(i<5){
        read(filed[0],readingBuffer,150,3); // Attention to PIDs !!
        printf("\nBuffer result is... ");
        printf(readingBuffer);
        i++;
    }
    printf("\nPerfect!\n");
    closePipe(1);

}

void takeFork(uint8_t mutexID, uint8_t pid){
  mutexLock(1, pid);
  changeState(1, pid, HUNGRY);
  //printf("Philosopher %d is hungry\n",pid );
  int j=0;
  while(j<500000000)
      j++;
  // Eat ONLY if neighbours are not eating
  checkSides(1, pid);
  mutexUnlock(1);
}

void putFork(uint8_t mutexID, uint8_t pid){
  //mutexLock(1, pid);
  changeState(1, pid, THINKING);
  //mutexUnlock(1);
  int j=0;
  while(j<500000000)
      j++;
}

void testPhi(){
  // Creo el entorno de los filosofos (mutex y mem compartida)
  uint8_t pid=0;
  getPID("testphi",&pid);
  uint8_t mutex=1;
  initMutex(&mutex,pid);
  printf("%d\n",mutex);
  mutexRemove(1, pid);
}

void phi(){
  uint8_t pid=0;
  uint8_t keepGoing=1;
  getPID("phi", &pid);
  connectMutex(1, pid);
  printf("I am philosopher %d\n",pid );
  while(keepGoing){
    takeFork(1,pid);
    int j=0;
    while(j<500000000)
        j++;
    putFork(1,pid);
    inMutexCheck(&keepGoing, pid);
  }
  quitProcess(pid);
}

void circleTest(){
   circleTesting(1);
}

void rphi(uint8_t pid){
  mutexRemove(1,pid);
}
