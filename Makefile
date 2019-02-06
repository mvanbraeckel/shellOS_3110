CFLAGS = -g -Wall

all: ish

ish: ish.o lex.yy.o
	gcc -o ish ish.o lex.yy.o -lfl  
ish.o: ish.c
	gcc $(CFLAGS) -c ish.c

lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c lex.yy.c
lex.yy.c: lex.c
	flex lex.c

git: *.c Makefile
	git add Makefile
	git add CIS3110_A1_2019.pdf
	git add *.c
	git commit -a
	git push
	mvanbraeckel

clean:
	rm -f *.o lex.yy.c ish
