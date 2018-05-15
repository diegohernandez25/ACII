	.equ	PRINT_INT,6
	.equ 	PRINT_INT_10,7
	.equ	READ_INT,5
	.equ	PRINT_STR,8	

	.data
msg1:	.asciiz "\n Introduza um numero (sinal e modulo)\n"
msg2:	.asciiz "\n Valor lido em base 2:"
msg3:	.asciiz "\n Valor lido em base 16:"
msg4:	.asciiz "\n Valor lido em base 10 (unsigned):"
msg5:	.asciiz "\n Valor lido em base 10 (signed):"
	.text
	.globl	main

main:
while:
	la	$a0,msg1
	ori	$v0,$0,PRINT_STR
	syscall
	ori	$v0,$0,READ_INT
	syscall
	or	$t0,$0,$v0
	#BASE 2	
	la	$a0,msg2
	ori	$v0,$0,PRINT_STR
	syscall
	or	$a0,$0,$t0
	li	$a1,2
	ori	$v0,$0,PRINT_INT
	syscall
	#BASE 16
	la	$a0,msg3
	ori	$v0,$0,PRINT_STR
	syscall
	or	$a0,$0,$t0
	li	$a1,16
	ori	$v0,$0,PRINT_INT
	syscall
	#BASE 10 UNSIGNED
	la	$a0,msg4
	ori	$v0,$0,PRINT_STR
	syscall
	or	$a0,$0,$t0
	li	$a1,10
	ori	$v0,$0,PRINT_INT
	syscall
	#BASE 10 SIGNED
	la	$a0,msg5
	ori	$v0,$0,PRINT_STR
	syscall
	or	$a0,$0,$t0
	ori	$v0,$0,PRINT_INT_10
	syscall

	j	while

endwhile:
	li	$v0,0
	jr	$ra
	

