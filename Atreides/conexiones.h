#ifndef CONEXIONES_H
#define CONEXIONES_H

#include <stdio.h>		// sprintf
#include <string.h>		// strlen, strcmp
#include <stdlib.h>		// malloc, realloc, EXIT_SUCCESS
#include <sys/mman.h>
#include "tipos.h"
#include "sockets.h"
#include <semaphore.h>
#include <unistd.h>
#include "ficheros.h"


int inicializarListaConexiones(Conexion **conexiones,int **numConexiones);

void insertarConexion(Conexion *conexion,Conexion *conexiones,int *numConexiones);

Conexion *obtenirConexion(int id, Conexion *conexiones,int *numConexiones); //devuelve una conexion (Fremen)

Conexion *obtenirListaConexiones(Conexion *conexiones,int *numConexiones); // devuelve todas las conexiones, lista de Fremen's

int existeConexion(int id,Conexion *conexiones,int *numConexiones); // devuelve 1 si existe, y si esta conectado (online=1 e)

void extraerConexiones(Conexion *conexiones,int *numConexiones); // escriber la lista de conexiones en un fichero txt o binario :<

int cargarConexiones(Conexion *conexiones); // leer las conexiones guardadas anteriormente, si el fichero existe


void printConexiones(Conexion *conexiones,int *numConexiones);

sem_t *inicializarSemaforo();

int atenderCliente(Config_Data c,int sfd2,Conexion *conexiones,int *numConexiones,sem_t *semaforo);

void gestionarTrama(int sfd2, Config_Data config, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones);

int generarID(Conexion *conexiones, int *numConexiones,char* nom);

void desconectarCliente(Conexion *conexiones,int* numConexiones);

int buscarPorCodigoPostal(char *codigoPostal,Conexion *conexiones, int *numConexiones); 
#endif