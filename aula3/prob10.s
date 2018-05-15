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
main:	#CONTADOR JOHNSON COM DESLOCAMENTO A ESQUERDA
	#RE0 saida
	lui	$s0,SFR_BASE_HI
	lw	$s1,TRISE($s0)
	andi	$s1,$s1,0xFFF0
	sw	$s1,TRISE($s0)
	lw	$s1,TRISB($s0)
	ori	$s1,$s1,0x0002
	sw	$s1,TRISB($s0)
	lw	$s1,LATE($s0)
	andi	$s1,$s1,0xFFF0
	sw	$s1,LATE($s0)
loop:
	lw	$s1,PORTB($s0)
	andi	$s1,$s1,0x0002
	beq	$s1,0x0002,esquerda
	#LATE0=V
	lw	$s1,LATE($s0)
	andi	$s2,$s1,0x0001
	sll	$s2,$s2,3
	xori	$s2,$s2,0x0008
	srl	$s1,$s1,1
	or	$s1,$s1,$s2
	sw	$s1,LATE($s0)
	j	end
esquerda:
	lw	$s1,LATE($s0)
	andi	$s2,$s1,0x0008
	srl	$s2,$s2,3
	xori	$s2,$s2,0x0001
	sll	$s1,$s1,1
	or	$s1,$s1,$s2
	sw	$s1,LATE($s0)	

end:	#delay(500)
	li	$a0,500
	jal	delay
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
	blt	$v0,8000,while2
	subu	$t0,$t0,1
	j	for
enddelay:
	jr	$ra
