GLOBAL getStackPointer
GLOBAL saveStackContext
GLOBAL loadStackContext


SECTION .text

; UNTESTED
getStackPointer:
    mov rax, rsp
    ret

; UNTESTED
saveStackContext:
    pushState
    pushfq
    ret

; UNTESTED
loadStackContext:
    mov rsp, rdi
    popfq
    popState
    iretq