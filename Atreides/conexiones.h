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

void insertarConexion(Conexion *conexion,Conexion *conexiones,int *numConexiones);

Conexion *obtenirConexion(int id, Conexion *conexiones,int *numConexiones); //devuelve una conexion (Fremen)

Conexion *obtenirListaConexiones(Conexion *conexiones,int *numConexiones); // devuelve todas las conexiones, lista de Fremen's

int existeConexion(int id, Conexion *conexiones, int *numConexiones, char *nomNovaConexion);

void extraerConexiones(Conexion *conexiones,int *numConexiones); // escriber la lista de conexiones en un fichero txt o binario :<


void printConexiones(Conexion *conexiones,int *numConexiones);

sem_t *inicializarSemaforo();

int atenderCliente(int sfd2,Conexion *conexiones,int *numConexiones,sem_t *semaforo);

void gestionarTrama(int sfd2, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones, Conexion **conexion);

int generarID(Conexion *conexiones, int *numConexiones,char* nom);

void desconectarCliente(Conexion *conexiones,int* numConexiones,int id);

int buscarPorCodigoPostal(char *codigoPostal,Conexion *conexiones, int *numConexiones); 

Conexion * recuperarConexion(char *login, char *cp, Conexion *conexiones, int *numConexiones);

Conexion *tratarNuevaConexion(char *trama,Conexion *conexiones, int *numConexiones);

void tratarComandaSearch(int sfd2, char *trama, char *datos, Conexion *conexiones, int *numConexiones, Conexion *conexion);

#endif