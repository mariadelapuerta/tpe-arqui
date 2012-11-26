GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand, _int_09_hand, _int_80_hand, _int_generic_hand
GLOBAL	_getc, _Hlt, _outb, _inb, _getch, _write, _read
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08, int_09, int_80, int_generic


SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret
	
_Hlt:
	hlt
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
	mov     ebp, esp
	mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
    	out	21h,al
    	pop     ebp
    	retn

_mascaraPIC2:			; Escribe mascara del PIC 2
		push    ebp
    		mov     ebp, esp
    		mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
    		out	0A1h,al
    		pop     ebp
    		retn

_read_msw:
    		smsw    ax		; Obtiene la Machine Status Word
    		retn


_lidt:				; Carga el IDTR
		push    ebp
		mov     ebp, esp
		push    ebx
		mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
		rol	ebx,16		    	
		lidt    [ds: ebx]          ; carga IDTR
		pop     ebx
		pop     ebp
		retn

_int_08_hand:				; Handler de INT 8 ( Timer tick)
		push    ds
		push    es                      ; Se salvan los registros
		pusha                           ; Carga de DS y ES con el valor del selector
		mov     ax, 10h			; a utilizar.
		mov     ds, ax
		mov     es, ax                  
		call    int_08                 
		mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		popa                            
		pop     es
		pop     ds
		iret

_int_09_hand:				; Handler de INT 9 (Teclado)

		pusha

		mov	eax, 0h
		in	al, 60h
		push	eax
		call    int_09                 
		pop	eax
		mov	al,20h		; Envio de EOI generico al PIC
		out	20h,al
		popa                            

		iret

_int_80_hand:				; Handler de INT 80 (I/O)

		sti
		push	ebx
		push	eax
		
		call    int_80		;llama a int_80 con los valores de los 
					;registros ebx y eax como parametros
		pop	eax
		pop	ebx

		iret

_int_generic_hand:
		sti
		push	ebx
		push	eax
		
		call    int_generic	;llama a int_generic (interrupcion genérica
					;agregada por usuario. 
		pop	eax
		pop	ebx

		iret

		
_write:
		push	ebp
		mov	ebp, esp
		pusha
		
		mov 	eax, [ebp+8]		;loads file descriptor
		mov	ebx, [ebp+12]		;loads buffer
		
		int 80h
		
		popa
		pop	ebp
		ret
	
_read:
		push	ebp
		mov	ebp, esp
		pusha
		
		mov	eax, [ebp+8]	;loads file descriptor
		mov	ebx, [ebp+12]	;loads buffer
		
		int	80h
		
		popa
		pop	ebp
		ret

_outb:
		push 	ebp
		mov	ebp, esp
		push 	ax
		push 	dx
		
		mov	word	dx, [ebp + 8]
		mov	word 	ax, [ebp + 12]
		
		out 	dx,al
				
		pop 	dx	
		pop	ax
		pop 	ebp
		
		ret
		
_inb:
		push 	ebp
		mov	ebp, esp
		push	dx

		xor 	eax,eax
		
		mov	word	dx, [ebp + 8]
		in	al, dx
		
		pop	dx
		pop 	ebp
		
		ret
		


; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0


_debug:
		push    bp
        	mov     bp, sp
        	push	ax
vuelve:		mov     ax, 1
        	cmp	ax, 0
		jne	vuelve
		pop	ax
		pop     bp
        	retn
