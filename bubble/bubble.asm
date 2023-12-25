# array to be sorted:

.word 0x100 2
.word 0x101 0
.word 0x102 4325
.word 0x103 22
.word 0x104 -34
.word 0x105 90
.word 0x106 -176
.word 0x107 22
.word 0x108 91
.word 0x109 90
.word 0x10A 93
.word 0x10B 92
.word 0x10C 94
.word 0x10D 100
.word 0x10E -1
.word 0x10F -1

# save initial registers in stack:
    add $sp, $zero, $imm, 511				# set stack pointer at the end of memory
	sub $sp, $sp, $imm, 3 					# make room for 3 elements in the stack
	sw $s2, $sp, $imm, 2					# Storing  $s2 in memory(sp + 2)
	sw $s1, $sp, $imm, 1					# Storing  $s1 in memory(sp + 1)
	sw $s0, $sp, $imm, 0					# Storing  $s0 in memory(sp)
	
        
# initialize variables:
MAIN:
    add $s1, $zero, $imm, 15                  # load the number of elements (including 0 - total of 15) in the array into $s1

# bubble sort algorithm:
OUTER_LOOP:
    add  $t0, $zero, $zero, 0                # initialize $t0=j as a "swap" indicator (if j=1 => swap occured)
    add  $t1, $zero, $zero, 0                # initialize $t1=i to 0 for loop condition
    add $s0, $zero, $imm, 256                # load the address of the array into $s0 = 0x100 at the start of the array

INNER_LOOP:
	lw $t2, $s0, $zero, 0                    # load the current element into $t2
    lw $t3, $s0, $imm, 1                     # load the next element into $t3
    ble $imm, $t2, $t3, SKIP_SWAP            # branch if $t2 <= $t3 to skip swap
    sw $t3, $s0, $zero, 0                    # store $t3 in the current element's location
    sw $t2, $s0, $imm, 1                     # store $t2 in the next element's location
    add $t0, $zero, $imm, 1                  # set $t0 to 1 to indicate a swap occurred

SKIP_SWAP:
	add $s0, $s0, $imm, 1                    # increment $s0 to the next element
    add $t1, $t1, $imm, 1                    # increment $t1 to track the number of elements checked
    bne $imm, $t1, $s1, INNER_LOOP           # if t1!=s1 (i<15) go to inner_loop
    bne $imm, $t0, $zero, OUTER_LOOP         # branch to outer_loop if a swap occurred

EXIT:
    lw $s2, $sp, $imm, 2					# restore $s2 to original value
	lw $s1, $sp, $imm, 1					# restore $s1 to original value 
	lw $s0, $sp, $imm, 0					# restore $s0 to original value 
	add $sp, $sp, $imm, 3					# release allocated space
    halt $zero, $zero, $zero, 0			# end program
  