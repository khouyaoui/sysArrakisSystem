#define _POSIX_SOURCE
#include "funciones.h"

void errorNumArgumentos(int argc, char *argv)
{
    if (argc != NUM_ARG)
    {
        display(ARG_NUMBER_ERR);
        exit(EXIT_SUCCESS);
    }
    if (0 != validarArgumento(argv))
    {
        display(ARG_FORMAT_ERR);
        exit(EXIT_SUCCESS);
    }
}

void display(char *string)
{
    write(1, string, sizeof(char) * strlen(string));
}

int validarArgumento(char arg[])
{
    for (int i = 0; i < (int)strlen(arg); i++)
    {
        if (!isdigit(arg[i]))
        {
            return -1;
        }
    }
    return 0;
}

void limpiarArrakisSystem(void)
{
    srand(time(0));
    char aux[STRBUF];
    char result[STRBUF];
    int n = rand() % 120;
    char program[STRBUF];
    pid_t pid;
    if (n >= 100)
    {
        strcpy(program, "ATREIDES.exe");
    }
    else
    {
        strcpy(program, "Fremen.exe");
    }

    int child_status;
    int canals[2];
    if (pipe(canals) == -1)
        exit(-1);
    int ret = fork();
    switch (ret)
    {
    case 0:
        dup2(canals[1], STDOUT_FILENO);
        close(canals[1]);
        execl("/usr/bin/pidof", "pidof", program, (char *)NULL);
        exit(0);
        break;
    case -1:
        write(0, "Error fork per netejar ArrakisSystem", strlen("Error fork per netejar ArrakisSystem"));
        break;
    default:
        waitpid(ret, &child_status, 0);
        close(canals[1]);
        read(canals[0], result, sizeof(result));
        pid = strtoul(result, NULL, 10);
        close(canals[1]);
        break;
    }
    if (pid > 0)
    {
        sprintf(aux, "killing pid %d\n", pid);
        display(aux);
        kill(pid, SIGINT);
        pid = 0;
        bzero(result, STRBUF);
    }
    close(canals[0]);
}
