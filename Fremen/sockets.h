#ifndef SOCKETS_H
#define SOCKETS_H

#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include "tipos.h"
#include "funciones.h"

uint16_t controlaPuerto(int aux);

int establecerConexion(Config_Data *c);

void extraeDatos(char *datos, char *trama);

#endif