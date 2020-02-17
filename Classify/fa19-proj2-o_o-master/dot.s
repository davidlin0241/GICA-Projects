
.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 is the pointer to the start of v0
#   a1 is the pointer to the start of v1
#   a2 is the length of the vectors
#   a3 is the stride of v0
#   a4 is the stride of v1
# Returns:
#   a0 is the dot product of v0 and v1
# =======================================================
dot:
    # Prologue

    addi t3 x0 4 #size of byte

    mul t0 t3 a3 #stride for v0
    mul t1 t3 a4 #stride for v1

    add t2, x0, x0 # total
loop_start:

    beq a2 x0 loop_end #stop loop when no more elements

    lw t5 0(a0) #load first element of v0 into t5
    lw t6 0(a1) #load first element of v1 into t6
    mul t3 t5 t6 #multiply t5 and t6 into t3

    add t2, t2, t3

    add a0 a0 t0 #increment v0 pointer by stride of v0
    add a1 a1 t1 #increment v1 pointer by stride of v1
    addi a2 a2 -1 #decrement no. of remaining elements by 1

    j loop_start #repeat

loop_end:

    # Epilogue
    add a0, t2, x0
    ret
