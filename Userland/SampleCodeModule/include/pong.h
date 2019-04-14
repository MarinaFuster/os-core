#ifndef __PONG__
#define __PONG__

#include <stdint.h>

extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

//128x48 amount of possible blocks
#define WIDTH 128
#define HEIGHT 48
#define EOF -1
#define TIME_LAPSE 0.2

//Ball movements
#define RIGHT 1
#define LEFT 2
#define	UPRIGHT 3
#define	DOWNRIGHT 4
#define UPLEFT 5
#define	DOWNLEFT 6
#define UP 7
#define DOWN 8


//Rackets movements
#define RACKET_SIZE 7
#define LUP 'w'
#define LDOWN 'z'
#define RUP 'i'
#define RDOWN 'm'
#define ENDGAME 'f'

#define ENTER '\n'


#endif
