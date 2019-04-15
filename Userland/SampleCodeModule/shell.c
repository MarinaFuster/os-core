#include <stdint.h>
#include "include/exceptions.h"
#include "include/stdlib.h"
#include "commands.h"

#define DATE "date"
#define TIME "time"
#define CLEAR "clear"
#define PONG "playpong"
#define HELP "help"
#define DIVZERO "divzero"
#define INVALIDOPERATION "invalidoperation"
#define EXIT "exit"

#define MAX_COMMAND_LENGTH 20
#define BUFFER_SIZE 100

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
  char * command[MAX_COMMAND_LENGTH];
  strncpy(command,shell_buffer,MAX_COMMAND_LENGTH);

  if(strcmp(command,DATE))
    date();
  else if(strcmp(command,TIME))
    time();
  else if(strcmp(command,CLEAR))
    clear();
  else if(strcmp(command,PONG))
    pong();
  else if(strcmp(command,HELP))
    help();
  else if(strcmp(command,EXIT)){
    exit();
    running=0;
  }
  else if(strcmp(command,DIVZERO)){
    divzero();
  }
  else if(strcmp(command,INVALIDOPERATION)){
    invalidoperation();
  }
  else
    printf(invalidCommandMessage);
}
