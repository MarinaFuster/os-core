#include "naiveConsole.h"

#define SIZE 120

static char inputBuffer[SIZE]={0};

static int lastindex=0;

void addToInputBuffer(char c){
  if(lastindex==SIZE)
    return;
  inputBuffer[lastindex++]=c;
}

void removeFromInputBuffer(){
  if(lastindex!=0){
    lastindex=lastindex-1;
    inputBuffer[lastindex]=0;
  }
}

/*
Leaves the answer in dest
*/
void readFromInputBuffer(int n, char * dest){
  for(int i=0;i<n;i++){
    dest[i]=inputBuffer[i];
    inputBuffer[i]=0;
  }
  lastindex=0;
}
