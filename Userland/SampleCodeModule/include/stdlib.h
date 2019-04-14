#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

char getChar();
void putChar(char c);
void printf(const char * str, ...);
void strncpy(char * dest, const char * source, int n);
int strcmp(const char * str1, const char * str2);
int strlen(const char * string);
void intToString(int n,char * str);

#endif
