# A simple makefile

smm: mat.o
	gcc -o $@ -lncurses mat.o

mat.o: mat.c
	gcc -c -lncurses mat.c
