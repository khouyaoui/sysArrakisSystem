#include <sys/signalfd.h>

#include "comandos.h"
#include "ficheros.h"
// Zona de variables_globales
Config_Data c;

void sigHandler(int signum) {
    if (signum == SIGINT) {
        liberarStructConfig_Data(&c);
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
}
void handleSIGPIPE(){
    printf("\n343434344334\n");
}
int main(int argc, char *argv[]) {
    char *input = NULL;
    int fdsocket = 0;

    errorNumArgumentos(argc);
    readConfig(argv[1], &c);
    signal(SIGINT, sigHandler);
    signal(SIGPIPE,handleSIGPIPE);
    display(INIT_MSG);
    for (;;) {
        display(TERMINAL_PROMPT);
        input = readInput();
        // printf("strlen = %ld\n", strlen(input));
        if (strlen(input))
            gestionarComandos(&input, &c, &fdsocket);
    }
    return 0;
}