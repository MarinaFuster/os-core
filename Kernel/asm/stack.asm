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
	mov rbp, rsp

	push rsi
	push 0x00
	push 0x01
	push 0x02
	push 0x03
	push 0x04
	push 0x05
	push 0x06
	push 0x07
	push 0x08
	push 0x09
	push 0x0A
	push 0x0B
	push 0x0C
	push 0x0D
	push 0x0F
	push 0x202

	mov rbp, r9
	mov rax, rsp
	mov rsp, r8
	ret
