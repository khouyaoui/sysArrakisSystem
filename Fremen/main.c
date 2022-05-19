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
    display(TERMINAL_PROMPT);

    for (;;)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(0, &set);
        if (fdsocket > 0)
            FD_SET(fdsocket, &set);
        int r = select(FD_SETSIZE, &set, NULL, NULL, NULL);
        // r és el número de file descriptors on puc fer operacions
        // r -1 vol dir error
        // 
        if (r < 0)
        {
            // display("fuckkkk select+sig alarm\n");
            continue;
        }
        if (FD_ISSET(0, &set))
        {
            input = readInput();
            if (strlen(input))
            {
                gestionarComandos(&input, &c, &fdsocket);
                display(TERMINAL_PROMPT);
            }
            else
            {
                free(input); // aquest free cal per quan ens posen espais i entre sense comandas
                display(TERMINAL_PROMPT);
            }
        }
        if (fdsocket > 0 && FD_ISSET(fdsocket, &set))
        {
            close(fdsocket);
            fdsocket = 0;
            display("\nS'ha perdut la connexiò amb el servidor, pots continuar executant comandes del sistema o tornat a establer la connexio\n");
            display(TERMINAL_PROMPT);
            //  si llegir socket retorna zero o -1: primer close i després *fd_socket = 0;
        }
    }
    return 0;
}
// -----------------------  gestionar las señales reprogramadas --------------------------
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
        int i, num_archivos = 0;
        struct dirent **archivos;
        num_archivos = scandir(c.directorio, &archivos, ocultarDirectorios, alphasort);
        if (num_archivos > 0)
        {
            for (i = 0; i < num_archivos; i++)
            {
                if (archivos[i]->d_type == DT_REG)
                {
                    char *extencio = strrchr(archivos[i]->d_name, '.');
                    if (extencio != NULL && strcasecmp(extencio + 1, "jpg") == 0)
                    {
                        char *aux = NULL;
                        asprintf(&aux, "%s/%s", c.directorio, archivos[i]->d_name); // like Directorio/nom.jpg
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
