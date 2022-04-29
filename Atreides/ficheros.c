#include "ficheros.h"

void readConfig(char *nombre_f, Config_Data *c) {
    int fd;
    fd = open(nombre_f, O_RDONLY);
    errorAbrirFichero(fd);
    errorFicheroVacio(fd);
    c->ip_server = readUntil(fd, '\n');
    c->port_server = readUntil(fd, '\n');
    c->directorio = readUntil(fd, '\n');
    close(fd);
}

void liberarStructConfig_Data(Config_Data *c) {
    liberarMemoria(c->ip_server);
    liberarMemoria(c->port_server);
    liberarMemoria(c->directorio);
}