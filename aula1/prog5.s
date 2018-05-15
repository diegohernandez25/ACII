	.equ	STR_MAX_SIZE,20
	.equ	PRINT_STR,8
	.equ	READ_STR,9
	.equ	PRINT_INT,6
	.equ	PRINT_INT10,7

	.data
msg1:	.asciiz "Introduza 2 strings:"
msg1:	.asciiz "Resultados:\n"
str1:	.space	STR_MAX_SIZE+1
str2:	.space	STR_MAX_SIZE+1
str3:	.space	STR_MAX_SIZE+STR_MAX_SIZE+1

	.text
	.globl main

main:
	li	$v0,PRINT_STR
	la	$a0,msg1
	syscall

	li	$v0,READ_STR
	la	$a0,str1
	la	$a1,STR_MAX_SIZE
	syscall
	
	li	$v0,READ_STR
	la	$a0,str2
	la	$a1,STR_MAX_SIZE
	syscall

	li	$v0,PRINT_STR
	la	$a0,msg2
	syscall

	la	$a0,str1
	jal	strlen	
	move	$s0,$v0
	
	la	$a0,str2
	jal	strlen	
	move	$s1,$v0

	move	$a0,$s0
	li	$a1,10
	li	$v0,PRINT_INT
	syscall

	move	$a0,$s1
	li	$a1,10
	li	$v0,PRINT_INT
	syscall

	la	$a0,str3
	la	$a1,str1
	jal	strcpy

	la	$a0,str3
	la	$a1,str2
	jal	strcat
	move	$a0,$v0

	li	$v0,PRINT_STR
	syscall

	la	$a0,str1
	la	$a1,str2
	jal	strcmp
	move	$a0,$v0

	li	$v0,PRINT_INT10
	syscall

	

trlen:
	li	$t0,0
strlen_for:
	lb	$t1,0($a0)
	beq 	$t1,0,strlen_endfor
	addi	$t0,$t0,1
	addi	$a0,$a0,1
	j	strlen_for
strlen_endfor:
	move	$v0,$t0
	jr	$ra

strcat:	
	addi	$sp,$sp,-16
	sw	$ra,0($sp)
	sw	$s0,4($sp)
	sw	$s1,8($sp)
	sw	$s2,12($sp)

	move	$s0,$a0
	move	$s1,$a1
	move	$s2,$a0

strcat_for:
	lb	$t0,0($s0)
	beq	$t0,0,strcat_endfor
	addi	$s0,$s0,1
	j	strcat_for

strcat_endfor:
	move	$a0,$s0
	move	$a1,$s1
	jal	strcpy

	move	$v0,$s2
	lw	$ra,0($sp)
	lw	$s0,4($sp)
	lw	$s1,8($sp)
	lw	$s2,12($sp)
	jr	$ra

strcpy:
	move	$v0,$a0
strcpy_for:
	lb	$t0,0($a0)
	sb	$t0,0($a1)
	beq	$t0,0,strcpy_endfor

	addi	$a0,$a0,1
	addi	$a1,$a1,1
	j	strcpy_for
strcpy_endfor:
	jr	$ra

strcmp:
strcmp_for:
	lb	$t0,0($a0)
	lb	$t1,0($a1)
	
cond1:
	bne	$t0,$t1,cond2
	j	cont 

cond2:
	beq	$t0,$0,strcmp_endfor
cont:
	addi	$a0,$a0,1
	addi	$a1,$a1,1
	j	strcmp_for

strcmp_endfor:
	sub	$v0,$t1,$t0
	jr	$ra
	
