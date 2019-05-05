GLOBAL buildStack
GLOBAL printValuesFromStack

SECTION .text

; NEEDED TO BE DONE
setFlags:

printValuesFromStack:
	;rax retorno
	mov rbx, [rbp+4]
	mov rax,[rbx]
	ret

; UNTESTED
buildStack:
    
	mov rcx,[rbp+8]
	mov rdx, [rbp+12]
	mov qword [rcx], rdx ;Esta creo que es la ip
	mov qword [rcx-1], 0x000 ;rax
	mov qword [rcx-2], 0x001 ;rbx
	mov qword [rcx-3], 0x002 ;rcx
	mov qword [rcx-4], 0x003 ;rdx
	mov qword [rcx-5], 0x004 ;rbp
	mov qword [rcx-6], 0x005 ;rdi
	mov qword [rcx-7], 0x06 ;rsi
	mov qword [rcx-8], 0x007 ;r8
	mov qword [rcx-9], 0x008
	mov qword [rcx-10], 0x009
	mov qword [rcx-11], 0x00A
	mov qword [rcx-12], 0x00B
	mov qword [rcx-13], 0x00C
	mov qword [rcx-14], 0x00D
	mov qword [rcx-15], 0x00E ;rd15
	mov qword [rcx-16], 0x202 ;flags

	ret