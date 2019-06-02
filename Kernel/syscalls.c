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

#define SYSCALLSQTY 27
#define VALID_SYS_CODE(c) (c>=0 && c<=SYSCALLSQTY)

typedef uint64_t (*syscall) (uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static syscall syscalls[SYSCALLSQTY];

/***********************************************************************
 * GENERAL
***********************************************************************/

//We cannot actually CLOSE the shell, just clears the screen to simulate that
uint64_t sys_exit(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  ncClear();
  return 0;
}

uint64_t sys_read(uint64_t file, uint64_t buffer, uint64_t size, uint64_t callingPID, uint64_t r9) {
  readFromPipe((uint8_t)file,(char *)buffer, (uint8_t)callingPID, size); // It is going to read up to 150 characters
  return 0;
}

uint64_t sys_write(uint64_t file, uint64_t buffer, uint64_t size, uint64_t otherPID, uint64_t r9){
  writeIntoPipe((uint8_t)file,(char *)buffer,(uint8_t)otherPID, size); // Fixed size in case is a pipe
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

/* Arguments:
   rsi: description of the process (char *)
   rdx: priority of the process (int)
   rcx: function pointer 
   r8: where the pid is stored (uint8_t *)
   r9: indicates if the stdin/stdout must be redirected

*/
uint64_t sys_exec(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint8_t newFileDescriptor=0;
  if(r9!=2){
    _cli();
    uint8_t * fileDescriptors=pipeCreate(0); // If it is already created, it will just open it
    newFileDescriptor=*(fileDescriptors+r9);
    _sti();
  }
  
  (*(uint64_t *)r8)=(uint64_t)createProcessWithPriority((char *)rsi, (int)rdx, rcx, r9, newFileDescriptor);
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
  uint8_t * result=pipeCreate(id);
  if( result == 0)
    ncPrint("No more pipes allowed");
  descriptor[0]=result[0];
  descriptor[1]=result[1];
  return 0;
}

uint64_t sys_pipe_close(uint64_t id, uint64_t filed, uint64_t rcx, uint64_t r8, uint64_t r9){
  pipeClose(id);
  return 0;
}

uint64_t sys_ask_pid(uint64_t descr, uint64_t pid, uint64_t rcx, uint64_t r8, uint64_t r9){
  uint8_t * aux=(uint8_t *)pid;
  *aux=getPID((char *)descr);
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
  syscalls[26]=&sys_ask_pid;
}

void sysCallsHandler(uint64_t syscode, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){ // lega en rdi desde asm
  if(VALID_SYS_CODE(syscode)){
    (syscalls[syscode])(rsi,rdx,rcx,r8,r9);
  }
  return;
}
