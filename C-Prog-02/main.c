/*
 *
 *      ECE474 introductory C programming assignment  #2
 *      Spring 2022
 *
 *      TESTING CODE DO NOT MODIFY THIS FILE
 *      WRITE YOUR CODE IN FUNCTIONS.C AND FUNCTIONS.H
 */

/*
#define constants are NOT required for this assignment but MAY be used.

Printing:  make your own "printf()" statements in this assignment.

*/
/*  #include some system library function prototypes */
      //  we will use printf()
#include <stdio.h>
#include <math.h>
#include <string.h>    // for strlen
#include <stdlib.h>    // for malloc() and random()
#include "functions.h"


/*************************************************************************
main() runS all demos and tests of the parts of assignment 2.

Use these to test your code.  Do not modify the code below.
See sample output file.
*************************************************************************/


int main(){
    int sid;   // student ID #

    // Part 2.1
    printf( "Part 2.1: Enter your UW Student ID number:");
    scanf("%d", &sid);
    printf(" You entered %d\n",sid);
    int msid =  mangle(sid);
    printf(" your mangled SID is %d\n\n", msid);


    // Part 2.1.2
    printf("Part 2.1.2: Check the bit_check() function\n");
    int onm1 = 3;
    int ofm1 = 4;
    int onm2 = 4;
    int ofm2 = 1; // same as example above
    int d = 0x14; // (1100)

    // The examples
    printf("d = %d Hex: %x, Octal: %o\n", d,d,d);
    printf("bit_check(d,onm1,ofm1): %d\n", bit_check(d,onm1,ofm1));  // example 1
    printf("bit_check(d,onm1,ofm2): %d\n", bit_check(d,onm1,ofm2));
    printf("bit_check(d,onm2,ofm2): %d\n", bit_check(d,onm2,ofm2));
    printf("bit_check(d,ofm1,ofm1): %d\n", bit_check(d,ofm1,ofm1));  // example 4
    printf("bit_check(129753, 129753, ~129753): %d\n", bit_check(129753, 129753, ~129753)); // 1

    d = 32768+4096+64;
    printf("d = %d Hex: %x, Octal: %o\n", d,d,d);
    printf("bit_check(32768+4096+64, 4096+64, 16384): %d\n", bit_check(32768+4096+64, 4096+64, 16384)); // 1
    printf("bit_check(32768+4096+64, 4096+64, 32768): %d\n", bit_check(32768+4096+64, 4096+64, 32768)); // 0
    printf("\n");

    // Part 2.2
    char ctest;
    printf(" Part 2.2.1: Enter a capital letter: ");
    scanf(" %c", &ctest);
    char * ptest = pmatch(ctest);
    if (ptest != NULL){
        printf(" you entered: %1c\n", *ptest);
        int nl = nlet(ptest);
        if (nl != -1){
            printf (" Part 2.2.2: The next letter after %1c is %c\n\n", *ptest, nlet(ptest));
        }
    }
    else { 
        printf(" you did NOT enter a capital letter!\n\n");
    }
    ctest = 'Z';
    ptest = &ctest;
    printf (" Part 2.2.2: The next letter after %1c is %d\n\n", *ptest, nlet(ptest));
    printf (" Part 2.2.3: M and Q are %d positions apart\n", ldif('M', 'Q'));
    printf (" Part 2.3.3: x and Q are %d positions apart\n\n", ldif('x', 'Q'));


    // Part 2.4   Struct and Sizeof
    // Example of declaring and initializing values in a struct
    Person testp = {
        .FirstName="",
        .LastName="",
        .StreetAddr="",
        .ZipCode="",
        .Height=0.0,
        .Weight=0.0,
        .DBirth=0
    };

    int persize = personSize(testp);
    printf ("Part 2.4.1:  Size of a Person is %d\n",persize);

    // Check if person has been defined and add some data
    // The instructions and test cases are slightly ambiguous about whether a
    // "20 character string" includes the null terminator, so at least make
    // sure we can store 19 characters for FirstName, etc.
    if(sizeof(testp.FirstName) > 19){
        strcpy(testp.FirstName, "Blake");
    } else{
        printf ("Person not defined correctly\n");
    }

    if(sizeof(testp.LastName) > 29){
        strcpy(testp.LastName, "Hannaford");
    } else{
        printf ("Person not defined correctly\n");
    }

    if(sizeof(testp.StreetAddr) > 78){
        strcpy(testp.StreetAddr, "124 N. Anystreet / Busytown, WA");
    } else{
        printf ("Person not defined correctly\n");
    }

    if(sizeof(testp.ZipCode) == 6){
        strcpy(testp.ZipCode, "99499"); 
    } else{
        printf ("Person not defined correctly\n");
    }

    testp.Height = 1.97;
    testp.Weight = 180.0/2.2;  // 180 lbs in kg
    // born 24-Mar-1995 (see  https://www.timeanddate.com/date/duration.html)
    testp.DBirth = 34780;
    
    // Check size again after adding data
    persize = personSize(testp);
    printf ("Part 2.4.1:  Size of a Person is STILL %d\n",persize);
    printf ("Part 2.4.2:  Our test person is:\n");
    per_print(&testp);
    // Now we move to a place w/ really long address!
    if(sizeof(testp.StreetAddr) > 78){
        strcpy(testp.StreetAddr,"12903010542039840 NW Antidisetablishmentarianism Street, Apt.5/AnyTown, U.S.A.");
        printf("\n The new Address is 79 characters!  Too long for 60\n");
    } else{
        printf ("Person not defined correctly\n");
    }
    per_print(&testp);

    return 0;
}
