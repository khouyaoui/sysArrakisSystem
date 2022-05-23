#define _POSIX_SOURCE
#include "funciones.h"

void errorNumArgumentos(int argc, char *argv) {
    if (argc != NUM_ARG) {
        display(ARG_NUMBER_ERR);
        exit(EXIT_SUCCESS);
    }
    if (0 != validarArgumento(argv)) {
        display(ARG_FORMAT_ERR);
        exit(EXIT_SUCCESS);
    }
}

void display(char *string) {
    write(1, string, sizeof(char) * strlen(string));
}

int validarArgumento(char arg[]) {
    for (int i = 0; i < (int)strlen(arg); i++) {
        if (!isdigit(arg[i])) {
            return -1;
        }
    }
    return 0;
}

void limpiarArrakisSystem(void) {
    srand(time(0));
    char aux[STRBUF];
    char result[STRBUF] = {0};
    int n = rand() % 120;
    char program[STRBUF];
    pid_t pid;
    if (n >= 100) {
        strcpy(program, ATREIDES);
    } else {
        strcpy(program, FREMEN);
    }

    int canals[2];
    if (pipe(canals) == -1)
        exit(-1);
    int forkPID = fork();
    switch (forkPID) {
        case 0:
            dup2(canals[1], STDOUT_FILENO);
            close(canals[1]);
            execl("/usr/bin/pidof", "pidof", program, (char *)NULL);
            exit(0);
            break;
        case -1:
            display(FORKERR);
            break;
        default:
            waitpid(forkPID, NULL, 0);
            close(canals[1]);
            read(canals[0], result, sizeof(result));
            pid = (pid_t)atoi(result);
            close(canals[1]);
            close(canals[0]);
            break;
    }
    if (pid > 0) {
        sprintf(aux, KILLINGMSG, pid);
        display(aux);
        kill(pid, SIGINT);
        pid = 0;
        bzero(result, STRBUF);
    }
    close(canals[0]);
}
