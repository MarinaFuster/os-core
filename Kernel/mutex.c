// MUTEX tiene estructura donde guarda el shared memory entre dos procesos.

// Deberia haber una funcion en donde se pueda crear un mutex entre dos procesos, que este guarda en la estructura.

// Cuando un proceso quiera acceder a una zona de memoria, deberia hacer lock, que es una funcion de aca que busca
// el pid y se fija si la zona de memoria esta disponible o no para escribir.

#include <stdint.h>
#include <syscalls.h>
#include <scheduler.h>
#include <mutex.h>
#include <naiveConsole.h>


typedef struct {
  //uint8_t involvedPID[2]; // No creo que haga falta
  uint64_t sharedMemory;
  uint8_t state;
  uint8_t mutexID;
} mutex;

static mutex array[1];
static  int mutexIDCounter=1;

// Habria que hacer una lista con nodos de mutex, pero si vamos a hacer solo dos procesos es al pedo.

uint8_t mutexLock(uint8_t mutexID, uint8_t callingPID){
    if( (array[mutexID]).state==LOCKED ){
      blockedState(callingPID);
    }

    else{
      array[mutexID].state=LOCKED;
    }

      return 1;
}

uint8_t mutexUnlock(int mutexID, uint8_t callingPID){
    array[mutexID].state=UNLOCKED;

    unblockedState(callingPID);     // callingPID en realidad deberia ser el pid del OTRO proceso QUE ESTABA ESPERANDO
    // Si fuesen mas, deberia haber una cola con los procesos que estan intentando accender a la shm. En este caso no

    return 1;
}

uint8_t createMutex(uint64_t shm){
    mutex newMutex;
    newMutex.sharedMemory=shm;
    newMutex.state=UNLOCKED;
    newMutex.mutexID=mutexIDCounter;
    array[mutexIDCounter]=newMutex;
    mutexIDCounter++;

    //////// TESTING  ///////////////////
    ncPrintHex(newMutex.sharedMemory);
    ncNewline();
    ncPrintDec(newMutex.state);
    ncNewline();
    ncPrintDec(newMutex.mutexID);
    /////////////////////////////////////

    return mutexIDCounter-1;    // Al pedo pq nadie mas deberia saberlo
}
