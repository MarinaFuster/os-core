GLOBAL buildStack

SECTION .text

setFlags:


buildStack:

    mov rax, [rbp+12]
;	mov cs, 0x008
;	mov rflags, 0x202
	mov rsp, [rbp+8]
;	mov ss, 0x000
;	mov base, 0x000
;   mov gs, 0x001
;	mov fs, 0x002
	mov r15, 0x003
	mov r14, 0x004
	mov r13, 0x005
	mov r12, 0x006
	mov r11, 0x007
	mov r10, 0x008
	mov r9, 0x009
	mov r8, 0x00A
	mov rsi, 0x00B
	mov rdi, 0x00C
	mov rbp, 0x00D
	mov rdx, 0x00E
	mov rcx, 0x00F
	mov rbx, 0x010
	jmp rax
