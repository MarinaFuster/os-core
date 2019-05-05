GLOBAL buildStack
GLOBAL printValuesFromStack

SECTION .text

printValuesFromStack:
	;rax retorno
	mov rbx, [rbp+4]
	mov rax,[rbx]
	ret

; NEEDED TO BE DONE
setFlags:


; UNTESTED
buildStack:
    
	mov rcx,[rbp+8]
;	mov cs, 0x008		mov rdx, [rbp+12]
;	mov rflags, 0x202	mov qword [rcx], rdx ;Esta creo que es la ip
	mov rsp, [rbp+8]	mov qword [rcx-1], 0x000 ;rax
;	mov ss, 0x000		mov qword [rcx-2], 0x001 ;rbx
;	mov base, 0x000		mov qword [rcx-3], 0x002 ;rcx
;   mov gs, 0x001		mov qword [rcx-4], 0x003 ;rdx
;	mov fs, 0x002		mov qword [rcx-5], 0x004 ;rbp
	mov r15, 0x003		mov qword [rcx-6], 0x005 ;rdi
	mov r14, 0x004		mov qword [rcx-7], 0x06 ;rsi
	mov r13, 0x005		mov qword [rcx-8], 0x007 ;r8
	mov r12, 0x006		mov qword [rcx-9], 0x008
	mov r11, 0x007		mov qword [rcx-10], 0x009
	mov r10, 0x008		mov qword [rcx-11], 0x00A
	mov r9, 0x009		mov qword [rcx-12], 0x00B
	mov r8, 0x00A		mov qword [rcx-13], 0x00C
	mov rsi, 0x00B		mov qword [rcx-14], 0x00D
	mov rdi, 0x00C		mov qword [rcx-15], 0x00E ;rd15
	mov rbp, 0x00D		mov qword [rcx-16], 0x202 ;flags
