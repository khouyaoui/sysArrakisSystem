#include <sys/signalfd.h>
#include <sys/select.h>
#include "funciones.h"
void sigHandler(int signum)
{
    if (signum == SIGINT)
    {
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    errorNumArgumentos(argc,argv[1]);
    signal(SIGINT, sigHandler);
    display(INIT_MSG);
    for (;;)
    {
        sleep (atoi(argv[1]));
        display(SCAN);
        limpiarArrakisSystem();
        
    }
    return 0;
}
