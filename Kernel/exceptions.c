#include <naiveConsole.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define EXCEPTIONS 2
#define REGISTERS 16

static void exceptionDisplay(int idx, uint64_t * stackpointer);
void exceptionDispatcher(int exception, uint64_t * stackpointer);

char * exceptions[EXCEPTIONS]= {"ZERO DIVISION EXCEPTION", "INVALID OPCODE EXCEPTION"};
char * registers[REGISTERS]= {"rax", "rbx", "rcx", "rdx", "rbp", "rdi", "rsi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip"};

void exceptionDispatcher(int exception, uint64_t * stackpointer) {
	if (exception == ZERO_EXCEPTION_ID)
		exceptionDisplay(0,stackpointer);

	if( exception == INVALID_OPCODE_ID)
		exceptionDisplay(1,stackpointer);

}

static void exceptionDisplay(int idx, uint64_t * stackpointer){
	ncPrint(exceptions[idx]);
	ncNewline();
	for(int i=0;i<REGISTERS;i++){
		ncPrint(registers[i]);
		ncPrint(" :");
		ncPrintHex(stackpointer[i]);
		ncNewline();
	}

	ncPrint("Exception caught...");
	ncNewline();
	ncPrint("Restarting shell...");
	ncNewline();

}
