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

  char command[2][MAX_COMMAND_LENGTH]={{0}};

  int pipeIndex=0;
  int bufferLength=0;
  int i=0;
  while(shell_buffer[i]!='\0'){
    if(shell_buffer[i]=='|'){
      pipeIndex=i;
    }
    i++;
  }
  bufferLength=i;
  
  if(pipeIndex==0){
    strncpy(command[0],shell_buffer,MAX_COMMAND_LENGTH);
  }
  else{
    strncpy(command[0],shell_buffer,pipeIndex-2);
    strncpy(command[1], shell_buffer+pipeIndex+2, bufferLength-pipeIndex-2);
  }
<<<<<<< HEAD
  
  uint8_t pid=0;
  for(int i=0; i<2; i++){
    
    if(command[i][0]!=0){ // First character is null

      uint8_t redirects=DO_NOT_REDIRECT;
      if(i==0 && pipeIndex!=0) // There was a pipe
        redirects=REDIRECT_STDOUT;
      else if(i==1)
        redirects=REDIRECT_STDIN;

      if(strcmp(command[i],DATE)){
        exec("date", LOW_PRIORITY, (uint64_t)date, &pid, redirects);
      }
      else if(strcmp(command[i],TIME)){
        exec("time", MEDIUM_PRIORITY, (uint64_t)time, &pid, redirects);
      }
      else if(strcmp(command[i],CLEAR)){
        exec("clear", LOW_PRIORITY, (uint64_t)clear, &pid, redirects);
      }
      else if(strcmp(command[i],PONG)){
        exec("pong", MEDIUM_PRIORITY, (uint64_t)pong, &pid, redirects);
      }
      else if(strcmp(command[i],PS)){
        exec("ps", LOW_PRIORITY, (uint64_t)ps, &pid, redirects);
      }
      else if(strcmp(command[i],HELP)){
        exec("help", LOW_PRIORITY, (uint64_t)help, &pid, redirects);
      }
      else if(strcmp(command[i],EXIT)){
        exec("exit", LOW_PRIORITY, (uint64_t)exit, &pid, redirects);
        running=0;
      }
      else if(strcmp(command[i],DIVZERO)){     // Exception do not count as process
        divzero();
      }
      else if(strcmp(command[i],INVALIDOPERATION)){   // Exception do not count as process
        invalidoperation();
      }
      else if(strcmp(command[i], TEST_MEMORY)){
        exec("testmemory", MEDIUM_PRIORITY, (uint64_t)testMemory, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_SHARED_MEMORY)){
        exec("testshmemory", LOW_PRIORITY, (uint64_t)testSharedMemory, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_PROCESS_A)){
        exec("testprocessa", LOW_PRIORITY, (uint64_t)testProcessA, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_PROCESS_B)){
        exec("testprcoessb", LOW_PRIORITY, (uint64_t)testProcessB, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_MUTEX_C)){
        exec("testmutexc", LOW_PRIORITY, (uint64_t)testMutexC, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_MUTEX_D)){
        exec("testmutexd", LOW_PRIORITY, (uint64_t)testMutexD, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_MUTEX_E)){
        exec("testmutexe", LOW_PRIORITY, (uint64_t)testMutexE, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_BLOCK)){
        exec("testblock", LOW_PRIORITY, (uint64_t)testBlock, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_UNBLOCK)){
        exec("testunblock", LOW_PRIORITY, (uint64_t)testUnblock, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_PIPE_F)){
        exec("testpipeF", LOW_PRIORITY, (uint64_t)testPipeF, &pid, redirects);
      }
      else if(strcmp(command[i], TEST_PIPE_G)){
        exec("testpipeG", LOW_PRIORITY, (uint64_t)testPipeG, &pid, redirects);
      }
      else if(strcmp(command,TEST_MUTEX_Z)){
        exec("testmutexz",LOW_PRIORITY,(uint64_t)testMutexZ,&pid, redirects);
      }
      else
        printf(invalidCommandMessage);
    }
    cleanBuffer(command[i]);
=======
  else if(strcmp(command, TEST_PIPE_G)){
    exec("testpipeG", LOW_PRIORITY, (uint64_t)testPipeG, &pid);
  }else if(strcmp(command,TEST_MUTEX_Z)){
    exec("testmutexz",LOW_PRIORITY,(uint64_t)testMutexZ,&pid);
>>>>>>> 5b642d798c5c00f8c10cfcd897c98c656c733398
  }
}
