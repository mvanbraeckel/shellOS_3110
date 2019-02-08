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

NOTE: This was made in 2 days (thank you Canada for clutch snowday that pushed back this deadline 24 hours). I lost all my work since my VM broke and won't start-up again. All recovery methods I have tried also did not work. I have done as much as I possibly can in the time I had. If I didn't lose all my work up until Wednesday my shell would have been much better. Unfortunately, I was naive and didn't backup my work anywhere since I've never had problems before. I learned my lesson now and have backed-up to git since then (as seen by my auto git add, commit, push sequence added to Makefile). Due to time constraints, I have "check[ed] and correctly handle[d] the return values of Every system call" as stated in the description for theis assignment. Since it never said anything, I have assumed that we did not need to check errno if it wasn't ABSOLUTELY necessary to.

NOTE: since the A1 description only said to "refer to the coding style guidelines", they are not required because they aren't coding conventions (guidelines are just recommendations)

NOTE: 

_HOW TO RUN_

1) Type "make"
    --> will create all the necessary files
2) Use "./ish" when in the submission folder to run the shell program (as assigment files wanted)
    --> you should see the prompt "[user@host]$ " (# instead of $ if ./ish is run as superuser/root) (either after typing "su", or calling "sudo ./ish")
3) Now, you should be in my "vbshell" and it will work with execvp() for calls like "ls" inlcuding if it has any flags. Also, you can redirect output to files, and redirect input from files. Calling a proces w/ the last argument as '&' will execute it asynchronously in the background. I have also included 3 built-in (i.e. internal) commands that can be run used (see below).

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

@PRE must be in vbshell program "./ish"
-it uses Euclid's algorithm to compute the lowest common multiple, and prints it
-input must be a non-negative numbers (lowest common multiple would be -infinity always)
-input may be in decimal form (eg. "12") or hexadecimal form and must begin w/ "0x" (eg. "0xc")
-will give a usage statement for bad input (give an additional error message for valid argument count, but invalid input for number)

eg. lcm 6 0x5
