all: ATREIDES

main.o: main.c funciones.h sockets.c tipos.h conexiones.h
	gcc -c -pthread -g main.c -Wall -Wextra

funciones.o: funciones.c funciones.h tipos.h sockets.h
	gcc -c -g funciones.c -Wall -Wextra

conexiones.o: conexiones.c conexiones.h tipos.h sockets.h ficheros.h funciones.h
	gcc -c -g conexiones.c -Wall -Wextra

ficheros.o: ficheros.c ficheros.h tipos.h
	gcc -c -g ficheros.c -Wall -Wextra

sockets.o: sockets.c sockets.h tipos.h funciones.c
	gcc -c -g sockets.c -Wall -Wextra

ATREIDES: main.o funciones.o ficheros.o sockets.o conexiones.o
	gcc -pthread main.o funciones.o ficheros.o sockets.o conexiones.o -o ATREIDES.exe

clean:
	rm *.o *.exe

go:
	./ATREIDES.exe config.dat

val:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./ATREIDES.exe config.dat
