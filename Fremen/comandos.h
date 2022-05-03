#ifndef COMANDOS_H
#define COMANDOS_H

#include <netdb.h>

#include "funciones.h"
#include "sockets.h"

typedef struct {
    char *id;
    char *nombre;
    char *codigoPostal;
} Conexion;

// MIRAR SI HAY QUE MOVERLO DE SITIO
Conexion *conexionData;

void ejecutarShell(char *args[], int num_args);

void ejecutarComandos(char *args[], int num_args, Config_Data *c, int *fdsocket);

void gestionarComandos(char **input, Config_Data *c, int *fdsocket);

#endif