.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# d = matmul(m0, m1)
#   If the dimensions don't match, exit with exit code 2
# Arguments:
# a0 is the pointer to the start of m0
# a1 is the # of rows (height) of m0
# a2 is the # of columns (width) of m0
# a3 is the pointer to the start of m1
# a4 is the # of rows (height) of m1
# a5 is the # of columns (width) of m1
# a6 is the pointer to the the start of d
# Returns:
# None, sets d = matmul(m0, m1)
# =======================================================
matmul:
    # Prologue
    addi sp sp -16
    sw ra, 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)

    addi s1 x0 4 # bytes

    add t0 a2 x0 #col of m0
    add t1 a4 x0 #row of m1
    mv s0 a2
    mv s2 a6

    mv t2 a0 #m0 pointer
    mv t3 a3 #m1 pointer
    mv t6 a3
    mv t4 a1 #rows for m0
    mv t5 a5 #columns for m1

    # Error if mismatched dimensions
    beq t0 t1 outer_loop_start #check if cols of first = rows of second
    j mismatched_dimensions #otherwise, error

outer_loop_start:

    mul t0 a2 s1 #t0 is the size of one row in bytes
    beq t4 x0 done
    mv t1 t5

    j inner_loop_start


outer_loop_end:

    mv t3 t6
    add t2 t2 t0
    addi t4 t4 -1

    j outer_loop_start

inner_loop_start:

    beq t1 x0 outer_loop_end

    mv a0 t2
    mv a1 t3
    mv a2 s0
    addi a3 x0 1
    mv a4 t5

    addi sp, sp, -60
    sw ra, 0(sp)
    sw a0, 4(sp)
    sw a1, 8(sp)
    sw a2, 12(sp)
    sw a3, 16(sp)
    sw a4, 20(sp)
    sw a5, 24(sp)
    sw a6, 28(sp)
    sw t0, 32(sp)
    sw t1, 36(sp)
    sw t2, 40(sp)
    sw t3, 44(sp)
    sw t4, 48(sp)
    sw t5, 52(sp)
    sw t6, 56(sp)

    jal ra dot
    sw a0 0(a6)

    lw ra, 0(sp)
    lw a0 4(sp)
    lw a1, 8(sp)
    lw a2, 12(sp)
    lw a3, 16(sp)
    lw a4, 20(sp)
    lw a5, 24(sp)
    lw a6, 28(sp)
    lw t0, 32(sp)
    lw t1, 36(sp)
    lw t2, 40(sp)
    lw t3, 44(sp)
    lw t4, 48(sp)
    lw t5, 52(sp)
    lw t6, 56(sp)
    addi sp, sp, 60

    addi t3 t3 4
    addi t1 t1 -1
    addi a6 a6 4

    j inner_loop_start

done:
    # Epilogue
    mv a6 s2

    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    addi sp sp 16

    ret


mismatched_dimensions:
    li a1 2
    jal exit2
