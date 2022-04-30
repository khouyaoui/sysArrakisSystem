#ifndef SOCKETS_H
#define SOCKETS_H

#include "funciones.h"

int crearConexion(char *ip, uint16_t port);

void gestionaConexiones(int sockfd);

void extraeDatos(char *datos, char *trama);

void muestraDatos(char *trama);

int aceptarConexion(int sfd2);

void encapsulaTrama(char *origen, char tipo, char *datos, char *trama);

#endif