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

git: *.c
	git add *
	git commit -a
	git push

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f ish
