#define _DEFAULT_SOURCE
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

// escriber la lista de conexiones en un fichero txt o binario :<
void extraerConexiones(Conexion *conexiones, int *numConexiones) {
    int fd = open("conexiones", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        close(fd);
    } else {
        for (int n = 0; n < *numConexiones; n++) {
            conexiones[n].online = 0;
            write(fd, conexiones + n, sizeof(Conexion));
        }
        // TODO revisar
        munmap(conexiones, sizeof(conexiones) * MAX_CON);
        munmap(numConexiones, sizeof(int));

        close(fd);
    }
}

// leer las conexiones guardadas anteriormente, si el fichero existe
int cargarConexiones(Conexion *conexiones) {
    if (existenConexiones() == 1) {
        int fd = open("conexiones", O_RDONLY);
        if (fd < 0) {
            display("\nError al abrir l'arxiu conexiones\n");
            // exit(1);
        }
        int n = 0;
        while (0 < read(fd, conexiones + n, sizeof(Conexion))) {
            n++;
        }
        close(fd);
        return n;
    } else {
        return 0;
    }
}
// comprova si el fitxer existeix per llegir-ho
int existenConexiones(void) {
    if (access("conexiones", F_OK) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int existePhoto(char *photo_id) {
    // canviar per open el file - no cal escanajer e¡el directori,
    // si fem open i no hem pogut es que no existeix!
    int i, num_archivos;
    struct dirent **archivos;
    strcat(photo_id, ".jpg");
    num_archivos = scandir("./Directorio", &archivos, ocultarDirectorios, alphasort);

    if (num_archivos <= 0) {
        display(DIR_EMPTY_ERR);
    } else {
        for (i = 0; i < num_archivos; i++) {
            // Identificamos el tipo de archivo segun la extension
            archivos[i]->d_type = archivos[i]->d_name[strlen(archivos[i]->d_name) - 1];
            if (archivos[i]->d_type == 'g') {
                if (!strcmp(archivos[i]->d_name, photo_id)) {
                    // calcu
                    return 1;
                }
            }
        }
        // FREE de cada elemento del Struct dirent
        for (i = 0; i < num_archivos; i++) {
            liberarMemoria(archivos[i]);
        }
        liberarMemoria(archivos);
    }
    return 0;
}

void enviarImagen(int sfd2, char *datos, File **imagen, char *trama, char *aux) {
    if ((*imagen)->fd > 0) {
        bzero(datos, LEN_DATOS);
        strcat(datos, (*imagen)->nom);
        strcat(datos, "*");
        sprintf(aux, "%d", (*imagen)->mida);
        strcat(datos, aux);
        strcat(datos, "*");
        strcat(datos, (*imagen)->hash);
        bzero(trama, LEN_TRAMA);
        encapsulaTrama(MACHINE_NAME, 'F', datos, trama);
        // enviamos la cabezera de la imagen: mida md5 nom
        write(sfd2, trama, LEN_TRAMA);

        while ((*imagen)->mida != 0) {
            if ((*imagen)->mida >= LEN_DATOS) {
                read((*imagen)->fd, datos, LEN_DATOS);
                encapsulaTramaBinaria(MACHINE_NAME, 'D', datos, trama);
                write(sfd2, trama, LEN_TRAMA);
                (*imagen)->mida -= LEN_DATOS;
            } else {
                // ultim troç d'imatge enviat !
                read((*imagen)->fd, datos, (*imagen)->mida);
                encapsulaTramaBinaria(MACHINE_NAME, 'D', datos, trama);
                write(sfd2, trama, LEN_TRAMA);
                (*imagen)->mida = 0;
            }
            if ((*imagen)->mida == 0) {
                close((*imagen)->fd);
            }
        }
    }
}

int ocultarDirectorios(const struct dirent *arg) {
    // Para que no nos devuelva el directorio actual (.) y el anterior (..)
    if (strcmp(arg->d_name, ".") == 0 || strcmp(arg->d_name, "..") == 0) {
        return 0;
    }
    return 1;
}
