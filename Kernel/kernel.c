#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "idtLoader.h"
#include "scheduler.h"
#include "processController.h"

extern void delay();
extern void _hlt();
extern void _cli();

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;


typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The sinasm -felf64 asm/interrupts.asm -o asm/interrupts.o
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}
void * initializeKernelBinary()
{

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main(){
	ncClear();
	load_idt();
	ncPrint("HOLA");
	initializeProcessRegister();
	initializeScheduler();
	//initializeScreen();
	createProcessWithPriority("init", 2, (uint64_t)sampleCodeModuleAddress);
	//((EntryPoint)sampleCodeModuleAddress)();
	while(1){
		_hlt();
	}
	return 0;
}
