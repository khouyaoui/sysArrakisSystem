#ifndef COMANDOS_H
#define COMANDOS_H

#include "funciones.h"
#include "sockets.h"
#include <netdb.h>

typedef struct {
    char *id;
    char *nombre;
    char *codigoPostal;
} Conexion;
Conexion *conexionData;
void ejecutarShell(char *args[], int num_args);

void ejecutarComandos(char *args[], int num_args,Config_Data *c,int *fdsocket);

void gestionarComandos(char **input,Config_Data *c,int *fdsocket);

#endif