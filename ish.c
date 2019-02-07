/**
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 100297
 * 06/02/2019
 * CIS*3110: Operating Systems A1 - simple shell
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// ================================= FUNCTION PROTOTYPES =================================
void signalHandler(int signalPassed);

void gcd(int argc, char* argv[]);
long calcGCD(long a, long b);
int isHex(char* strNum);
void listargs(int argc, char* argv[]);
void lcm(int argc, char* arv[]);
long calcLCM(long a, long b);

// =======================================================================================

extern char **getln();
//const char *mypath[] = { "./", "/usr/bin/", "/bin/", NULL};
const char myShellName[] = "vbshell";

int main() {
    // declare variables
    char hostname[_SC_HOST_NAME_MAX+1];
    
    // infinitely loop until "exit" is entered
    while(1) {
        char prompt = '$';
        // get user ID in text format from password struct
        struct passwd *password = getpwuid(getuid()); // set errno to 0 before call, then check after
        if(password == NULL) {
            fprintf(stderr, "password is null\n");
            exit(EXIT_FAILURE);
        } 

        // get host name
        if(gethostname(hostname, sizeof(hostname)) == -1) { // need to check errno for this too
            fprintf(stderr, "gethostname returned an error\n");
            exit(EXIT_FAILURE);
        }
        hostname[_SC_HOST_NAME_MAX] = '\0';
        /*// get current working directory
        char *cwd;
        cwd = getcwd(0,0);
        if(!cwd) { // check errno for this too
            fprintf(stderr, "getcwd failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }*/

        // display prompt - '$' for user, '#' for superuser or root
        if(getuid() == 0) {
            prompt = '#';
        }
        fprintf(stdout, "[%s@%s]%c ", password->pw_name, hostname, prompt);
        //free(cwd);

        // use lex to parse shell prompt input, then count #of arguments
        char **args = getln();

        int argc = 0;
        while( args[argc] != NULL) argc++;
        if(argc <= 0) continue; //skip because no arguments

        printf("\t#args = %d\n", argc);
        int hasAmp = 0;
        int writeOut = 0;
        int readIn = 0;
        // check for ampersand at the end
        if(strcmp(args[argc-1], "&") == 0) {
            hasAmp = 1;
        }
        // check for file redirect
        if(argc >= 3) {
            if(strcmp(args[argc-2], ">") == 0) {
                writeOut = 1;
            } else if(strcmp(args[argc-2], "<") == 0) {
                readIn = 1;
            }
        }

        // check for internal/built-in command
        if(args[0] != NULL) {
            if(argc == 1 && strcmp(args[0], "exit") == 0) {
                exit(EXIT_SUCCESS);

            } else if(strcmp(args[0], "args") == 0) {
                listargs(argc, args);
                continue; //skip to next loop

            } else if(strcmp(args[0], "gcd") == 0) {
                gcd(argc, args);
                continue; //skip to next loop

            } else if(strcmp(args[0], "lcm") == 0) {
                lcm(argc, args);
                continue; //skip to next loop
            }
        }

        // print all the arguments from lex
        for(int i = 0; args[i] != NULL; i++) {
            fprintf(stdout, "arg[%d]: %s\n", i, args[i]);
        }
        
        /* If necessary locate executable using mypath array */

        // Launch executable
        sigset(SIGCHLD, SIG_IGN);
        pid_t pid = fork();

        if(pid < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(EXIT_FAILURE);

        } else if(pid == 0) { // child process
            if(execvp(args[0], args) == -1) { // need to check errno :TODO
                fprintf(stderr, "%s: %s: ", myShellName, args[0]);
                perror("");
                exit(EXIT_FAILURE);
            }
                 
        } else { // parent process (waits for child to finish)
            if(hasAmp) {
                sigset(SIGCHLD, signalHandler);
            } else {
                wait(NULL); // check error TODO
            }
            //printf("child exited/completed\n");

        } // end of if

    } // end infinite while loop

    return 0;
}

// ======================================= HELPERS =======================================

void signalHandler(int signalPassed) {
    wait(NULL);
    // if system call interrupted it, reset 
    if(errno == EINTR) {
        errno = 0;
    }
}

// ========================================= GCD =========================================

/**
 * Built-in/Internal Command: Displays the greatest common denominator of two (decimal or hexadecimal) numbers
 * NOTE: I use longs for calculations even though I say integer, same things tho, just bigger
 * @param int argc -the number of arguments
 * @param char* argv[] -used for the array of arguments
 */
void gcd(int argc, char* argv[]) {
    // check that proper #of arguments were inputted
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <integer number 1> <integer number 2>\n", argv[0]);
        return;
    }

    // declare variables
    int hexCheckNum1, hexCheckNum2;
    long num1, num2, ans;
    char *ptr1, *ptr2;

    // check is its a hex number, set it to 16, otherwise 10 (decimal by default)
    hexCheckNum1 = isHex(argv[1]);
    hexCheckNum2 = isHex(argv[2]);

    // convert arguments to integers (different conversion for hex or decimal)
    num1 = strtol(argv[1], &ptr1, hexCheckNum1);
    num2 = strtol(argv[2], &ptr2, hexCheckNum2);

    // check that both numbers are valid integers in hex or decimal
    if(strcmp(ptr1, "") != 0 || strcmp(ptr2, "") != 0) {
        // display error msg before exiting
        fprintf(stderr, "Error: arguments must be valid integers in decimal or hexadecimal (0x0) format\n");
        fprintf(stderr, "Usage: %s <integer number 1> <integer number 2>\n", argv[0]);
        return;
    }

    // calculate GCD: if negative, make positive (because it makes sense)
    ans = calcGCD(num1, num2);
    if(ans < 0) {
        ans *= -1;
    }

    // print output
    fprintf(stdout, "GCD(%s, %s) = %ld\n", argv[1], argv[2], ans);
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
 * Checks if a number is in hexadecimal form
 * @param char* strNum -the number to be checked
 * @return 16 if it's a hexadecimal number, 10 otherwise (default decimal)
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

// ======================================== ARGS =========================================

/**
 * Built-in/Internal Command: Counts the total number of arguments passed to "args" command
 * --> Prints the arguments delimited by ", "
 * @param int argc -the number of arguments
 * @param char* argv[] -used for the array of arguments
 */
void listargs(int argc, char* argv[]) {
    // check that proper #of arguments were inputted (>1)
    if(argc == 1) {
        fprintf(stderr, "Usage: %s [arguments]+\n", argv[0]);
        return;
    }
    // count and print number of arguments (other than "args")
    fprintf(stdout, "argc = %d, args = ", argc-1);
    // print all other arguments separated by ", "
    for(int i = 1; argv[i] != NULL; i++) {
        if(i == 1) {
            fprintf(stdout, "%s", argv[i]);
        } else {
            fprintf(stdout, ", %s", argv[i]);
        }
    }
    fprintf(stdout, "\n");
}

// ========================================= LCM =========================================

/**
 * Built-in/Internal Command: Displays the lowest common multiple of two (decimal or hexadecimal) numbers
 * NOTE: I use longs for calculations even though I say integer, same things tho, just bigger
 * @param int argc -the number of arguments
 * @param char* argv[] -used for the array of arguments
 */
void lcm(int argc, char* argv[]) {
    // check that proper #of arguments were inputted
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <integer number 1> <integer number 2>\n", argv[0]);
        return;
    }

    // declare variables
    int hexCheckNum1, hexCheckNum2;
    long num1, num2, ans;
    char *ptr1, *ptr2;

    // check is its a hex number, set it to 16, otherwise 10 (decimal by default)
    hexCheckNum1 = isHex(argv[1]);
    hexCheckNum2 = isHex(argv[2]);

    // convert arguments to integers (different conversion for hex or decimal)
    num1 = strtol(argv[1], &ptr1, hexCheckNum1);
    num2 = strtol(argv[2], &ptr2, hexCheckNum2);

    // check that both numbers are valid integers in hex or decimal
    if(strcmp(ptr1, "") != 0 || strcmp(ptr2, "") != 0 || argv[1][0] == '-' || argv[2][0] == '-') {
        // display error msg before exiting
        fprintf(stderr, "Error: arguments must be valid positive integers in decimal or " \
                "hexadecimal (0x0) format\n");
        fprintf(stderr, "Usage: %s <positive integer 1> <positive integer 2>\n", argv[0]);
        return;
    }

    // calculate LCM
    ans = calcLCM(num1, num2);

    // print output
    fprintf(stdout, "LCM(%s, %s) = %ld\n", argv[1], argv[2], ans);
}

/**
 * Calculates the lowest common multiple of two numbers using Euclid's GCD Algorithm
 * @param long a -the first number
 * @param long b -the second number
 * @return the lowest common multiple of the two numbers a and b
 */
long calcLCM(long a, long b) {
    return a * b / calcGCD(a, b);
}

// =======================================================================================
