.import read_matrix.s
.import write_matrix.s
.import matmul.s
.import dot.s
.import relu.s
.import argmax.s
.import utils.s

.globl main

.text
main:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0: int argc
    #   a1: char** argv
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    addi t6 x0 4 # 4 bytes THIS CHANGES
    # Exit if incorrect number of command line args
    addi t0 x0 5
    beq t0 a0 continue
    j exit2

continue:
	# =====================================
    # LOAD MATRICES
    # =====================================
    # Load pretrained m0
    lw t0 4(a1)
    # Load pretrained m1
    lw t1 8(a1)
    # Load input matrix
    lw t2 12(a1)
    lw a7 16(a1)

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input

    #allocate memory for integer pointers 2 sets b/c 2 matrices
    mv a0 t6
    jal ra malloc   #m0
    mv t3 a0        #t3 is int pointer for rows

    mv a0 t6
    jal ra malloc
    mv t4 a0        #t4 is int pointer for cols

    mv a0 t6
    jal ra malloc   #input
    mv t5 a0        #t5 is int pointer for rows

    mv a0 t6
    jal ra malloc
    mv t6 a0        #t6 is int pointer for cols

    # two read_matrix calls for two matrices
    mv a0 t0
    mv a1 t3
    mv a2 t4

    jal ra prologue

    jal ra read_matrix
    mv s0 a0       #t0 is ptr to the m0 matrix

    jal ra epilogue

    mv a0 t2
    mv a1 t5
    mv a2 t6

    jal ra prologue
    jal ra read_matrix
    mv s1 a0       #t1 is ptr to the input matrix
    jal ra epilogue

    #Setting up matmul
    mv a0 s0
    lw a1 0(t3)    #rows for m0
    lw a2 0(t4)    #cols for m0

    mv a3 s1
    lw a4 0(t5)    #rows for input
    lw a5 0(t6)    #cols for input

    #malloc for matmul d
    mul t2 a1 a5   #size of resulting matrix
    addi t6 x0 4
    mul t2 t2 t6   #size of resulting matrix in bytes

    addi sp sp -8
    sw a0 0(sp)
    sw a1 4(sp)

    mv a0 t2
    jal ra malloc
    mv s0 a0

    lw a0 0(sp)
    lw a1 4(sp)
    addi sp sp 8

    mv a6 s0       #a6 is an int pointer of size of the resulting matrix from multiplication

	  jal ra prologue
    jal ra matmul
    mv s0 a6
    jal ra epilogue

    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    mv a0 s0      #pointer to the result of the matrix multiplication
    div t2 t2 t6  #num elements of the resulting matrix

    mv t5 a1 #save a1 which contains the # of rows for m0

    jal ra prologue
    mv a1 t2
    jal ra relu
    jal ra epilogue

    #a0 is ReLU(m0 * input)
    mv a6 s0       #a6 is ReLU(m0 * input)

    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

    #read_matrix for m1
    mv a0 t1  #pointer to string of filename
    mv a1 t3  #t3 already an int pointer
    mv a2 t4  #t4 already an int pointer

    jal ra prologue
    jal ra read_matrix
    mv s5 a0
    jal ra epilogue

    #matmul
    mv a0 s5
    lw a1 0(t3)       #a1 is the rows of m1
    lw a2 0(t4)       #a2 is the cols of m1
    mv a3 a6          #a3 is the pointer to ReLU(m0 * input)
    mv a4 t5          #a4 is the rows of ReLU(m0 * input)
    #a5 already set to be cols of second matrix

    mul t3 a1 a5
    addi t4 x0 4
    mul t3 t3 t4 #t3 is the size in bytes of resulting matrix

    addi sp sp -8
    sw a0 0(sp)
    sw a1 4(sp)

    mv a0 t3
    jal ra malloc
    mv s3 a0

    lw a0 0(sp)
    lw a1 4(sp)
    addi sp sp 8

	  jal ra prologue
    mv a6 s3         #a6 is the result of matmul
    jal ra matmul
    mv s4 a6
    jal ra epilogue

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

    #lw a0 16(s0) # Load pointer to output filename
    mv a0 a7         #a0 is the pointer to string representing filename
    mv a2 a1         #a2 is the number of rows
    mv a3 a5         #a3 is the number of cols
    mv t6 a1
    mv a1 s4         #a1 is the pointer to start of matrix in memory

    jal ra prologue
    jal ra write_matrix
    jal ra epilogue

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    mv a0 s4
    mul a1 t6 a5

    jal ra prologue
    jal ra argmax

    # Print classification

    mv a1 a0
    jal ra print_int
    jal ra epilogue


    # Print newline afterwards for clarity
    li a1 '\n'
    jal print_char

    jal exit

prologue:
  addi sp, sp, -60
  sw a0, 0(sp)
  sw a1, 4(sp)
  sw a2, 8(sp)
  sw a3, 12(sp)
  sw a4, 16(sp)
  sw a5, 20(sp)
  sw a6, 24(sp)
  sw t0, 28(sp)
  sw t1, 32(sp)
  sw t2, 36(sp)
  sw t3, 40(sp)
  sw t4, 44(sp)
  sw t5, 48(sp)
  sw t6, 52(sp)
  sw a7, 56(sp)
  ret

epilogue:
  lw a0, 0(sp)
  lw a1, 4(sp)
  lw a2, 8(sp)
  lw a3, 12(sp)
  lw a4, 16(sp)
  lw a5, 20(sp)
  lw a6, 24(sp)
  lw t0, 28(sp)
  lw t1, 32(sp)
  lw t2, 36(sp)
  lw t3, 40(sp)
  lw t4, 44(sp)
  lw t5, 48(sp)
  lw t6, 52(sp)
  lw a7, 56(sp)
  addi sp, sp, 60
  ret
