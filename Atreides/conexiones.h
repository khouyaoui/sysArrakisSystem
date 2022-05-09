#ifndef CONEXIONES_H
#define CONEXIONES_H
#include <stdio.h>		
#include <string.h>		
#include <stdlib.h>		
#include <sys/mman.h>
#include "tipos.h"
#include "sockets.h"
#include <semaphore.h>
#include <unistd.h>
#include "ficheros.h"

int inicializarListaConexiones(Conexion **conexiones,int **numConexiones);

Conexion *obtenirConexion(int id, Conexion *conexiones,int *numConexiones); //devuelve una conexion (Fremen)

Conexion *obtenirListaConexiones(Conexion *conexiones,int *numConexiones); // devuelve todas las conexiones, lista de Fremen's

int existeConexion(int id, Conexion *conexiones, int *numConexiones, char *nomNovaConexion);

void printConexiones(Conexion *conexiones,int *numConexiones);

int atenderCliente(Config_Data *c, int sfd2,Conexion *conexiones,int *numConexiones,sem_t *semaforo);

void gestionarTrama(Config_Data *c,int sfd2, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones, Conexion **conexion,File **f);

void desconectarCliente(Conexion *conexiones,int* numConexiones,int id);

#endif