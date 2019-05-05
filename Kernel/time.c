#include <processController.h>
#include <scheduler.h>
#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>

extern uint64_t getStackPointer();
extern void loadStackContext(uint64_t stackPointer);
extern void saveStackContext();

//define MAX_FUNCTIONS 255
static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	if(noProcessRunning()){
		return;
	}
	saveStackContext();
	uint64_t rsp=getStackPointer();
	ncPrintHex(rsp);
	ncNewline();
	uint64_t newRsp=contextSwitching(rsp);
	ncPrintHex(newRsp);
	ncNewline();
	loadStackContext(newRsp);
}

int ticks_elapsed(){
	return ticks;
}

int seconds_elapsed(){
	return ticks / 18;
}
