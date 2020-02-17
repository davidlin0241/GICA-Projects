.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 is the pointer to the array
#	  a1 is the # of elements in the array
# Returns:
#	  None
# ==============================================================================
relu:
    # Prologue

loop_continue:
    beq a1, x0, loop_end  # end if number of elements is reached

    lw t2, 0(a0)

    addi a1, a1, -1
    addi a0, a0, 4

    slti t3, t2, 0
    beq t3, x0, loop_continue

    sw x0, -4(a0) # set element to zero

    j loop_continue
loop_end:
    # Epilogue
	  ret # same as jr ra, which is jalr x0, ra, 0

#for i in range(array.len):
# if array[i] < 0:
#   array[i] = 0
