#define _GNU_SOURCE
#include <sys/signalfd.h>
#include <sys/select.h>
#include "comandos.h"
#include "ficheros.h"
// Zona de variables_globales
Config_Data c;

void sigHandler(int signum);

int main(int argc, char *argv[])
{
    char *input = NULL;
    int fdsocket = 0;

    errorNumArgumentos(argc);
    readConfig(argv[1], &c);
    signal(SIGINT, sigHandler);
    signal(SIGALRM, sigHandler); // Clean .JPG
    alarm(c.segundos);
    display(INIT_MSG);
    for (;;)
    {
        display(TERMINAL_PROMPT);
        fd_set set;
        FD_ZERO(&set);
        FD_SET(0, &set);
        if (fdsocket > 0)
            FD_SET(fdsocket, &set);
        int r = select(FD_SETSIZE, &set, NULL, NULL, NULL);
        // r és el número de file descriptors on puc fer operacions
        // r -1 vol dir error
        // TODO
        if (r == 1)
        { // nomes per fer la documentació
        }
        if (FD_ISSET(0, &set))
        {
            input = readInput();
            if (strlen(input)){
                gestionarComandos(&input, &c, &fdsocket);
            }else {
                free(input);
            }
        }
        if (fdsocket > 0 && FD_ISSET(fdsocket, &set))
        {
            close(fdsocket);
            fdsocket = 0;
            display("\nS'ha perdut la connexiò amb el servidor\n");
            //  si llegir socket retorna zero o -1: primer close i després *fd_socket = 0;
        }
    }
    return 0;
}

void sigHandler(int signum)
{
    if (signum == SIGINT)
    {
        liberarStructConfig_Data(&c);
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
    if (signum == SIGALRM)
    {

        int i, num_archivos;
        struct dirent **archivos;
        num_archivos = scandir(c.directorio, &archivos, ocultarDirectorios, alphasort);

        if (num_archivos <= 0)
        {
            // do nothing
        }
        else
        {
            for (i = 0; i < num_archivos; i++)
            {
                if (archivos[i]->d_type == DT_REG)
                {
                    char *extencio = strrchr(archivos[i]->d_name, '.');
                    if (extencio != NULL && strcasecmp(extencio + 1, "jpg") == 0){
                        char * aux = NULL;
                        asprintf (&aux, "%s/%s", c.directorio, archivos[i]->d_name);
                        unlink(aux);
                        free(aux);
                    }
                }
                
            }
        }
        // FREE de cada elemento del Struct dirent
        for (i = 0; i < num_archivos; i++)
        {
            liberarMemoria(archivos[i]);
        }
        liberarMemoria(archivos);

        alarm(c.segundos); // tornem a reconfigurar per la propera neteja
    }
}
