#ifndef __PROCESS_CONTROLLER__
#define __PROCESS_CONTROLLER__


typedef struct{
    char* description;
    uint32_t pid;
    uint64_t address;
}processDescriptor;

typedef struct processListNode{
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


#endif
