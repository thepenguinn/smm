# A simple makefile

smm: smm.o matrix.o drw.o
	gcc -o $@ -lncurses smm.o matrix.o drw.o

smm.o: smm.c matrix.h drw.h
	gcc -c -lncurses smm.c

matrix.o: matrix.c matrix.h drw.h
	gcc -c matrix.c

drw.o: drw.c matrix.h drw.h
	gcc -c -lncurses drw.c
