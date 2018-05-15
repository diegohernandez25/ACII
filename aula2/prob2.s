	.equ	PRINT_INT,6
	.equ	RESET_CORE_TIMER,12
	.equ	READ_CORE_TIMER,11
	.data	
	.text	
	.globl	main
main:	
	li	$s0,0
while:	
	addi	$s0,$s0,1
	move	$a0,$s0
	li	$a1,10
	li	$v0,PRINT_INT
	syscall
	li	$a0,1000
	jal	delay
	j	while
	jr	$ra

delay:
	move	$t0,$a0
for:
	ble	$t0,0,enddelay
	li	$v0,RESET_CORE_TIMER
	syscall
while2:
	li	$v0,READ_CORE_TIMER
	syscall
	blt	$v0,20000,while2
	subu	$t0,$t0,1
	j	for
enddelay:
	jr	$ra
