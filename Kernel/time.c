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
	if(noProcessRunning())
		return;
	saveStackContext();
	loadStackContext(contextSwitching(getStackPointer(getStackPointer())));
}

int ticks_elapsed(){
	return ticks;
}

int seconds_elapsed(){
	return ticks / 18;
}
