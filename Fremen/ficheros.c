#include "ficheros.h"

void readConfig(char *nombre_f, Config_Data *c) {
    int fd;
    char *aux;

    // Abrimos fichero en modo lectura
    fd = open(nombre_f, O_RDONLY);

    // Controlamos error de obertura
    errorAbrirFichero(fd);

    // Precheck para controlar si esta vacio
    errorFicheroVacio(fd);

    // Extraemos los campos del fichero
    // segundos
    aux = readUntil(fd, '\n');
    c->segundos = atoi(aux);
    // ip_server
    c->ip_server = readUntil(fd, '\n');
    // port_server
    c->port_server = readUntil(fd, '\n');
    // ip_wendy
    c->directorio = readUntil(fd, '\n');

    // Liberamos memoria y cerramos el file descriptor asociado al fichero
    liberarMemoria(aux);
    close(fd);
}

void liberarStructConfig_Data(Config_Data *c) {
    liberarMemoria(c->ip_server);
    liberarMemoria(c->port_server);
    liberarMemoria(c->directorio);
}