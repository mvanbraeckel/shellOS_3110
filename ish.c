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

const char *mypath[] = { "./", "/usr/bin/", "/bin/", NULL};
extern char **getln();

void signalHandler(int signalPassed)
{
    wait(NULL);
    // if system call interrupted it, reset 
    if(errno == EINTR) {
        errno = 0;
    }
}

int main() {
    // declare variables
    int i, j, c, argc, hasAmp;
    char **args;
    struct passwd *password;
    char hostname[_SC_HOST_NAME_MAX+1];
    char prompt = '$';
    pid_t pid;
    
    // infinitely loop until "exit" is entered
    while (1) {
        // get user ID in text format from password struct
        password = getpwuid(getuid()); // set errno to 0 before call, then check after
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
        } else {
            prompt = '$';
        }
        fprintf(stdout, "[%s@%s]%c ", password->pw_name, hostname, prompt);
        //free(cwd);

        // use lex to parse shell prompt input
        args = getln();

        argc = 0;
        // find number of arguments
        for(argc = 0; args[argc] != NULL; argc++);
        argc--;

        // check for ampersand at the end
        hasAmp = 0;
        if(strcmp(args[argc], "&") == 0) {
            hasAmp = 1;
        }

        // check for command
        if(args[0] != NULL) {
            if(strcmp(args[0], "exit") == 0) {
                exit(EXIT_SUCCESS);

            } else if(strcmp(args[0], "args") == 0) {
                // count and print number of arguments (other than "args")
                /*argc = 0;
                for(argc = 1; args[argc] != NULL; argc++);
                fprintf(stdout, "argc = %d, args = ", argc-1);
                // print all other arguments separated by ", "
                for(j = 1; args[j] != NULL; j++) {
                    // remove '\n'
                    c = 0;
                    while(args[j][c] != '\n' && args[j][c] != '\0') {
                        if(args[j][c] == '\n') {
                            args[j][c] = '\0';
                        }
                        c++;
                    }
                    // print the argument
                    if(j == 1) {
                        fprintf(stdout, "%s", args[j]);
                    } else {
                        fprintf(stdout, ", %s", args[j]);
                    }
                }
                fprintf(stdout, "\n");*/
                // TODO - "text, text" needs to be treated as 1 instead of 2
            }
        }

        // print all the arguments from lex
        for(i = 0; args[i] != NULL; i++) {
            fprintf(stdout, "arg[%d]: %s\n", i, args[i]);
        }
        
        /* If necessary locate executable using mypath array */

        // Launch executable
        sigset(SIGCHLD, SIG_IGN);
        pid = fork();
        if(pid < 0) {
            fprintf(stderr, "Fork failed\n");
            exit(EXIT_FAILURE);

        } else if(pid == 0) { // child process
            if(strcmp(args[0], "gcd") == 0) {
                if(execvp("./gcd", args) == -1) { // need to check errno :TODO
                    exit(EXIT_FAILURE);
                }
            } else{
                if(execvp(args[0], args) == -1) { // need to check errno :TODO
                    exit(EXIT_FAILURE);
                }
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

/**
 * Flushes all leftover data in the stream
 * @param char* input - the string that was just read from stdin
 */
/*void flush_input(char* input) {
    // if the '\n' is NOT found in the word itself, flush the stream
    if(strchr(input, '\n') == NULL) {
        while(getchar() != '\n');
        input[strlen(input)] = '\0';
    } else {
        input[strlen(input)-1] = '\0';
    }
}*/
