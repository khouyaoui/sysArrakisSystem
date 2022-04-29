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

#include <semaphore.h>
#include <sys/mman.h> 
// ----------------------------------  Librerias PROPIAS  ----------------------------------
#include "tipos.h"

void liberarMemoria(void *ptr);

void errorNumArgumentos(int argc);

void errorAbrirFichero(int fd);

void errorFicheroVacio(int fd);

void display(char *string);

char *readUntil(int fd, char end);

void esperarConexiones(Config_Data *c);

uint16_t controlaPuerto(char *user_port);

void errorReadSocket(int num_bytes);

sem_t *inicializarSemaforo();

#endif