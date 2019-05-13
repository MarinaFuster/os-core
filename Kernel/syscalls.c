#include <stdint.h>
#include "naiveConsole.h"
#include "rtc.h"
#include "keyboard.h"
#include "inputBuffer.h"
#include "interrupts.h"
#include "video.h"
#include "time.h"
#include "beep.h"
#include "memoryManager.h"
#include "processController.h"
#include "scheduler.h"
#include "mutex.h"
#include "shm.h"
#include "messages.h"

#define EOF -1
#define TAB '\t'
#define ENTER '\n'
#define DELETE '\b'

#define SYSCALLSQTY 26
#define VALID_SYS_CODE(c) (c>=0 && c<=SYSCALLSQTY)

typedef uint64_t (*syscall) (uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static syscall syscalls[SYSCALLSQTY];

/***********************************************************************
 * GENERAL
***********************************************************************/

//No puedo deshabilitar las interrupciones realmente con lo cual, unicamente va a cerrar el shell,
//pero sigue sinedo posible escribir con el teclado
uint64_t sys_exit(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  ncClear();
  return 0;
}

uint64_t sys_read(uint64_t file, uint64_t buffer, uint64_t size, uint64_t callingPID, uint64_t r9) {

    if(file>=2)
      readFromPipe((uint8_t)file,(char *)buffer, (uint8_t)callingPID); // It is going to read up to 150 characters
    else
      readFromInputBuffer(size,(char *)buffer); // Arqui TP code

    return 0;
}

uint64_t sys_write(uint64_t file, uint64_t buffer, uint64_t size, uint64_t otherPID, uint64_t r9){
    
    if(file>=2)
      writeIntoPipe((uint8_t)file,(char *)buffer,(uint8_t)otherPID); // Fixed size in case is a pipe
    
    // Arqui TP code
    if(file==1){
          for(int i=0;i<size;i++){
            char c=((char *)buffer)[i];
            if(c==ENTER)
              ncNewline();
            else if(c==TAB)
              ncTab();
            else if(c==DELETE)
              ncDelete();
            else
              ncPrintChar(c);
          }
    }
    return 0;
}

uint64_t sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    ncClear();
    return 0;
}

uint64_t sys_time(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    char hour=_getHours();
    char min=_getMinutes();
    char sec=_getSeconds();
    ncPrintDec(hour);
    ncPrint(":");
    ncPrintDec(min);
    ncPrint(":");
    ncPrintDec(sec);
    return 0;
}

uint64_t sys_date(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  char year=_getYear();
  char month=_getMonth();
  char day=_getDayofMonth();
  ncPrintDec(month);
  ncPrint("-");
  ncPrintDec(day);
  ncPrint("-");
  ncPrintDec(year);
    return 0;
}

//x and y position. paint 0=erase figure, paint 1=paint figure
uint64_t sys_paint(uint64_t x_position, uint64_t y_position, uint64_t paint, uint64_t r8, uint64_t r9){
    if(paint==1)
      drawAFigure(x_position,y_position);
    else
      deleteAt(x_position,y_position);
    return 0;
}

uint64_t sys_ticks(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    int * ticks=(int *)rsi;
    *ticks=ticks_elapsed();
    return 0;
}

uint64_t sys_beep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  beep((uint32_t)rsi,rdx);
  return 0;
}
uint64_t sys_disable_beep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  nosound();
  return 0;
}

/***********************************************************************
 * MEMORY MANAGEMENT
***********************************************************************/

/* arguements : bytes of memory we need to allocate and pointer_address where we
   store the new address if possible (else null)
*/
uint64_t sys_malloc(uint64_t bytes, uint64_t pointer_address, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint64_t address=allocate(bytes);
  unsigned long int * pointer=(unsigned long int *)pointer_address;
  *pointer=address;
  return 0;
}

/* arguements : pointer to memory address we need to free
*/
uint64_t sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  free(rsi);
  return 0;
}

/*Creates shared memory*/
uint64_t sys_shm_create(uint64_t id, uint64_t shm, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint64_t address=shmCreate((uint8_t)id);
  uint64_t * aux=(uint64_t *)shm;
  *aux=address;
  return 0;
}

/*Gives access to an already created shared memory*/
uint64_t sys_shm_open(uint64_t id, uint64_t shm, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint64_t address=shmOpen((uint8_t)id);
  uint64_t * aux=(uint64_t *)shm;
  *aux=address;
  return 0;
}

/*Releases shared memory*/
uint64_t sys_shm_close(uint64_t id, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  shmClose(id);
  return 0;
}

/***********************************************************************
 * PROCESS MANAGEMENT
***********************************************************************/

/* arguments:
   char * description on rsi
   int priority on rdx
   uint64_t function pointer on rcx
*/
uint64_t sys_exec(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  (*(uint64_t *)r8)=(uint64_t)createProcessWithPriority((char *)rsi, (int)rdx, rcx);
  return 0;
}

uint64_t sys_exit_process(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  exitProcess(pid);
  return 0;
}

uint64_t sys_ps(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  ps();
  return 0;
}

uint64_t sys_block(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  blockedState((uint8_t)pid);
  return 0;
}

uint64_t sys_unblock(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  unblockedState((uint8_t)pid);
  return 0;
}


/***********************************************************************
 * IPC
***********************************************************************/

uint64_t sys_init_mutex(uint64_t mutexID, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  int * id=(int *)mutexID;
  *id=initMutex();
  return 0;
}

uint64_t sys_destroy_mutex(uint64_t mutexID, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  destroyMutex((uint8_t)mutexID);
  return 0;
}

uint64_t sys_mutex_lock(uint64_t mutexID, uint64_t callingPID, uint64_t rcx, uint64_t r8, uint64_t r9){
  mutexLock((uint8_t)mutexID, (uint8_t)callingPID);
  return 0;
}

uint64_t sys_mutex_unlock(uint64_t mutexID, uint64_t otherPID, uint64_t rcx, uint64_t r8, uint64_t r9){
  mutexUnlock((uint8_t)mutexID, (uint8_t)otherPID);
  return 0;
}

uint64_t sys_pipe_create(uint64_t id, uint64_t filed, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint8_t * descriptor=(uint8_t *)filed;
  *descriptor=pipeCreate(id);
  if( (*descriptor) == -1)
    ncPrint("No more pipes allowed");

  /*Testing!  
  ncNewline();
  ncPrint("Pipe de ID ");
  ncPrintDec(id);
  ncPrint(" creado y su file descriptor es ");
  ncPrintDec(*((uint8_t *)filed));
  ncNewline();*/
  return 0;
}

uint64_t sys_pipe_close(uint64_t id, uint64_t filed, uint64_t rcx, uint64_t r8, uint64_t r9){
  pipeClose(id);

  /*Testing!
  ncNewline();
  ncPrint("Pipe de ID ");
  ncPrintDec(id);
  ncPrint(" cerrado ");
  ncNewline();*/
  return 0;
}

void loadSysCalls(){
  syscalls[0]=&sys_exit;
  syscalls[1]=&sys_time;
  syscalls[2]=&sys_date;
  syscalls[3]=&sys_read;
  syscalls[4]=&sys_write;
  syscalls[5]=&sys_clear;
  syscalls[6]=&sys_paint;
  syscalls[7]=&sys_ticks;
  syscalls[8]=&sys_beep;
  syscalls[9]=&sys_disable_beep;
  syscalls[10]=&sys_malloc;
  syscalls[11]=&sys_free;
  syscalls[12]=&sys_exec;
  syscalls[13]=&sys_exit_process;
  syscalls[14]=&sys_ps;
  syscalls[15]=&sys_shm_create;
  syscalls[16]=&sys_shm_open;
  syscalls[17]=&sys_shm_close;
  syscalls[18]=&sys_block;
  syscalls[19]=&sys_unblock;
  syscalls[20]=&sys_init_mutex;
  syscalls[21]=&sys_destroy_mutex;
  syscalls[22]=&sys_mutex_lock;
  syscalls[23]=&sys_mutex_unlock;
  syscalls[24]=&sys_pipe_create;
  syscalls[25]=&sys_pipe_close;
}

void sysCallsHandler(uint64_t syscode, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){ // lega en rdi desde asm
  if(VALID_SYS_CODE(syscode)){
    (syscalls[syscode])(rsi,rdx,rcx,r8,r9);
  }
  return;
}
