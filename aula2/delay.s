	.equ	READ_CORE_TIMER,11
	.equ	RESET_CORE_TIMER,12
	.data
	.text
	.globl	main
main:
	subu 	$sp,$sp,8
	sw	$ra,0($sp)
	sw	$s0,4($sp)
	move	$s0,$a0		#ms
for:
	ble	$s0,0,endfor
	li	$v0,RESET_CORE_TIMER
	syscall
	
while:	
	li	$v0,READ_CORE_TIMER
	syscall
	blt	$v0,20000,while	#substituir K| 1ms->K=20000
	j	for
	
endfor:
	lw	$ra,0($sp)
	lw	$s0,4($sp)
	addu	$sp,$sp,8
	jr	$ra

