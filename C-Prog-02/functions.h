#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
 *
 *      ECE474 introductory C programming assignment  #2
 *      Spring 2021
 *
 *      Student Name: Illya Kuzmych
 *      Student ID: 2127069
 *
 *
 */

#include <stdio.h>
#include <stdint.h>

//DEFINE YOUR CONSTANTS HERE

/*************************************************************************
   Part 2.1:  Bitwise operations
*************************************************************************/

int mangle(int);

int  bit_check(int, int, int);

/*************************************************************************
   Part 2.2:  Basic Pointer declaration and usage
*************************************************************************/
char * pmatch(char);


/*************************************************************************
   Part 2.3:  Pointer Arithmetic
*************************************************************************/
char nlet(char*);

int ldif(char, char);

/*************************************************************************
   Part 2.4   struct and sizeof
*************************************************************************/

typedef struct Person {
    char FirstName[21];
    char LastName[31];
    char StreetAddr[81];
    char ZipCode[6];
    double Height;
    float Weight;
    long int DBirth;
} Person ;

int personSize(Person);

void per_print(Person*);

#endif // FUNCTIONS_H_INCLUDED
