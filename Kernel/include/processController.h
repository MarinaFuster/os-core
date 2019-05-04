#ifndef _PROCESS_CONTROLLER_
#define _PROCESS_CONTROLLER_
/*

typedef struct{
    char* description;
    uint8_t pid;
}processDescriptor;

typedef struct{
    processDescriptor process;
    struct processListNode * next;
}processListNode;

typedef struct {
    processListNode * first;
    uint32_t size;
}processList;
void ps ();
void recursivePs(processListNode* node);
int removeFromController(int pid);
int removeFromControllerRec(processListNode* node, int pid);
int createProcess(uint64_t size,char * description);
int createProcessWithPriority(uint64_t size,char * description,int priority);
*/
#endif
