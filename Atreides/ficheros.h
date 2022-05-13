#ifndef FICHEROS_H
#define FICHEROS_H

#include "funciones.h"
#include <strings.h>
#include <dirent.h>

void readConfig(char *nombre_f, Config_Data *c);

void liberarStructConfig_Data(Config_Data *c);

void extraerConexiones(Conexion *conexiones,int *numConexiones); // escriber la lista de conexiones en un fichero txt o binario :<

int existenConexiones(void);

int cargarConexiones(Conexion *conexiones); // leer las conexiones guardadas anteriormente, si el fichero existe

int existePhoto(char *photo_id);

int ocultarDirectorios(const struct dirent *arg);

#endif
