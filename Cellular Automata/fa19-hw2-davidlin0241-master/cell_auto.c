/****************************************************************************************
 *
 * CS 61C Fall 2019
 * Homework 2: C Basics -- Cellular Automata
 *
 ****************************************************************************************/

/****************************************************************************************
 *
 * Includes
 *
 ****************************************************************************************/

// <stdio.h> is needed for `printf()` and `fprintf()`
#include <stdio.h>

// <stdlib.h> is needed for `malloc()`, `free()`, etc.
#include <stdlib.h>

// <sys/types.h> needed for u_int32_t
#include <sys/types.h>

// Feel free to include any other C standard library header files here
// (Our staff solution does not use any other header files)

#include <string.h>



/****************************************************************************************
 *
 * Functions
 *
 ****************************************************************************************/

/*
 * This function is called when `cell_auto.c` is run with invalid inputs
 */
void usage(void)
{

    printf("    Proper usage: cell_auto <rule> <rows>\n\n");

    printf("    This program simulates a simple cellular automata, outputting data to STDOUT\n"
           "    in the PBM file format. It takes two arguments. First, `rule`, which should be\n"
           "    a number between 0 and 2^32 - 1. Second, `rows`, the number of generations the\n"
           "    simulation should run for. Including the first row (the tip of the triangle),\n"
           "    which is one \"alive\" cell in the center, surrounded by `rows` \"dead\" cells\n"
           "    on its left and right, the resulting grid from simulation should have dimensions\n"
           "    [ (`rows` + 1) x (2 * `rows` + 1)].\n");

    // Execution should *NOT* continue after `usage()` is called, call `exit()` with return code 1
    exit(1);

}

//turn rule into binary
void getBinaryString(char** str, u_int32_t n) {
 int i, bit;
 for (i = 31; i >= 0; i --) {
     bit = n >> i;
     //printf("%d\n", bit);
     if (bit & 1) //off
         strcat(*str, "1");
     else //on
         strcat(*str, "0");
 }
}

//printer for the output
void printBinaryString(char* string, int width) {
 int i;

 for (i = 2; i<width-2; i++){
     printf("%u", *(string+i));

     if (i != width - 3)
       printf(" ");
 }
 printf("\n");
}

// copy SRC into DEST
void copyRowValues(char* dest, char* src, int width) {
  for (int i = 0; i < width; i++) {
    *(dest+i) = *(src+i);
  }
}

// get value of 5 bits (leftmost bit is 16, etc.)
int getRuleIndex(int* bits) {
 int i, sum = 0, power = 16;

 for (i = 0; i < 5; i++) {
   //printf("%d bits :)\n", bits[i]);
   sum += power * *(bits+i);
   power /= 2;
 }
 return sum;
}

/*
 * Takes in a `rule` and number of `rows` and prints the cellular automata simulation in PBM format
 */
int main(int argc, char ** argv)
{

    char *rowA, *rowB, **endptr;
    u_int32_t rule, rows;
    int* fiveBits = calloc(5, sizeof(int));
    char* ruleBinaryString = malloc(33);
    strcpy(ruleBinaryString, ""); 
    /*
     * PART 1: USING COMMAND LINE ARGUMENTS
     * ====================================
     * When using command line arguments, its important to perform validation to prevent improper usage.
     * In Task 1, check that the number of arguments the user has provided is correct.
     *
     * In C, all command line arguments are provided as strings, but we need our arguments as integers.
     * In Task 2, finish the conversion from strings to unsigned ints using `strtoul().
     * `strtoul` converts a string (str) to (to) an unsigned long (ul).
     * Try `man strtoul` if you want more information about what this function does.
     */

    /* PART 1, TASK 1: Check that the number of arguments is correct */
    if (argc != 3) {
      usage();
    }
    // YOUR CODE HERE

    /* PART 1, TASK 2: Convert `rule` and `rows` from a strings (char *) to an unsigned ints */

    char * rule_str;
    // YOUR CODE HERE: set rule_str to the appropriate string
    rule_str = *(argv+1);
    rule = (unsigned int) strtoul(rule_str, (endptr = argv), 10);
    if(**endptr)
        usage();

    char * rows_str;
    // YOUR CODE HERE: set row_str to the appropriate string
    rows_str = *(argv+2);
    rows = (unsigned int) strtoul(rows_str, (endptr = argv), 10);
    if(**endptr)
        usage();

    int i, width = 2 * rows + 1, height = rows, paddedWidth = 2 * rows + 5, j, index, ruleIndex, bit;

    /*
     * PART 2: USING FORMATTING ARGUMENTS & PRINTF
     * ===========================================
     * A properly formatted PBM file starts with P1 <width> <height>
     * Write code that will print "P1 <width> <height> ## (Rule <rule>)", followed by a newline
     *
     * Example:
     * P1 11 6 ## 5 rows of automata simulation (Rule 450)
     * ...
     */

    // YOUR CODE HERE
    //printf("%s", rule_str); 
    printf("P1 %i %i ## %u rows of automata simulation (Rule %u)\n", width, height+1, rows, rule);
    /*
     * PART 3: MEMORY MANAGEMENT
     * =========================
     * (There is no task for Part 3, but please read through the code & any accompanying
     * comments: you should fully understand everything that is being done here)
     */

    /*
     * Allocates two chunks of memory (`rowA` and `rowB`), each with space for a single generation
     * of the cellular automata, along with two elements worth of padding on either side. Because
     * these chunks are allocated with `calloc()`, the space is set to 0.
     */
    rowA = calloc(2 * rows + 5, 1);
    rowB = calloc(2 * rows + 5, 1);

    /*
     * Check to see that memory allocation has been successful.
     * If not, write "Out of memory" to STDERR and exit with error code 1.
     */
    if(!(rowA && rowB && rule_str && fiveBits && ruleBinaryString)) {
      fprintf(stderr, "Error allocating rowA, rowB, rule_str, ruleBinaryString, and/or fiveBits.\n");
      exit(1);
    }

    /*
     * PART 4: C CODING & (hint!) BITWISE OPERATIONS
     * =============================================
     * You must now generate and print the initial generation and each successive generation.
     */

    // YOUR CODE HERE
     getBinaryString(&ruleBinaryString, rule);
     *(rowA+rows+2) = 1; // get initial row

     for (; height >= 0; height--) {
       printBinaryString(rowA, paddedWidth);
       //iterate through all indices/blocks
       for (i = 2; i < paddedWidth - 2; i++) {
         //get the 5 bis
         index = 0;
         for (j = i-2; j <= i+2; j++) { 
           fiveBits[index] = *(rowA+j);
           //printf("%d bit\n" ,fiveBits[index]);
           index += 1;
         }
         ruleIndex = getRuleIndex(fiveBits);
         //printf("%i index\n", ruleIndex);
         //printf("%s\n", ruleBinaryString);
         bit = ruleBinaryString[31 - ruleIndex] - 48;
        // printf("%i bit \n", bit);
         //printf("\n");
         *(rowB + i) = bit;
       }
       //printBinaryString(rowB, width);
       copyRowValues(rowA, rowB, paddedWidth);
    }


    /* PART 3.5: MEMORY MANAGEMENT, PT. 2
     * (There is no task for Part 3, but please read through the code & any accompanying
     * comments: you should fully understand everything that is being done here)
     * Free initialized memory and exit successfully.
     */
    free(rowA);
    free(rowB);
    free(fiveBits);
    free(ruleBinaryString);

    return 0;

}
