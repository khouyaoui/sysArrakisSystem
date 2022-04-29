#include "ficheros.h"
#include "sockets.h"
#include "funciones.h"
#include "conexiones.h"
#include <semaphore.h>

Config_Data c;
Conexion **conexiones;
int **numConexiones;

void sigHandler(int signum)
{
    if (signum == SIGINT)
    {
        // liberarStructConfig_Data(&c);
        extraerConexiones(conexiones,&numConexiones);
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
}
int main(int argc, char *argv[])
{
    errorNumArgumentos(argc);
    display(INIT_MSG);
    readConfig(argv[1], &c);
    signal(SIGINT, sigHandler);
    signal(SIGCHLD, SIG_IGN);
    int sfd1 = crearConexion(c.ip_server, atoi(c.port_server));
    sem_t *semaforo = inicializarSemaforo();
    inicializarListaConexiones(conexiones, numConexiones);
 
    display(TERMINAL_PROMPT);
    // test copy object to memory
    Conexion *client;
    client = malloc(sizeof(Conexion));
    sem_wait(semaforo);
    strcpy(client->codigoPostal, "08092");
    strcpy(client->nom, "hahhaha");
    client->id = 1212;
    client->online = 1;
    // aqui salta SEG FAULT nose con accedir a les posicions del array q hi ha a la memoria
    insertarConexion(client, conexiones, numConexiones);
    sem_post(semaforo);
    /*
    for (;;)
    {
        int sfd2 = aceptarConexion(sfd1);
        //int id = generarID();
        switch (fork())
        {
        case -1:
            exit(-1);
        case 0:
            signal(SIGINT, SIG_IGN);
            //intentant llegir
            sem_wait(semaforo);
            //printConexiones(conexiones, numConexiones);
            sem_post(semaforo);
            
            close(sfd1);
            //tratar el client
            // atenderCliente(c, sfd2, conexiones, numConexiones, semaforo, id);
            exit(0);
        default:
            exit(0);
            close(sfd2);
        }
    }
    */
    return 0;
}
