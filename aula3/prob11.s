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
	ori	$s1,$s1,0x0001
	sw	$s1,TRISB($s0)
	lw	$s1,LATE($s0)
	andi	$s1,$s1,0xFFF1
	sw	$s1,LATE($s0)
loop:
	lw	$s1,PORTB($s0)
	andi	$s1,$s1,0x0001
	beq	$s1,0x0001,esquerda
	#DIREITA
	lw	$s1,LATE($s0)
	andi	$s2,$s1,0x0001
	beq	$s2,0x0000,cont1
	li	$s1,0X0008	
	sw	$s1,LATE($s0)
	j	end
	
cont1:	srl	$s1,$s1,1
	sw	$s1,LATE($s0)
	j	end
esquerda:#ESQUERDA
	lw	$s1,LATE($s0)
	andi	$s2,$s1,0x0008
	beq	$s2,0x0000,cont2
	li	$s1,0x0001	
	sw	$s1,LATE($s0)
	j	end
	
cont2:	sll	$s1,$s1,1
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
