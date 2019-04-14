#ifndef __KEYBOARD__
#define __KEYBOARD__

void addToBuffer(char c);
void removeLastFromBuffer();
void keyboard_handler();
char readLast();

#endif
