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

void limpiarArrakisSystem()
{
    char aux[STRBUF];
    char result[STRBUF];
    FILE *pipeCMD = popen("pidof -s ATREIDES.exe", "r");
    fgets(result, STRBUF, pipeCMD);
    pid_t pid = strtoul(result, NULL, 10);
    sprintf(aux,"killing pid %d\n",pid);
    display(aux);
    kill(pid,SIGINT);
    pclose(pipeCMD);
}