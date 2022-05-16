#ifndef SOCKETS_H
#define SOCKETS_H

#include "funciones.h"
#include <strings.h>

int crearConexion(uint16_t port);

void extraeDatos(char *datos, char *trama);

int aceptarConexion(int sfd2);

void encapsulaTrama(char *origen, char tipo, char *datos, char *trama);

void extraeDatosBinarios(char *datos, char *trama);

#endif
