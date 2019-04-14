GLOBAL getPressedKey

section .text

;------------------------------------------------------------
; Funcion que toma del puerto 60h la tecla presionada al
; registro al para que sea recibido por el driver de teclado
; como char
;------------------------------------------------------------

getPressedKey:
	mov qword rax,0 
	in al, 60h
	ret
