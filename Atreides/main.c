#include "ficheros.h"
#include "sockets.h"
#include "funciones.h"
#include "conexiones.h"
#include <semaphore.h>

Config_Data c;
Conexion *conexiones;
int *numConexiones;
sem_t *semaforo;
void sigHandler(int signum);

int main(int argc, char *argv[])
{
     
    errorNumArgumentos(argc);
    display(INIT_MSG);
    readConfig(argv[1], &c);
    signal(SIGINT, sigHandler);
    signal(SIGCHLD, SIG_IGN);
    int sfd1 = crearConexion(atoi(c.port_server));
    semaforo = inicializarSemaforo();
    inicializarListaConexiones(&conexiones, &numConexiones);
    *numConexiones = cargarConexiones(conexiones);
    display(TERMINAL_PROMPT);  
    for (;;)
    {
        int sfd2 = aceptarConexion(sfd1);
        switch (fork())
        {
        case -1:
            exit(-1);
        case 0:
            signal(SIGINT, SIG_DFL);
            signal(SIGCHLD, SIG_IGN);
            close(sfd1);
            atenderCliente(&c,sfd2, conexiones, numConexiones, semaforo);
            exit(0);
        default:
            close(sfd2);
        }
    }
    return 0;
}

void sigHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
    {
        liberarStructConfig_Data(&c);
        if (*numConexiones > 0)
        {
            extraerConexiones(conexiones,numConexiones);
        }
        sem_destroy(semaforo);
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
}
