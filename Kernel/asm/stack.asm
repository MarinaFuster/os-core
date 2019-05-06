GLOBAL buildStack
GLOBAL printValuesFromStack

SECTION .text

printValuesFromStack:
	;rax retorno
	mov rax,[rdi]
	ret

; NEEDED TO BE DONE
setFlags:


; UNTESTED
buildStack:

    mov r8, rsp 	; r8 is reserved for argument passing, thus its not used
	mov r9, rbp		; r9 is also reserved for argument passing
	mov rsp, rdi

	push 0x0000     ; stack segment
	push rdi        ; stack pointer
	push 0x206	    ; rflags	
	push 0x8		; cs
	push rsi		; rip
	push 0x0000		; rax
	push 0x0001		; rbx
	push 0x0002		; rcx
	push 0x0003		; rdx
	push 0x0004		; rbp
	push 0x0005		; rdi
	push 0x0006		; rsi
	push 0x0007		; r8
	push 0x0008
	push 0x0009
	push 0x000A
	push 0x000B
	push 0x000C
	push 0x000D
	push 0x000F		; r15

	mov rax, rsp

	mov rbp, r9
	mov rsp, r8

	ret
