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

    void * almost_all_memory_block=malloc(0x5F50000);
    printf("FIRST BLOCK");
    if(almost_all_memory_block==(void*)0x905000)
        printf("Test 3... OK\n");
    else
        printf("Wrong pointer assignment: should be at the beggining of the memory block\n");
    void * exceeds_memory_block=malloc(0x10000);
    if(exceeds_memory_block==0)
        printf("Test 4... OK\n");
    else
        printf("There is no enough space. Memory block should not be assigned!\n");
    free(almost_all_memory_block);

    void * first_block=malloc(0x2000);
    if(first_block==(void*)0x905000)
        printf("Test 5... OK\n");
    else
        printf("Wrong pointer assignment for first block\n");
    void * second_block=malloc(0x2000);
    if(second_block==(void*)0x907000)
        printf("Test 6... OK\n");
    else
        printf("Wrong pointer assignment for second block\n");
    void * third_block=malloc(0x2000);
    if(third_block==(void*)0x909000)
        printf("Test 7... OK\n");
    else
        printf("Wrong pointer assignment for third block\n");
    void * fourth_block=malloc(0x2000);
    if(fourth_block==(void*)0x90B000)
        printf("Test 8... OK\n");
    else
        printf("Wrong pointer assignment for fourth block\n");
    
    free(second_block);
    void * meant_to_be_after_fourth_block=malloc(0x27FF);
    if(meant_to_be_after_fourth_block==(void *)0x90D000)
        printf("Test 9... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be after fourth block\n");
    void * meant_to_be_after_first_block=malloc(0x1000);
    if(meant_to_be_after_first_block==(void *)0x907000)
        printf("Test 10... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be after first block\n");
    
    free(third_block);
    void * meant_to_be_perfect_fit=malloc(0x1000);
    if(meant_to_be_perfect_fit==(void *)0x908000)
        printf("Test 11... OK\n");
    else
        printf("Wrong pointer assignment for block meant to be perfect fit\n");

    free(first_block);
    free(meant_to_be_after_first_block);
    free(meant_to_be_perfect_fit);
    free(fourth_block);
    free(meant_to_be_after_fourth_block);   

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

}

void testMutexD(){

}

void testMutexE(){

}