#ifndef SOCKETS_H
#define SOCKETS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

#include "funciones.h"
#include "tipos.h"

int establecerConexion(Config_Data *c);

void extraeDatos(char *datos, char *trama);

void extraeDatosBinarios(char *datos, char *trama);

void errorSocket(int sockfd);

#endif