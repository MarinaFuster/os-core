#include "include/shell.h"
#include "include/stdlib.h"
/* sampleCodeModule.c */

int main() {
	exec("bash", 0,(uint64_t)displayShell);
	//displayShell();
	return 0;
}
