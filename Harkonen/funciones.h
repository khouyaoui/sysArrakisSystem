#ifndef FUNCIONES_H
#define FUNCIONES_H

// ----------------------------------  Librerias GENERALES del sistema  ----------------------------------
// Output
#include <stdio.h>  // sprintf
//#include <sys/types.h>  //  pthread_t, size_t

// Ficheros
#include <fcntl.h>   // Modos de opertura : O_RDONLY
#include <unistd.h>  // read, write, open, close. execlp
#include <string.h>
#include <stdlib.h> 

// Signals
#include <signal.h>    // Signals
#include <sys/wait.h>  // wait
#include <ctype.h>
#include "tipos.h"

void errorNumArgumentos(int argc,char *argv);

int validarArgumento(char nomImagen[]);

void display(char *string);

void limpiarArrakisSystem();


#endif