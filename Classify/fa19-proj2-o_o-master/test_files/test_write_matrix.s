.import ../write_matrix.s
.import ../utils.s

.data
m0: .word 1, 2, 3, 4, 5, 6, 7, 8 # MAKE CHANGES HERE
file_path: .asciiz "test_files/test_output.bin"

.text
main:
    la s0 m0
    la s1 file_path

    #setup for function call
    mv a0 s1
    mv a1 s0
    addi a2 x0 3  #MAKE CHANGES HERE , NUM ROWS
    addi a3 x0 3  #MAKE CHANGES HERE , NUM COLS

    # Write the matrix to a file
    jal ra write_matrix

    # Exit the program
    addi a0 x0 10
    ecall
