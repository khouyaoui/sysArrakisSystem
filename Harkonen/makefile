all: Harkonen

main.o: main.c funciones.h tipos.h
	gcc -c -g main.c -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes \
    -Wold-style-definition

funciones.o: funciones.c funciones.h tipos.h
	gcc -c -g funciones.c -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes \
    -Wold-style-definition

Harkonen: main.o funciones.o 
	gcc main.o funciones.o -o Harkonen.exe

clean:
	rm *.o *.exe

go:
	./Harkonen.exe 10

val:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Harkonen.exe
