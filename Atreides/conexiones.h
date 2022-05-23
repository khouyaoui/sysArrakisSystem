#ifndef CONEXIONES_H
#define CONEXIONES_H
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "ficheros.h"
#include "sockets.h"
#include "tipos.h"

int inicializarListaConexiones(Conexion **conexiones, int **numConexiones);

int atenderCliente(Config_Data *c, int sfd2, Conexion *conexiones, int *numConexiones, sem_t *semaforo);

void gestionarTrama(Config_Data *c, int sfd2, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones, Conexion **conexion, File **f);

void desconectarCliente(Conexion *conexiones, int *numConexiones, int id);

#endif