	.equ	INKEY,1
	.equ	PRINT_STR,8

	.data
msg:	.asciiz	"Key Pressed\n"
	.text
	.globl	main

main:
while:	
	ori	$v0,$0,INKEY
	syscall				#c=inkey()
	beq	$v0,0,while		#if c==0 -> while
if:	beq	$v0, '\n',endwhile	#if c==\n -> endwhile
	ori	$v0,$0,PRINT_STR	
	la	$a0,msg			
	syscall				#printStr("Key pressed\n")
	j	while

endwhile:
	li	$v0,0
	jr	$ra			#return 0
