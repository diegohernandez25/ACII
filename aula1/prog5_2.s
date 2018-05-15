	.eu	PRINT_STR,8	
	.equ	READ_STR,9
	.equ	PRINT_INT,6
	.equ	PRINT_INT10,7
	.equ	STR_MAX_SIZE,20

	

	.data
msg1:	.asciiz "Introduza 2 strings:"
msg2:	.asciiz "Resultados:\n"
str1:	.space	STR_MAX_SIZE+1
str2:	.space	STR_MAX_SIZE+1
str3:	.space	STR_MAX_SIZE+STR_MAX_SIZE+1
	
	.text
	.globl 	main

main:
	
	#printStr("Introduza 2 strings:")
	la	$a0,msg1
	li	$v0,PRINT_STR
	syscall
	
	#readStr(str1, STR_MAX_SIZE)
	la	$a0,str1
	la	$a1,STR_MAX_SIZE
	li	$v0,READ_STR
	syscall
	
	#readStr(str1, STR_MAX_SIZE)
	la	$a0,str2
	la	$a1,STR_MAX_SIZE
	li	$v0,READ_STR
	syscall

	#printStr("Resultados:\n")
	li	$v0,PRINT_STR
	la	$a0,msg2
	syscall

	#printInt(strlen(str1),10)
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

	#strcpy(str3, str1)
	la	$a0,str3
	la	$a1,str1
	jal	strcpy
	
	#printStr(strcat(str3, str2))
	la	$a0,str3
	la	$a1,str2
	jal	strcat
	move	$a0,$v0
	li	$v0,PRINT_STR
	syscall
	
	#printInt10(strcmp(str1,str2))
	la	$a0,str1
	la	$a1,str2
	jal	strcmp
	move	$a0,$v0
	li	$v0,PRINT_INT10
	syscall

	#return 0
	li	$v0,0
	jr	$ra

strlen:
	subu	$sp,$sp,12
	sw	$ra,0($sp)
	sw	$s0,4($sp)
	sw	$s1,8($sp)
	li	$s0,0			#len=0
	
strlen_loop:
	lb	$s1,0($a0)		#conteudo do ponteiro a0
	beq	$s1,0,strlen_end
	addi	$s0,$s0,1
	addi	$a0,$a0,1
	j	strlen_loop

strlen_end:
	move	$v0,$s0
	lw	$ra,0($sp)
	lw	$s0,4($sp)
	lw	$s0,8($sp)
	addu	$sp,$sp,12
	jr	$ra

strcat:
	subu	$sp,$sp,16
	sw	$ra,0($sp)
	sw	$s0,4($sp)
	sw	$s1,8($sp)
	sw	$s2,12($sp)
	move	$s0,$a0
	move	$s1,$a1
	move	$s2,$a0

strcat_loop:
	lb	$t0,0($s0)
	beq	$t0,0,strcat_end
	addi	$s0,$s0,1
	j	strcat_loop

strcat_end:
	move	$a0,$s0
	move	$a1,$s1
	jal	strcpy
	move	$v0,$s2
	lw	$ra,0($sp)
	lw	$s0,4($sp)
	lw	$s1,8($sp)
	lw	$s2,12($sp)
	addu	$sp,$sp,16
	jr	$ra

strcpy:
	move	$v0,$a0
strcpy_loop:
	lb	$t0,0($a0)
	sb	$t0,0($a1)
	beq	$t0,0,strcpy_end
	addi	$a0,$a0,1
	addi	$a1,$a1,1
	j	strcpy_loop

strcpy_end:
	jr	$ra

strcmp:
strcmp_for:
	lb	$t0,0($a0)
	lb	$t1,0($a1)

cond1:
	beq	$t0,$t1,cond2
	j	strcmp_endfor
cond2:
	beq	$t0,$0,strcmp_endfor
cont:
	addi	$a0,$a0,1
	addi	$a1,$a1,1
	j	strcmp_for

strmp_endfor:
	sub	$v0,$t1,$t0
	jr	$ra




