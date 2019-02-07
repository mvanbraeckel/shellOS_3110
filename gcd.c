/**
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 100297
 * 06/02/2019
 * CIS*3110: Operating Systems A1 - simple shell
 *  --> displays the greatest common denominator of two (decimal or hexadecimal) numbers
 * NOTE: I use longs for calculations even though I say integer, same things tho, just bigger
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// inlcudes
long calcGCD(long a, long b);
int isHex(char* strNum);

int main(int argc, char* argv[]) {
    // check that proper #of arguments were inputted
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <integer number 1> <integer number 2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // declare variables
    int hexCheckNum1, hexCheckNum2;
    long num1, num2, ans;
    char *ptr1, *ptr2;

    hexCheckNum1 = isHex(argv[1]);
    hexCheckNum2 = isHex(argv[2]);

    // convert arguments to integers (different conversion for hex or decimal)
    num1 = strtol(argv[1], &ptr1, hexCheckNum1);
    num2 = strtol(argv[2], &ptr2, hexCheckNum2);

    // check that both numbers are valid integers in hex or decimal
    if(strcmp(ptr1, "") != 0 || strcmp(ptr2, "") != 0) {
        // display error msg before exiting
        fprintf(stderr, "Error: arguments must be valid positive integers in decimal or " \
                "hexadecimal (0x0) format\n");
        fprintf(stderr, "Usage: %s <integer number 1> <integer number 2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // calculate GCD: if negative, make positive
    ans = calcGCD(num1, num2);
    if(ans < 0) {
        ans *= -1;
    }

    // print output
    fprintf(stdout, "GCD(%s, %s) = %ld\n", argv[1], argv[2], ans);

    return 0;
}

/**
 * Calculates the greatest common divisor of two numbers using Euclid's GCD Algorithm
 * @param long a -the first number
 * @param long b -the second number
 * @return the greatest common divisor of the two numbers a and b
 */
long calcGCD(long a, long b) {
    if(b == 0) {
        return a;
    } else {
        return calcGCD(b, a % b);
    }
}

/**
 * Checks if a number in string form is a valid decimal or hexadecimal (first 2 chars are "0x") number
 * @param char* strNum -the inputted number to bec validated
 * @return 0 if it's not valid, 16 if it's a valid hexadecimal number, 10 for valid decimal number
 */
/*int isValid(char* strNum) {
    // declare variables
    int i;
    int xFound = 0; //false

    for(i = 0; i < strlen(strNum); i++) {
        // must all be digits
        if(isdigit(strNum[i])) {
            continue;

        } else { // not a digit
            // exception for hex #s -> it's first 2 chars are a '0x' sequence
            if(strNum[i] == 'x' && !xFound && i == 1 && strNum[i-1] == '0') {
                xFound = 1; //set to true
                // must be followed by at least one digit
                if(isdigit(strNum[i+1])) {
                    continue;
                }
            }
            // wasn't valid hexadecimal form
            return 0; 
        }
    } // end for loop

    if(xFound) {
        return 16; // valid hex
    } else {
        return 10; // valid decimal
    }
}*/

/**
 * Checks if a number is in hexadecimal form
 * @param char* strNum -the number to be checked
 * @return 16 if it's a hex number, 10 otherwise
 */
int isHex(char* strNum) {
    // is of hex form if the 'x' is in the right spot
    // account for negatives
    int len = strlen(strNum);
    if(len > 0 && strNum[0] == '-') {
        if(len > 2 && strNum[2] == 'x') {
            return 16; // negative hex
        }
    } else if(len > 1 && strNum[1] == 'x') {
        return 16; // hex
    }
    return 10;
}