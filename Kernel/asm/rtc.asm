GLOBAL _getSeconds
GLOBAL _getMinutes
GLOBAL _getHours
GLOBAL _getDayofMonth
GLOBAL _getMonth
GLOBAL _getYear

section .text

%macro _setState 1
            mov rax,0
            mov al,0Bh
            out 70h,al
            in al,71h
            or al,4
            out 71h,al
            mov al, %1
            out 70h,al
%endmacro


_getSeconds:
            push rbp
            mov rbp,rsp
            _setState 0
            in al,71h
            leave
            ret

_getMinutes:
            push rbp
            mov rbp,rsp
            _setState 2
            in al,71h
            leave
            ret

_getHours:
            push rbp
            mov rbp,rsp
            _setState 4
            in al,71h
            leave
            ret

_getDayofMonth:
            push rbp
            mov rbp,rsp
            _setState 7
            in al,71h
            leave
            ret

_getMonth:
            push rbp
            mov rbp,rsp
            _setState 8
            in al,71h
            leave
            ret

_getYear:
            push rbp
            mov rbp,rsp
            _setState 9
            in al,71h
            leave
            ret
