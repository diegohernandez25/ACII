	.equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE,0x6100
	.equ	PORTE,0X6110
	.equ	LATE,0x6120
	.equ	TRISB,0x6040
	.equ	PORTB,0x6050
	.equ	RESET_CORE_TIMER,12
	.equ	READ_CORE_TIMER,11
	.data
	.text
	.globl	main
main:
	#RE0 saida
	lui	$s0,SFR_BASE_HI
	lw	$s1,TRISE($s0)
	andi	$s1,$s1,0xFFFE
	sw	$s1,TRISE($s0)
	li	$s2,0x0000

loop:
	#LATE0=V
	lw	$s1,LATE($s0)
	andi	$s1,$s1,0xFFFE
	or	$s1,$s1,$s2
	sw	$s1,LATE($s0)
	#delay(500)
	li	$a0,500
	jal	delay
	#x=x xor 1
	xori	$s2,$s2,0x0001
	j	loop
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

