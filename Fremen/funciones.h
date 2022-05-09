#ifndef FUNCIONES_H
#define FUNCIONES_H

// ----------------------------------  Librerias GENERALES del sistema  ----------------------------------
// Output
#include <stdio.h>  // sprintf
//#include <sys/types.h>  //  pthread_t, size_t

// Ficheros
#include <fcntl.h>   // Modos de opertura : O_RDONLY
#include <unistd.h>  // read, write, open, close. execlp

// Strings
#include <string.h>  // strlen, strcmp
//#include <strings.h>    // bzero

// Memoria
#include <stdlib.h>  // malloc, realloc, EXIT_SUCCESS
//#include <unistd.h>     // pause, alarm, getpid

// Signals
#include <signal.h>    // Signals
#include <sys/wait.h>  // wait

// Directorios
#include <dirent.h>

// Sockets
#include <arpa/inet.h>   // inet_addr(ip)
#include <stdint.h>      // type uint16_t
#include <sys/socket.h>  // int socket(), connect(), struct sockaddr_in
//#include <netinet/in.h>
#include <sys/stat.h>
//isgidit
#include <ctype.h>
// ----------------------------------  Librerias PROPIAS  ----------------------------------
#include "tipos.h"

void liberarMemoria(void *ptr);

void errorNumArgumentos(int argc);

void errorAbrirFichero(int fd);

void errorFicheroVacio(int fd);

void display(char *string);

char *readUntil(int fd, char end);

char *readInput();

char *extraerPalabra(char ***string, int *i);

int contarPalabras(char ***string);

void pasarMinus(char *frase);

void encapsulaTrama(char *origen, char tipo, char *datos, char *trama);

void calcularHash(char *hash, char *fileName);

void encapsulaTramaBinaria(char *origen, char tipo, char *datos, char *trama);

int calcularMida(int fd);

void crearFichero(int ID, char *directorio, File **file, char *trama);

void leerDatosIMG(int sfd, File **file, char *trama);

int validarNomImagen(char nomImagen[]);

int ocultarDirectorios(const struct dirent *arg);

int existePhoto(char *photo_id,char *directorio);

#endif