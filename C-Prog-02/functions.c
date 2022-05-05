/*
 *
 *      ECE474 introductory C programming assignment  #2
 *      Spring 2022
 *
 *      Student Name: Illya Kuzmych
 *      Student ID: 2127069
 *
 *
 */

/*  Objectives:
       Gain experience and proficiency with C programming.


Format of assignment:
  1)  make a backup copy of this file
  2)  enter your name and ID# into the space above
  3)  Read through the comments below and
edit code into this file to solve the problems.
  4)  Run the tests in main();
  4)  check your results against posted solution

#define constants are NOT required for this assignment but MAY be used.

Printing:  use printf() to print
    printf(const char *format, ...)
    format - tags prototype is %[flags][width][.precision][length]specifier
    flags:
    d - Signed decimal integer
    f - Decimal floating point
    s - String of characters

*/
#include "functions.h"

/*************************************************************************
   Part 2.1:  Bitwise operations
*************************************************************************/
/*
 * Some terminology:
 * SETTING a bit means that if K-th bit is 0, then set it to 1 and if it
 * is 1 then leave it unchanged (in other words, a bit is SET if it is 1)
 *
 * CLEARING a bit means that if K-th bit is 1, then clear it to 0 and if
 * it is 0 then leave it unchanged (a bit is CLEAR if it is 0)
 *
 * COMPLEMENTING or TOGGLING a bit means that if K-th bit is 1, then
 * change it to 0 and if it is 0 then change it to 1.
*/


/*
 * Function 2.1 mangle
 * The function mangle will take your student ID and alter it in
 * ways that depend on the binary representation of it as an integer.
 *
 * Specifications:
 *      1) right shift the ID by 2 positions
 *      2) clear bit 6 (counting from the LSB=0)
 *      3) complement bit 3 (If bit 3 is one, make it 0. If bit 3 is 0, make it 1.)
 *
 *   Example output (try to do the bitwise math yourself to see if this adds up):
 *
 *   Part 2.1: Enter your UW Student ID number:51218
 *   You entered 51218
 *   your mangled SID is 12812
 */

int mangle(int sid) {
    int mangled_id = 0;
    
    mangled_id = (sid >> 2);
    mangled_id &= ~(1 << 6);
    mangled_id ^= 1 << 3;

    return mangled_id;
}

// Part 2.1.1  More bit manipulation
/* The function bit_check(int data, int bits_on, int bits_off), will check an int to see if a
 * specific mask of  bits is set AND that another bit mask is clear.  Returns 1 if matches
 * and 0 if not.  Return -1 for the degenerate case of testing for BOTH off and on for any bit
 *
 *  Pseudocode examples of some masks and what they are checking  (Using 4-bit #'s for clarity)
 *
 *  bits_on = binary(0011) - this mask means that we are checking if the last two LSBs
 *                           (e.g. 00XX, the X bits) of the data are SET (1).
 *  bits_off= binary(0100) - this mask means that we are checking if the third LSB
 *                           (e.g. 0X00, the X bit) of the data is CLEAR (0).
 *
 *  example of returning -1:
 *  bits_on = binary(0110)
 *  bits_off= binary(0011)
 *  here the second LSB (00X0, the X bit) has to be both SET and CLEAR to pass according to the
 *  masks, which is impossible - in this case we return a -1 for invalid mask combinations.
 *
 *
 *  Again in pseudocode, some examples of actual checks/values that the test code would do: (Using 4-bit #'s for clarity)
 *
 *  int d = binary( 1100)  // pseudocode
 *  int onmask1 =  binary(0011)  // will fail
 *  int offmask1 = binary(0100)  // will fail
 *  int onmask2 =  binary(0100)  // succeed
 *  int offmask2 = binary(0001)  // succeed
 *
 *  bit_check(d,onmask1,offmask1) --> 0
 *  bit_check(d,onmask1,offmask2) --> -1
 *  bit_check(d,onmask2,offmask2) --> 1
 *  bit_check(d,offmask1,offmask1) --> -1 // contradictory
 */


int  bit_check(int data, int bits_on, int bits_off) {
    
    if ((bits_on & bits_off) >= 1) {
        return -1;
    } else if (((data & bits_on) == bits_on) && ((data & ~bits_off) == data)) { // if data and bits_on are the same, then it worked. 
        return 1;                                                               // or if data and not bits off are the same, then it also works
    }
    
    return 0;
}

/*************************************************************************
   Part 2.2:  Basic Pointer declaration and usage
*************************************************************************/
char a_array[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

/* pmatch()
 * This function will return a pointer to the element in a pre-defined
 * array which matches it's character arg. (accepts capital letters only).
 * If the character cannot be found, return the NULL pointer.
 *
 * Example: pmatch('A') should return a pointer to 'A' in a_array
 * Dereferencing that pointer should then print 'A'
 */

char * pmatch(char c) {
    
    for (int i = 0; i < 26; i++) {
        if (a_array[i] == c) {
            return &a_array[i];
        }
    }
    
    return NULL;
}


/*************************************************************************
   Part 2.3:  Pointer Arithmetic
*************************************************************************/


/* 2.3.1 Next letter:
 * char nlet(char * ptr)
 * ptr is a pointer returned by pmatch()
 * This function will return the next letter of the alphabet (Not
 * a pointer to the next letter of the alphabet) unless
 * the pointer points to 'Z'. In that case it will return -1.
 * If the argument does not point to a capital letter A-Y, return -1.
 */

char nlet(char *ptr) {
    if(*ptr == 'Z')
        return -1;

    return *(ptr + 1);
}

/*
 * 2.3.2 Pointer math II
 *
 * ldif(c1,c2)  Find the alphabet distance between two letters using pointers.
 * Example:     Ldif('A','E') -> 4
 *
 * if either character is not a capital letter, return any negative number < -26
 */

int ldif(char c1, char c2) {
    
    int difference; 
    int CountC1 = 28;
    int CountC2 = 28;
    int badReturn = -9999999;

    for (int i = 0; i < 25; i++) {
        if ((a_array[i] == c1)) {
            CountC1 = i;
        }
    }
    
    for (int j = 0; j < 25; j++) {
        if ((a_array[j] == c2)) {
            CountC2 = j;
        }
    }
    
    if ((CountC1 == 28) || (CountC2 == 28)) {
        return badReturn;
    }

    difference = abs(CountC2 - CountC1);
    
    return difference;
}


/*************************************************************************
   Part 2.4   struct and sizeof
*************************************************************************/

/* Part 2.4.1
 * Create a struct in "functions.h" to represent a custom datatype Person.
 * Modify the existing declaration "typedef struct Person {..." to contain:
 *     1) a 20 char string: FirstName
 *     2) a 30 char string: LastName
 *     3) an 80 char string: StreetAddr
 *     4) a 6 char string: ZipCode 
 *        (a zip code if 5 characters think why might we want 6?)
 *     5) double: Height      // height in meters
 *     6) float: Weight       // weight in kg
 *     7) long int: DBirth    // birthday (days since 1-Jan-1900)
 *
 * NOTE: The existing declaration shows the general syntax of creating a
 * struct but is just a placeholder with the correct fields so the code
 * will compile and the tests will run.
 */


/*
 * Part 2.4.2
 * Return the number of bytes required to hold person struct
 *
 * NOTE: depending on your machine, setup and compiler, the size would be 
 * different for everyone. Look up 'structure padding in C' to see why 
 * - the computer processes data in the minimum of 4 or 8 bytes for 32/64 
 * bit processors.
 */
int personSize(Person p) {
    return sizeof(Person);
}
/*
 * Part 2.4.3
 * Write a function: per_print(person * p)  which prints out a formatted person.
 * If the person's address is longer than 60 characters, truncate it to 60 characters
 * but do not erase memory of the last 20 characters if present.

 OUTPUT EXAMPLE:
     --- person report: ---
    First Name:       Blake
    Last Name:        Hannaford
    Address:          124 N. Anystreet / Busytown, WA
    Zip:              99499
    Height:            1.97
    Weight:           81.82
    DOB 1/1/1900:     34780
    ---   ---   ---
*/
void per_print(Person *p) {
    printf("Function per_print not implemented\n");
}
