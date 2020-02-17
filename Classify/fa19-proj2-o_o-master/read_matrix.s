.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#   If any file operation fails or doesn't read the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 is the pointer to string representing the filename
#   a1 is a pointer to an integer, we will set it to the number of rows
#   a2 is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 is the pointer to the matrix in memory
# ==============================================================================
read_matrix:

    # Prologue


    addi t0 x0 -1 #error checking (-1 is error)
    addi t1 x0 8 #number of bytes to malloc and read for num rows and cols
    addi a6 x0 4  #size of each int
    mv t2 a1 #Keep track of initial arguments
    mv t3 a2 #Keep track of initial arguments
    mv a5 ra #Keep track of initial ra so we can use ret

    #fopen call

    mv a1 a0
    mv a2 x0
    jal ra fopen
    mv a7 a0              #a7 is file descriptor

    beq t0 a0 eof_or_error

    #malloc call for 8 bytes
    mv a0 t1
    jal ra malloc         #malloc changes a1
    mv a2 a0              #a2 is buffer (8 bytes)

    #fread call
    mv a1 a7
    mv a3 t1
    jal ra fread

    bne t1 a0 eof_or_error

    lw t4 0(a2)           #t4 is num rows
    lw t5 4(a2)           #t5 is num cols
    sw t4 0(t2)
    sw t5 0(t3)           #t4 = 0(t2) is row, t5 = 0(t3) is column,

    mul t6 t4 t5
    mul t6 t6 a6

    #addi sp sp -4
    #sw a1 0(sp)

    mv a0 t6
    jal ra malloc

    #lw a1 0(sp)
    #addi sp sp 4

  	mv a1 a7
    mv a2 a0
    mv a3 t6
    jal ra fread
    bne a0 t6 eof_or_error

    mv a1 a7
    jal ra fclose
    beq t0 a0 eof_or_error

    mv a0 a2
    mv a1 t2
    mv a2 t3

    mv ra a5
    ret

eof_or_error:
    li a1 1
    jal exit2
