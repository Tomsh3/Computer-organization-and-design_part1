
  # Set up matrix1:

  .word 0x100 0 				# a11 in matrix1
  .word 0x101 1 				# a12 in matrix1
  .word 0x102 2  				# a13 in matrix1 
  .word 0x103 3  				# a14 in matrix1
  .word 0x104 4  				# a21 in matrix1
  .word 0x105 5  				# a22 in matrix1
  .word 0x106 6 				# a23 in matrix1 
  .word 0x107 7 				# a24 in matrix1
  .word 0x108 8  				# a31 in matrix1
  .word 0x109 9 				# a32 in matrix1
  .word 0x10A 10 				# a33 in matrix1 
  .word 0x10B 11 				# a34 in matrix1
  .word 0x10C 12 				# a41 in matrix1
  .word 0x10D 13 				# a42 in matrix1
  .word 0x10E 14 				# a43 in matrix1 
  .word 0x10F 15 				# a44 in matrix1

   # Set up matrix2:

  .word 0x110 0  				# b11 in matrix2
  .word 0x111 10				# b12 in matrix2
  .word 0x112 20  				# b13 in matrix2 
  .word 0x113 30				# b14 in matrix2
  .word 0x114 40 				# b21 in matrix2
  .word 0x115 50				# b22 in matrix2
  .word 0x116 60  				# b23 in matrix2 
  .word 0x117 70 				# b24 in matrix2
  .word 0x118 0  				# b31 in matrix2
  .word 0x119 -10 				# b32 in matrix2
  .word 0x11A -20 				# b33 in matrix2 
  .word 0x11B -30 				# b34 in matrix2
  .word 0x11C -40				# b41 in matrix2
  .word 0x11D -50 				# b42 in matrix2
  .word 0x11E -60				# b43 in matrix2 
  .word 0x11F -70				# b44 in matrix2


main:
  # Load addresses of matrices into registers
  add $t0, $zero, $imm, 256		# t0 = 0x100
  add $t1, $zero, $imm, 272		# t1 = 0x110
  add $t2, $zero, $imm, 288		# t2 = 0x120
  add $sp, $zero, $imm, 511		# set stack pointer at the end of memory
  sub $sp, $sp, $imm, 2			# make room for 3 elements in the stack
  sw $sp, $s0, $imm, 2			# store %s0 in memory[$sp + 2]
  sw $sp, $s1, $imm, 1			# store %s1 in memory[$sp + 1]
  sw $sp, $s2, $imm, 0			# store %s2 in memory[$sp + 0]

  # Add corresponding elements of matrices
  add $t3, $zero, $imm, 16      # Initialize loop counter to the number of itterations required (16)

loop:
  lw $s0, $t0, $zero, 0			# Load element from matrix1 ### not good! ###
  lw $s1, $t1, $zero, 0			# Load element from matrix2 ### not good! ###
  add $s2, $s0, $s1, 0			# Add the two elements ### not good! ###
  sw $s2, $t2, $zero, 0			# Store the result in matrix3
  add $t0, $t0, $imm, 1			# Increment matrix1 pointer
  add $t1, $t1, $imm, 1			# Increment matrix2 pointer
  add $t2, $t2, $imm, 1			# Increment matrix3 pointer
  sub $t3, $t3, $imm, 1			# Decrement loop counter
  bne $imm, $t3, $zero, loop	# Repeat until all elements added

exit:
  lw $s0, $sp, $imm, 2			# store %s0 in memory[$sp + 2]
  lw $s1, $sp, $imm, 1			# store %s1 in memory[$sp + 1]
  lw $s2, $sp, $imm, 0			# store %s2 in memory[$sp + 0]
  halt	$zero, $zero, $zero, 0	# end program
