GLOBAL getStackPointer
GLOBAL saveStackContext
GLOBAL loadStackContext


SECTION .text

getStackPointer:
    mov rax, rsp
    ret

saveStackContext:
    pushState
    pushfq
    ret

loadStackContext:
    mov rsp, [rbp+8]
    popfq
    popState
    iretq