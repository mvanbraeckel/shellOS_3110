CFLAGS = -g -Wall

all: ish git

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
	git add *.c
	git commit -m "automatic makefile backup"
	git remote rm origin
	git config credential.helper store
	git config --global credential.helper 'cache --timeout 3600'
	git remote add origin https://github.com/mvanbraeckel/shellOS_3110.git
	git push -u origin master

clean:
	rm -f *.o lex.yy.c ish
