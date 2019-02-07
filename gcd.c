/**
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 100297
 * 06/02/2019
 * CIS*3110: Operating Systems A1 - simple shell
 *  --> displays the greatest common denominator of two (decimal or hexadecimal) numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// inlcudes
long calcGCD(long a, long b);
int isValid(char* strNum);

int main(int argc, char* argv[]) {
    // check that proper #of arguments were inputted
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <positive integer 1> <positive integer 2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // declare variables
    int checkNum1, checkNum2;
    long num1, num2, ans;

    // check that both numbers are valid integers in hex or decimal
    checkNum1 = isValid(argv[1]);
    checkNum2 = isValid(argv[2]);

    if(!checkNum1 || !checkNum2) {
        // display error msg before exiting
        fprintf(stderr, "Error: arguments must be valid positive integers in decimal or " \
                "hexadecimal (0x0) format\n");
        exit(EXIT_FAILURE);
    }

    // convert arguments to integers (different conversion for hex or decimal)
    num1 = strtol(argv[1], NULL, checkNum1);
    num2 = strtol(argv[2], NULL, checkNum2);

    // calculate GCD
    ans = calcGCD(num1, num2);

    // print output
    fprintf(stdout, "%s = %ld\n", argv[1], num1);
    fprintf(stdout, "%s = %ld\n", argv[2], num2);
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
int isValid(char* strNum) {
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
}