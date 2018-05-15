	.equ	GET_CHAR,2
	.equ	PUT_CHAR,3
	
	.data	
	.text
	.globl	main

main:
while:
	ori	$v0,$0,GET_CHAR	#c=getChar()
	syscall
	beq	$v0,'\n',endwhile #t0==\n -> endwhile
	or	$a0,$0,$v0
	ori	$v0,$0,PUT_CHAR
	syscall
	j	while 
	
endwhile:
	li	$v0,0
	jr	$ra

