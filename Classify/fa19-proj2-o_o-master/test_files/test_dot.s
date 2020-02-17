.import ../dot.s
.import ../utils.s

# Set vector values for testing
.data
vector0: .word 1 2 3 4 5 6 7 8 9
vector1: .word 1 2 -3 4 5 6 7 8 9


.text
# main function for testing
main:
    # Load vector addresses into registers
    la s0 vector0
    la s1 vector1

    #registers for immediates
    addi t0, x0, 3
    addi t1, x0, 1
    addi t2, x0, 2

    # Set vector attributes
    mv a0 s0
    mv a1 s1
    mv a2 t0
    mv a3 t1
    mv a4 t2

    # Call dot function
    jal ra dot

    # Print integer result
    mv a1 a0
    jal ra print_int

    # Print newline
    li a1 '\n'
    jal ra print_char

    # Exit
    jal exit
