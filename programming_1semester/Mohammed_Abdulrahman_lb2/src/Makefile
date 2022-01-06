c99 = -std=c99

all: menu.o max.o min.o diff.o sum.o
	gcc menu.o max.o min.o diff.o sum.o -o menu

menu.o: menu.c diff.h sum.h max.h min.h
	gcc -c $(c99) menu.c

sum.o: sum.c sum.h min.h
	gcc -c $(c99) sum.c

diff.o: diff.c diff.h max.h min.h
	gcc -c $(c99) diff.c

min.o: min.c min.h
	gcc -c $(c99) min.c

max.o: max.c max.h
	gcc -c $(c99) max.c

clean:
	rm -rf *.o menu
