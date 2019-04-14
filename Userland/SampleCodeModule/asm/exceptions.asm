GLOBAL _zerodiv
GLOBAL _invalidOpcode

section .text

_zerodiv :
  mov rax,0
  div rax
	ret


_invalidOpcode :
  ud2
  ret
