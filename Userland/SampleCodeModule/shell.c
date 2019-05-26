#include <stdint.h>
#include "include/exceptions.h"
#include "include/stdlib.h"
#include "commands.h"
#include "tests.h"

// Buffer constants
#define MAX_COMMAND_LENGTH 20
#define BUFFER_SIZE 100

// Processes priority
#define LOW_PRIORITY 2
#define MEDIUM_PRIORITY 1
#define HIGH_PRIORITY 0

// For executing piped processes
#define DO_NOT_REDIRECT 2
#define REDIRECT_STDIN 0
#define REDIRECT_STDOUT 1

void execute();

static char shell_buffer[BUFFER_SIZE];

const static char * welcomeMessage="WELCOME\n\n";

const static char * invalidCommandMessage="ERROR: invalid command. Please use help to learn more about our commands\n";

const static char * user="user@localhost > ";

static int buffer_index=0;
int running=1;

void cleanBuffer(){
  for(int i=0;i<buffer_index;i++){
    shell_buffer[i]=0;
  }
  buffer_index=0;
}

void displayShell(){
    printf(welcomeMessage);
    help();
    printf(user);
    while(running){
      char c=getChar();
      if(c!=0){
        if (c!='\b'){
          putChar(c);
        }
        else{
          if (buffer_index!=0){
            putChar(c);
          }
        }
        if(c=='\n'){
          execute();
          if(!running){
              return;
          }
          cleanBuffer();
          printf(user);
        }
        else if(c=='\b'){
          if (buffer_index!=0){
            shell_buffer[--buffer_index]=0;
          }
        }
        else{
          shell_buffer[buffer_index++]=c;
        }
    }
  }
}

void execute(){
  char command[MAX_COMMAND_LENGTH];
  strncpy(command,shell_buffer,MAX_COMMAND_LENGTH);
  uint8_t pid=0;
  
  if(strcmp(command,DATE)){
    exec("date", LOW_PRIORITY, (uint64_t)date, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,TIME)){
    exec("time", MEDIUM_PRIORITY, (uint64_t)time, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,CLEAR)){
    exec("clear", LOW_PRIORITY, (uint64_t)clear, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,PONG)){
    exec("pong", MEDIUM_PRIORITY, (uint64_t)pong, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,PS)){
    exec("ps", LOW_PRIORITY, (uint64_t)ps, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,HELP)){
    exec("help", LOW_PRIORITY, (uint64_t)help, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command,EXIT)){
    exec("exit", LOW_PRIORITY, (uint64_t)exit, &pid, DO_NOT_REDIRECT);
    running=0;
  }
  else if(strcmp(command,DIVZERO)){     // Exception do not count as process
    divzero();
  }
  else if(strcmp(command,INVALIDOPERATION)){   // Exception do not count as process
    invalidoperation();
  }
  else if(strcmp(command, TEST_MEMORY)){
    exec("testmemory", MEDIUM_PRIORITY, (uint64_t)testMemory, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_SHARED_MEMORY)){
    exec("testshmemory", LOW_PRIORITY, (uint64_t)testSharedMemory, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_PROCESS_A)){
    exec("testprocessa", LOW_PRIORITY, (uint64_t)testProcessA, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_PROCESS_B)){
    exec("testprcoessb", LOW_PRIORITY, (uint64_t)testProcessB, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_MUTEX_C)){
    exec("testmutexc", LOW_PRIORITY, (uint64_t)testMutexC, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_MUTEX_D)){
    exec("testmutexd", LOW_PRIORITY, (uint64_t)testMutexD, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_MUTEX_E)){
    exec("testmutexe", LOW_PRIORITY, (uint64_t)testMutexE, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_BLOCK)){
    exec("testblock", LOW_PRIORITY, (uint64_t)testBlock, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_UNBLOCK)){
    exec("testunblock", LOW_PRIORITY, (uint64_t)testUnblock, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_PIPE_F)){
    exec("testpipeF", LOW_PRIORITY, (uint64_t)testPipeF, &pid, DO_NOT_REDIRECT);
  }
  else if(strcmp(command, TEST_PIPE_G)){
    exec("testpipeG", LOW_PRIORITY, (uint64_t)testPipeG, &pid, DO_NOT_REDIRECT);
  }
  else
    printf(invalidCommandMessage);
}
