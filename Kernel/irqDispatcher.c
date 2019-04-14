#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <rtc.h>

void addToBuffer();
char readFromBuffer();
char readLast();

static void int_21();
static void int_20();

int print=1;

void irqDispatcher(uint64_t irq) {

	switch (irq) {
		case 1:{
			int_21();
			break;
		}
		case 0:
			int_20();
			break;

	}
	return;
}


void int_20(){
	timer_handler();
}

void int_21() {
	keyboard_handler();
}
