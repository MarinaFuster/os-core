#ifndef __SYS__CALLS
#define __SYS__CALLS

uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_time(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_date(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_ticks(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_paint(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_exit(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_beep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_disable_beep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void loadSysCalls();

#endif
