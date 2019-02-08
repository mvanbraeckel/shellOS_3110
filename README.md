# shellOS_3110 A1
Simple shell for OS (Operating Systems course A1) "vbshell"

// Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
// 07/02/2019
// CIS*3110: Operating Systems Assignment 1
// A brief report about my assinment

LINKS USED:
- man pages (especially for signal related things, i.e. sigset, sigactio, etc.)
- https://courselink.uoguelph.ca/d2l/le/content/550369/Home
- https://www.geeksforgeeks.org/zombie-processes-prevention/
- http://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
- https://stackoverflow.com/questions/50280498/how-to-only-kill-the-child-process-in-the-foreground?fbclid=IwAR3pmr6csA0gT0yvCAnDYu0Q8XEoaVJwyaIOVegXKnE_zVu62X1aKlUYkfk
- https://docs.oracle.com/cd/E19455-01/806-4750/signals-7/index.html?fbclid=IwAR1fzKDNU-49mrHsO4z6wWvw8bAXp3_cYInEkzVn87xzloITcfwH3i2Qt24
- https://stackoverflow.com/questions/12663270/freopen-and-execvp-in-c

NOTE: This was made in 2 days (thank you Canada for clutch snowday that pushed back this deadline 24 hours). I lost all my work since my VM broke and won't start-up again. All recovery methods I have tried also did not work. I have done as much as I possibly can in the time I had. If I didn't lose all my work up until Wednesday my shell would have been much better. Unfortunately, I was naive and didn't backup my work anywhere since I've never had problems before. I learned my lesson now and have backed-up to git since then (as seen by my auto git add, commit, push sequence added to Makefile). Due to time constraints, I have "check[ed] and correctly handle[d] the return values of Every system call" as stated in the description for theis assignment. Since it never said anything, I have assumed that we did not need to check errno if it wasn't ABSOLUTELY necessary to. I usually use built in system calls to help error check (coded in a way they do it for me).

NOTE: since the A1 description only said to "refer to the coding style guidelines", they are not required because they aren't coding conventions (guidelines are just recommendations)

NOTE: I have incorporated the C99 standard into the compilation for my *.c files so it is a little more standardized. I also have a bunch of neccessary include statements. I also define _XOPEN_SOURCE=700 in the makefile so things work properly.

NOTE: We were given the lex.c file to use with "flex lex" that creates a "lex.yy.c" file to be used to create a "lex.yy.o" file. When compiling lex.yy.c into lex.yy.o it gives two warnings: for 'yyunput' and 'input' functions defined but not used. As this is on your side, I assume these are fine and no deductions for them. Also, I added a regex command to the lex.c that takes quote (" or ') encapsulated things as a single argument, and I don't count < or > and the following filename for redirection (like the lecture slides show).

NOTE: May print the error message for an unsupported command on the same line as the prompt rather than before (only happens sometimes: of these times, more common when involving file redirection or &)

NOTE: All things stated in this document that haven't been confirmed with a TA or the professor are assumed. Please also see my in-code comments for more/extra information.

NOTE: If this file is really messy due to formatting issues, please let me know and I can submit it as an alternate file format. Also, if something is not understood or misunderstood, I can meet to walkthrough my program.

# MAKEFILE, THEN RUN THE SHELL

_HOW TO RUN_

1) Type "make"
    --> will create all the necessary files
2) Use "./ish" when in the submission folder to run the shell program (as assigment files wanted)
    --> you should see the prompt "[user@host]$ " (# instead of $ if ./ish is run as superuser/root) (either after typing "su", or calling "sudo ./ish")
3) Now, you should be in my "vbshell" and it will work with execvp() for calls like "ls" inlcuding if it has any flags. Also, you can redirect output to files, and redirect input from files. Calling a proces w/ the last argument as '&' will execute it asynchronously in the background. I have also included 3 built-in (i.e. internal) commands that can be run used (see below). Since these are built-in, it's unnecessary to fork.

# GCD

_HOW TO RUN_

@PRE must be in vbshell program "./ish"
-it uses Euclid's algorithm to compute the greatest common denominator, and prints it
-it also works with negative numbers
-->note that if either number is a 0, then GCD is 0 because anything can be successfully divided by zero
-->note that negative don't really affect anything because if you can divide by a negative, you can also divide by the equivalent negative
-input may be in decimal form (eg. "12" or "-12") or hexadecimal form and must begin w/ "0x", or "-0x" (eg. "0xc" or "-0xc")
-will give a usage statement for bad input (give an additional error message for valid argument count, but invalid input for number)

eg. gcd <integer number 1> <integer number 2>
Input: gcd -0xc 20
Output: GCD(-0xc, 20) = 4

# LISTARGS

_HOW TO RUN_

NOTE: I have accounted for "" and '' encapuslation by modifying the given lex.c with a new regex command called QUOTED and checked for it first
NOTE: I have accounted for file redirection, where the < or > and the filename aren't counted as arguments
NOTE: I have accounted for & as the last argument to not count as an argument
@PRE must be in vbshell program "./ish"
-may have up to 100 arguments (including "args" command), and will print the argument count other than itself

eg. args [arg1, arg2, arg3, ...]
Input: args 1 2 3 4 5 "6 7 8"
Output: argc = 6, args = 1, 2, 3, 4, 5, "6 7 8"

# LCM

_HOW TO RUN_

NOTE: this was one of the first exercises I did in my Java programming class in high school, so it was the first math function that I immediately thought of when I read the description to implement our own internal command
@PRE must be in vbshell program "./ish"
-it uses Euclid's algorithm to compute the lowest common multiple, and prints it
-input must be a non-negative numbers (lowest common multiple would be -infinity always)
-input may be in decimal form (eg. "12") or hexadecimal form and must begin w/ "0x" (eg. "0xc")
-will give a usage statement for bad input (give an additional error message for valid argument count, but invalid input for number)

eg. lcm <positive integer 1> <positive integer 2>
Input: lcm 6 0xf
Output: LCM(6, 0xf) = 30

# EXPLANATIONS based on A1 DESCRIPTION

#1
I used the user ID to get the password uid passwd struct, then used the struct to get the user name. I also got the hostname in a standard way. I checked if it was root/superuser by checking if user ID was 0, which means it is, and changed the prompt to a # instead of $

#2.1
If "exit" was the only command line argument, I exited with success

#2.2
Incorporated execvp() calls to run external commands like "ls", making sure that I fork and have the parent wait for the child to finish. Terminal is blocked and can't be used until child process finishes.

#2.3
Same as 2.2: it also accomplshes flags (eg. "ls -l" works)

#2.4
Used sigaction and custom signal handling behaviours I made to wait until that specific child process finishes and then it's collected by the parent's waitpid call to terminate it properly (avoids creating zombies). Essentially, I have custom behaviour defined using SIGCHLD. Also, if it's interrupted, I have catching so it doesn't break. Please see in-code comments for more detailed explanation. Basically, if & is last argument, process is run asynchronously in the background. Terminal can still be used. Child process is properly terminated and no zombie is created.

#2.5
I used freopen to redirect to the given file instead of stdout (eg. "ls >1.txt" writes to that file instead of terminal)

#2.6
I used freopen to redirect input to be from a file instead of stdin (eg. "sort <1.txt" uses that file and sorts it and outputs to terminal)

#3
see above GCD, LISTARGS, and LCM for this