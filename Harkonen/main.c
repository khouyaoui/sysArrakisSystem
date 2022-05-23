#include <sys/signalfd.h>

#include "funciones.h"
void sigHandler(int signum);

int main(int argc, char *argv[]) {
    errorNumArgumentos(argc, argv[1]);
    signal(SIGINT, sigHandler);
    display(INIT_MSG);
    for (;;) {
        display(SCAN);
        for (size_t i = atoi(argv[1]); i > 0; i--) {
            display(".");
            sleep(1);
        }
        display("\n");
        limpiarArrakisSystem();
    }
    return 0;
}

void sigHandler(int signum) {
    if (signum == SIGINT) {
        display(FINAL_MSG);
        exit(EXIT_SUCCESS);
    }
}