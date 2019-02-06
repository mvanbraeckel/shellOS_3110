/**
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 100297
 * 06/02/2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#include <string.h>
#include <strings.h>

const char *mypath[] = { "./", "/usr/bin/", "/bin/", NULL};

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

extern char **getln();

int main(int argc, char *argv[]) {
    int i;
    char **args;
    
    // loop until "exit" is entered
    while (1) {
        // get user ID in text format from password struct
        struct passwd *password = getpwuid(getuid()); // set errno to 0 before call, then check after
        if(password == NULL) {

            fprintf(stderr, "password is null\n");

        } 
        // get host name
        char hostname[_SC_HOST_NAME_MAX+1];
        if(gethostname(hostname, sizeof(hostname)) == -1) { // need to check errno for this too
            
            fprintf(stderr, "gethostname returned an error\n");

        }
        hostname[_SC_HOST_NAME_MAX] = '\0';
        
        fprintf(stdout, "[%s@%s]$ ", password->pw_name, hostname);

        // use lex to parse shell prompt input
        args = getln();

        if(args[0] != NULL) {
            if(strcasecmp(args[0], "exit") == 0) {
            
                fprintf(stdout, "time to exit!\n");

                exit(EXIT_SUCCESS);
            } else if(strcasecmp(args[0], "args") == 0) {
                
            }
        }

        for(i = 0; args[i] != NULL; i++) {
            fprintf(stdout, "arg[%d]: %s\n", i, args[i]);
        }
        
        /* Parse input */
        /*while (getln() != NULL) {
            ...
        }*/

        /* If necessary locate executable using mypath array */

        /* Launch executable */
        /*if (fork () == 0) {
            // execute command
        } else {
            wait(NULL);
        } // end of if*/

    }

    return 0;
}