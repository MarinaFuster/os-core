#include "scanCodes.h"
#include <naiveConsole.h>
#include "inputBuffer.h"

#define BUFFER_SIZE 50
#define TRUE 1
#define FALSE 0
#define EOF -1


#define RIGHT_SHIFT_PRESSED 54
#define RIGHT_SHIFT_RELEASED -74
#define LEFT_SHIFT_PRESSED 42
#define LEFT_SHIFT_RELEASED -86
#define CAPS_LOCK 58
#define ENTER '\n'
#define DELETE '\b'
#define TAB '\t'

static char circularBuffer[BUFFER_SIZE]={0};
static int currentIndex;
static int lastIndex;
static int bufferEmpty=TRUE;

extern char getPressedKey();

typedef int bool;

#define IS_ALPHA(c) ((c>='a' && c<='z') || (c>='A' && c<='Z'))
#define IS_UPPER_CASE(c) (c>='A' && c<='Z')
#define IS_LOWER_CASE(c) (c>='a' && c<='z')

#define VALID_KEY(c) ((c>0 && c<64))

static bool shift;
static bool caps;

void removeLastFromBuffer(){
	if(bufferEmpty==TRUE){
		return;
	}
	lastIndex=(lastIndex-1)%80;
}

void addToBuffer(char c){
	circularBuffer[lastIndex]=c;                   //Guarda en el buffer la tecla ingresada
	lastIndex = (lastIndex + 1) % BUFFER_SIZE;     //Comienza a pisar el buffer
	bufferEmpty = FALSE;
}

char readLast(){
	if(lastIndex==0){
		lastIndex=BUFFER_SIZE;
	}
	char c=circularBuffer[(lastIndex)-1];
	circularBuffer[(lastIndex)-1]=0;
	return c;
}

void keyboard_handler(){

	char pressedKey=getPressedKey();
	addToBuffer(pressedKey);
	switch(pressedKey){
		case RIGHT_SHIFT_PRESSED:
		case RIGHT_SHIFT_RELEASED:
			shift=!shift;
			break;
		case LEFT_SHIFT_PRESSED:
		case LEFT_SHIFT_RELEASED:
			shift=!shift;
			break;
		case CAPS_LOCK:
			caps = !caps;
			break;
	}

	if(pressedKey<0)
		return;

	if(VALID_KEY(pressedKey)){    //Si presione una tecla correcta
		char asciiKey;
		asciiKey=ASCII_VALUE[pressedKey];

		if(asciiKey==ENTER)
			addToInputBuffer(ENTER);

		else if(asciiKey==DELETE)
			addToInputBuffer(asciiKey);

		else if(asciiKey==TAB)
			addToInputBuffer(TAB);
		else if(asciiKey==0)
			;
		else{
			if(shift){
				asciiKey=SHIFT_ASCII_VALUE[pressedKey];
			}
			else if(caps){
		    	if(IS_LOWER_CASE(asciiKey)){
	            asciiKey=asciiKey-'a'+'A';
          }else if(IS_UPPER_CASE(asciiKey)){
              asciiKey=asciiKey+'a'-'A';
          }
		  }
			addToInputBuffer(asciiKey);
		}
	}
}
