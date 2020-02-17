.import ../matmul.s
.import ../utils.s
.import ../dot.s

# static values for testing
.data
d: .word 0 0 0 0 0 0 0 0 0 # allocate static space for output

#Given test
#m0: .word 1 2 3 4 5 6 7 8 9
#m1: .word 1 2 3 4 5 6 7 8 9

#Invalid size testing 2x3 with 2x4
#m0: .word 1 -1 1 10 245 0
#m1: .word -50 12 13 27 -15 12 9 0

#Invalid size testing 5x2 4x1
m0: .word 1 -1 1 10 245 0 27 3 10 9
m1: .word -50 12 13 27

#2x2 with 2x4
#m0: .word 1 -1 10 245
#m1: .word -50 12 13 27 -15 12 9 0


.text
main:
    # Load addresses of input matrices (which are in static memory), and set their dimensions
    la s0 m0
    la s1 m1
    la s2 d

    mv a0 s0
    addi a1 x0 5  #CHANGE FOR TESTING
    addi a2 x0 2  #CHANGE
    mv a3 s1
    addi a4 x0 4  #CHANGE
    addi a5 x0 1  #CHANGE
    mv a6 s2

    # Call matrix multiply, m0 * m1
    jal ra matmul

    # Print the output (use print_int_array in utils.s)
    mv a0 a6
    addi a1, x0, 2 #CHANGE  num rows in array
    addi a2, x0, 4 #CHANGE  num columns in array
    jal ra print_int_array

    # Exit the program
    jal exit
