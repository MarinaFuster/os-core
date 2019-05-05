GLOBAL saveStackContext
GLOBAL loadStackContext


SECTION .text

; UNTESTED
saveStackContext:
    push rbp
    mov rbp, rsp

    pushState
    pushfq

    mov rsp, rbp
    pop rbp

    ret

; UNTESTED
loadStackContext:

    mov rsp, rdi
    popfq
    popState

    iretq