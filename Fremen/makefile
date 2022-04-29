all: Fremen
# La opcion -g al compilar el codigo fuente (*.c) sirve para poder conocer la linea al debugar con valgrind

main.o: main.c funciones.h comandos.h tipos.h
	gcc -c -g main.c -Wall -Wextra

funciones.o: funciones.c funciones.h tipos.h
	gcc -c -g funciones.c -Wall -Wextra

ficheros.o: ficheros.c ficheros.h tipos.h
	gcc -c -g ficheros.c -Wall -Wextra

comandos.o: comandos.c comandos.h funciones.h tipos.h sockets.h
	gcc -c -g comandos.c -Wall -Wextra

sockets.o: sockets.c sockets.h funciones.h tipos.h
	gcc -c -g sockets.c -Wall -Wextra

Fremen: main.o funciones.o ficheros.o comandos.o sockets.o
	gcc main.o funciones.o ficheros.o comandos.o sockets.o -o Fremen.exe

clean:
	rm *.o *.exe

go:
	./Fremen.exe config.dat

val:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Fremen.exe config.dat
