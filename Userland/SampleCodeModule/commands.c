#include "include/stdlib.h"
#include "include/exceptions.h"

extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
extern void pong();

const static char * date_msg= "Date:  ";
const static char * country= "Argentina";
const static char * time_msg= "TIme:  ";

const static char * helpInstructions="\nCommands\n\ndate             : get current date\ntime             : get current time\nclear            : clear terminal screen\nplaypong         : play ping pong and have fun\nhelp             : shows commands functions\ndivzero          : script that provokes division by zero\ninvalidoperation : script that provokes an invalid operation\nexit             : exit shell\n\n\n";

void date(){
    printf(date_msg);
    int80(2,0,0,0,0,0);
    printf("   ");
    printf(country);
    printf("\n");
}
void time(){
    printf(time_msg);
    int80(1,0,0,0,0,0);
    printf("   ");
    printf(country);
    printf("\n");
}
void clear(){
    int80(5,0,0,0,0,0);
}
void playpong(){
  pong();
  clear();
}
void help(){
  printf(helpInstructions);
}
void divzero(){
  _zerodiv();
}

void invalidoperation(){
  _invalidOpcode();
}
void exit(){
    int80(5,0,0,0,0,0);
}

void test_malloc_and_free(){ /* This must be removed later ! */
    int80(10,0,0,0,0,0);
    printf("\n");
    int80(11,0,0,0,0,0);
    printf("\n");
}
