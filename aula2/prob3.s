	.equ	READ_CORE_TIMER,11
	.equ	RESET_CORE_TIMER,12
	.equ	PRINT_INT,6
	.equ	PUT_CHAR,3
	.data
	.text
	.globl	main
main:	
while:
	lui	$s1,0xBF88
	lw	$s2,0x6050($s1)		
	andi	$s0,$s2,0x000F		#4 bits menos significativos
	move	$a0,$s0			
	li	$a1,2
	li	$v0,PRINT_INT
	syscall
	li	$a0,' '
	li	$v0,PUT_CHAR
	syscall
	####(16-valSwitch)*64####
	li	$a0,16
	subu	$s0,$a0,$s0
	mul	$a0,$s0,64
	jal	delay
	#########################
	j	while

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
