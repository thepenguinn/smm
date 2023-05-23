# A simple makefile

smm: smm.o matrix.o drw.o
	gcc -o $@ -g -lncurses smm.o matrix.o drw.o

smm.o: smm.c matrix.h drw.h
	gcc -c -g -lncurses smm.c

matrix.o: matrix.c matrix.h drw.h
	gcc -c -g matrix.c

drw.o: drw.c matrix.h drw.h
	gcc -c -g -lncurses drw.c
