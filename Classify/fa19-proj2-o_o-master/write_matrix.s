.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
#   If any file operation fails or doesn't write the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 is the pointer to string representing the filename
#   a1 is the pointer to the start of the matrix in memory
#   a2 is the number of rows in the matrix
#   a3 is the number of columns in the matrix
# Returns:
#   None
# ==============================================================================
write_matrix:

    # Prologue

    addi t0 x0 -1 #error checking (-1 is error)
    addi t1 x0 8 #number of bytes to malloc and read for num rows and cols
    addi a6 x0 4 #size of each int
    addi t4 x0 1 #write permission

    #fopen requires a1 and a2 to be the arguments
    mv t2 a1 #Keep track of initial arguments. t2 is pointer to start of matrix
    mv t3 a2 #Keep track of initial arguments. t3 is num rows of matrix
    mv a5 ra #Keep track of initial ra so we can use ret

    #fopen call
    mv a1 a0
    mv a2 t4
    jal ra fopen
    mv a7 a0              #a7 is file descriptor

    beq t0 a0 eof_or_error

    #malloc buffer for writing the num rows and cols
    mv a0 t1 # 8 bytes
    jal ra malloc
    # mv t6 a0 # t6 is 8 byte buffer

    #putting num rows and cols into the buffer
    sw t3 0(a0)
    sw a3 4(a0)

    #fwrite setup for both calls
    mv a1 a7
    mv a4 a6 #size 4

    #fwrite call to write num rows and cols - fwrite only changes a0
    mv a2 a0 # a2 is the 8 byte buffer
    mv t4 a3 #t4 is the num of cols
    addi a3 x0 2 # 2 items to read

    jal ra fwrite
    addi t5 x0 2
    bne t5 a0 eof_or_error

    #fwrite call to write elements
    mul t5 t3 t4 #total elements of matrix
    mv a2 t2
    mv a3 t5

    jal ra fwrite
    bne t5 a0 eof_or_error


    # Epilogue

    mv a1 a7
    jal ra fclose
    beq t0 a0 eof_or_error

    mv ra a5
    ret

eof_or_error:
    li a1 1
    jal exit2
