#include "ficheros.h"

void readConfig(char *nombre_f, Config_Data *c)
{
    int fd;
    fd = open(nombre_f, O_RDONLY);
    errorAbrirFichero(fd);
    errorFicheroVacio(fd);
    c->ip_server = readUntil(fd, '\n');
    c->port_server = readUntil(fd, '\n');
    c->directorio = readUntil(fd, '\n');
    close(fd);
}

void liberarStructConfig_Data(Config_Data *c)
{
    liberarMemoria(c->ip_server);
    liberarMemoria(c->port_server);
    liberarMemoria(c->directorio);
}

// escriber la lista de conexiones en un fichero txt o binario :<
void extraerConexiones(Conexion *conexiones, int *numConexiones)
{
    int fd = open("conexiones", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        close(fd);
    }
    else
    {
        for (int n = 0; n < *numConexiones; n++)
        {
            conexiones[n].online = 0;
            write(fd, conexiones + n, sizeof(Conexion));
        }
        //TODO revisar
        munmap(conexiones,sizeof(conexiones)*MAX_CON);
        munmap(numConexiones,sizeof(int));

        close(fd);
    }
}

// leer las conexiones guardadas anteriormente, si el fichero existe
int cargarConexiones(Conexion *conexiones)
{
    if (existenConexiones() == 1)
    {
        int fd = open("conexiones", O_RDONLY);
        if (fd < 0)
        {
            display("\nError al abrir l'arxiu conexiones\n");
            // exit(1);
        }
        int n = 0;
        while (0 < read(fd, conexiones + n, sizeof(Conexion)))
        {
            n++;
        }
        close(fd);
        return n;
    }
    else
    {
        return 0;
    }
}
// comprova si el fitxer existeix per llegir-ho
int existenConexiones()
{
    if (access("conexiones", F_OK) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}