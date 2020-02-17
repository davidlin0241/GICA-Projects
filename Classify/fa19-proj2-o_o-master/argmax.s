.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 is the pointer to the start of the vector
#	  a1 is the # of elements in the vector
# Returns:
#	  a0 is the first index of the largest element
# =================================================================
argmax:
    # Prologue


loop_start:
  lw t0 0(a0) #t0 contains max
  add t4, x0, x0 #index of max

  addi t3, x0, -1 #to multiply by negative
  mul t0 t0 t3 # no greater than, so need to negate for less than

  add t2 x0 x0 #index counter

loop_continue:
  addi a0 a0 4 #next element of array
  addi t2 t2 1 #increment index counter

  beq t2 a1 loop_end

  lw t1 0(a0) #t1 contains current element
  mul t1 t1 t3 # no greater than, so need to negate for less than

  slt t5 t1 t0 #if t2 is one, then we have new max
  beq t5 x0 loop_continue
  add t0, t1, x0
  add t4, t2, x0
  j loop_continue

loop_end:
  # Epilogue
  add a0, t4, x0
  ret

#max = array[0]
#for i in range(array.size):
# if (array[i] > max):
#   max = array[i]
# return i
