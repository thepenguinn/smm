# A simple makefile

smm: smm.o matrix.o drw.o log.o
	gcc -o $@ -g -lncurses smm.o matrix.o drw.o log.c

smm.o: smm.c smm.h matrix.h drw.h log.h
	gcc -c -g -lncurses smm.c

matrix.o: matrix.c matrix.h drw.h log.h
	gcc -c -g matrix.c

drw.o: drw.c matrix.h drw.h log.h
	gcc -c -g -lncurses drw.c

log.o: log.c log.h
	gcc -c -g log.c
