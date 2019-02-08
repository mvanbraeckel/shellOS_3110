# Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 100297
# 07/02/2019
# CIS*3110: Operating Systems A1 - simple shell
 
CC = gcc
CFLAGS = -g -Wall -std=c99

all: ish git

ish: ish.o lex.yy.o
	$(CC) $(CFLAGS) ish.o lex.yy.o -o ish -lfl  
ish.o: ish.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=700 -c ish.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=700 -c lex.yy.c
lex.yy.c: lex.c
	flex lex.c

git: *.c Makefile
	git add Makefile
	git add README.md
	git add *.c
	git commit -m "automatic backup via makefile"
	git remote rm origin
	git config credential.helper store
	git config --global credential.helper 'cache --timeout 3600'
	git remote add origin https://github.com/mvanbraeckel/shellOS_3110.git
	git push -u origin master

clean:
	rm -f *.o lex.yy.c ish
