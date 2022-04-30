#ifndef FICHEROS_H
#define FICHEROS_H

#include "funciones.h"

void readConfig(char *nombre_f, Config_Data *c);

void liberarStructConfig_Data(Config_Data *c);

void extraerConexiones(Conexion *conexiones,int *numConexiones); // escriber la lista de conexiones en un fichero txt o binario :<

int existeConexion(int id,Conexion *conexiones,int *numConexiones); // devuelve 1 si existe, y si esta conectado (online=1 e)

int cargarConexiones(Conexion *conexiones); // leer las conexiones guardadas anteriormente, si el fichero existe

int existenConexiones();

#endif