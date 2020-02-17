.import ../read_matrix.s
.import ../utils.s

.data
file_path: .asciiz "./test_input.bin"

.text
main:
    # Read matrix into memory

    la s0 file_path

    #mallocing space for pointer to num rows and num cols
    addi a0 x0 4
    jal ra malloc
    mv a1 a0

    addi a0 x0 4
    jal ra malloc
    mv a2 a0

    mv a0 s0
    jal ra read_matrix

    # Print out elements of matrix

    lw a1 0(a1)
    lw a2 0(a2)
    jal ra print_int_array

    # Terminate the program
    addi a0, x0, 10
    ecall
