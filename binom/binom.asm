		
		.word 0x100 3								# initialize n as an input
		.word 0x101 2								# initialize k as an input
		
		lw		$a1, $zero, $imm, 0x100				# store n in $a1
		lw		$a0, $zero, $imm, 0x101				# store k in $a2
		add		$sp, $zero, $imm, 511				# set stack pointer at the end of memory
		jal		$imm, $zero, $zero, bin				# call function binom
		sw		$v0, $zero, $imm, 0x102				# store return value in 0x102
		halt	$zero, $zero, $zero, 0				# end program
bin:		
		sub		$sp, $sp, $imm, 4   				# alocate space in stack
		sw		$ra, $sp, $imm, 4					# store $ra MEM(sp+4)
		sw		$s0, $sp, $imm, 3					# store $s0 MEM(sp+3)
		sw		$a0, $sp, $imm, 2					# store $a0 in MEM(sp+2)
		sw		$a1, $sp, $imm, 1					# store $a1 in MEM(sp+1)
		beq		$imm, $a0, $zero, stop			    # if k = 0 jump to stop
		beq		$imm, $a0, $a1, stop				# if n = k jump to stop
		sub		$a1, $a1, $imm, 1	 				# n = n - 1
		jal		$imm, $zero, $zero, bin				# binom(n-1,k) 
		sub		$a0, $a0, $imm, 1					# k = k - 1 
		add		$s0, $zero, $v0, 0					# save binom(n-1,k) in s0
		jal		$imm, $zero, $zero, bin				# bin(n-1,k-1) 
		add		$v0, $v0, $s0, 0					# v0 = binom(n-1,k) + binom(n-1,k-1) 
return:		
		lw		$ra, $sp, $imm, 4					# Restor ra 
		lw	 	$s0, $sp, $imm, 3					# restore s0 
		lw		$a0, $sp, $imm, 2					# restore a0
		lw		$a1, $sp, $imm, 1					# restore a1
		add		$sp, $sp, $imm, 4					# free space in stack
		beq		$ra, $zero, $zero, 0				# Return to calle
stop:
		add		$v0, $zero, $imm, 1					# v0 = 1
    	beq		$imm, $zero, $zero, return			# jump to return
    
