GLOBAL buildStack
GLOBAL printValuesFromStack

EXTERN exitProcess

SECTION .text

printValuesFromStack:
	;rax retorno
	mov rax,[rdi]
	ret

; NEEDED TO BE DONE
setFlags:


; UNTESTED
buildStack:

    mov r10, rsp 	; r8 is reserved for argument passing, thus its not used
	mov r11, rbp		; r9 is also reserved for argument passing
	mov rsp, rdi
	
	push 0x0000     ; stack segment
	push rdi        ; stack pointer
	push 0x202	    ; rflags	
	push 0x8		; cs
	push rsi		; rip
	push 0x0000		; rax
	push 0x0001		; rbx
	push 0x0002		; rcx
	push r8 		; rdx
	push 0x0003		; rbp
	push rdx		; rdi
	push rcx		; rsi
	push 0x0004		; r8
	push 0x0005
	push 0x0006
	push 0x0007
	push 0x0008
	push 0x0009
	push 0x000A
	push 0x000B		; r15

	mov rax, rsp

	mov rbp, r11
	mov rsp, r10

	ret
