[org 7C00h] 				; Set location counter

; Start of the boot sector's main routine
bootload_start:
	
	mov si, message
	call print_string
	

	jmp $ 					; Hang

; Output string in SI to screen	
print_string:			
	pusha

	mov ah, 0Eh			; int 10h teletype function

.repeat:

	lodsb				; Get char from string
	cmp al, 0
	je .done			; If char is zero, end of string
	int 10h				; Otherwise, print it
	jmp short .repeat 	; And repeat for all characters

.done:

	popa
	ret	


message db "Sorry, your computer died :(", 0

times 510-($-$$) db 0	; Fill memory with 0 so that the size of the bootloader will always be 512 (+2 signature bytes)
dw 0AA55h				; Boot sector signature - tell the CPU this is our bootloader

