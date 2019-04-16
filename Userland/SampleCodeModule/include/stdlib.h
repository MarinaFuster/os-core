#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

void free(void * pointer);
char getChar();
void intToString(int n,char * str);
void * malloc(uint64_t size);
void printf(const char * str, ...);
void putChar(char c);
int strcmp(const char * str1, const char * str2);
int strlen(const char * string);
void strncpy(char * dest, const char * source, int n);


#endif
