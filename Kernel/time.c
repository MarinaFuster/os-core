#include <scheduler.h>
#include <stdint.h>
#include <time.h>

uint64_t getStackPointer();
void loadStackContext(uint64_t stackPointer);
void saveStackContext();

//define MAX_FUNCTIONS 255
static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	saveStackContext();
	loadStackContext(contextSwitching(getStackPointer()));
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
