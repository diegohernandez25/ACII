	.equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE,0x6100
	.equ	PORTE,0X6110
	.equ	LATE,0x6120
	.equ	TRISB,0x6040
	.equ	PORTB,0x6050
	.data
	.text
	.globl	main
main:
	lui	$t0,SFR_BASE_HI
	#REO-RE3 saida
	lw	$t1,TRISE($t0)
	andi	$t1,$t1,0xFFF0
	sw	$t1,TRISE($t0)
	#RB0-RB3 entrada
	lw	$t1,TRISB($t0)
	ori	$t1,$t1,0x000F
	sw	$t1,TRISB($t0)

loop:	#RE0=RB0\ RE1=RB1 RE2=RB2 RE3=RB3\
	lw	$t1,PORTB($t0)
	andi	$t1,$t1,0X000F
	xori	$t1,$t1,0X0009
	lw	$t2,LATE($t0)
	andi	$t2,$t2,0xFFF0
	or	$t2,$t2,$t1
	sw	$t2,LATE($t0)
	j	loop
	jr	$ra

