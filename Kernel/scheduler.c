#include <stdint.h>

typedef struct{
    uint32_t pid;
    uint8_t state;
    uint64_t time; // Tiempo transcurrido
    uint64_t stackPointer; // No se si esto era necesario al final. Veremos en la implementacion
    char * description;
}taskDescriptor;

typedef struct{
    taskDescriptor task;
    taskDescriptor * next;
}dequeueNode;

typedef struct{
    taskDescriptor * first;
    uint32_t size;
}dequeue;
// Chequeen en queue si prefieren dejar el size o ver de hacer el nodo centinela
// Me parecio mas quilombo asi que deje el size, el otro no sabia muy bien como hacerlo.
// Faltarian metodos add, remove, etc de la FIFO

/* 
El indice corresponde a la prioridad: 0 (mas importante), 1, 2.
*/
static dequeue * priorityQueues[3]={0};
