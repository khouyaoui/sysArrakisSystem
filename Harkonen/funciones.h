#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include "tipos.h"
#include <stdio.h>
#include <sys/wait.h>
#include <strings.h>
#include <string.h>
void errorNumArgumentos(int argc,char *argv);

int validarArgumento(char nomImagen[]);

void display(char *string);

void limpiarArrakisSystem(void);

#endif
