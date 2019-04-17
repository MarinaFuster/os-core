#include <stdint.h>

typedef struct{
    uint32_t pid;
    uint64_t address;
}processDescriptor;

typedef struct{
    processDescriptor process;
    processDescriptor * next;
}processListNode;

typedef struct {
    processDescriptor * first;
    uint32_t size;
}processList;

// Hay que chequear que no haya problemas con esta inicializacion
// Asi como con otras. Revisar codigo! 
static processList * processRegister=0; 

// ESTA FUNCION TIENE QUE DESAPARECER UNA VEZ TERMINADO TODO!
void
function_just_meant_to_remove_process_controler_warnings(){
    processRegister=0;
    return;
}