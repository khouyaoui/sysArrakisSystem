#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "tipos.h"
void errorNumArgumentos(int argc, char *argv);

int validarArgumento(char nomImagen[]);

void display(char *string);

void limpiarArrakisSystem(void);

#endif
