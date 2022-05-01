#include "comandos.h"

void ejecutarShell(char *args[], int num_args)
{
    // display(num_args);
    // display(args[0]);
    args[num_args] = NULL;
    pid_t pid = fork();
    switch (pid)
    {
    case 0:
        if (num_args)
        {
            execvp(args[0], args);
            _exit(0);
        }
        break;
    case -1:
        display(COMMNAND_ERR);
        exit(0); // cuando ejecutas basura en shell ej:eufhuerf
        break;
    default:
        waitpid(pid, NULL, 0);
    }
}

void ejecutarComandos(char *args[], int num_args, Config_Data *c, int *fdsocket)
{
    char trama[LEN_TRAMA];
    char datos[LEN_DATOS];
    char aux[LEN_TRAMA];
    int num_bytes;

    pasarMinus(args[0]);
    if (!strcmp(args[0], LOGIN))
    {
        if (num_args == LOGIN_ARG)
        {
            if (*fdsocket > 0)
            {
                display("CONEXIÓN ESTABLECIDA ANTERIORMENTE\n");
            }
            else
            {
                *fdsocket = establecerConexion(c);
                bzero(datos, LEN_DATOS);
                strcat(datos, args[1]);
                datos[strlen(datos)] = '*';
                strcat(datos, args[2]);
                encapsulaTrama(MACHINE_NAME, 'C', datos, trama);
                num_bytes = send(*fdsocket, trama, LEN_TRAMA, 0);
                printf("\n\nenviados---> n = %d\n\n", num_bytes);
                num_bytes = read(*fdsocket, trama, LEN_TRAMA);
                if (num_bytes)
                {
                    if (trama[LEN_ORIGEN] == 'O')
                    {
                        extraeDatos(datos, trama);
                        sprintf(aux, "Recibiendo O: %s\n", datos);
                        display(aux);
                        printf("trama: %s", trama);
                        conexionData = malloc(sizeof(Conexion));
                        conexionData->nombre = malloc(sizeof(char *));
                        conexionData->codigoPostal = malloc(sizeof(char *));
                        conexionData->id = malloc(sizeof(char *));
                        strcpy(conexionData->nombre, args[1]);
                        strcpy(conexionData->codigoPostal, args[2]);
                        strcpy(conexionData->id, datos);
                    }
                    if (trama[LEN_ORIGEN] == 'E')
                    {
                        extraeDatos(datos, trama);
                        sprintf(aux, "Recibiendo E: %s\n", datos);
                        display(aux);
                    }
                }
            }
        }
        else if (num_args < LOGIN_ARG)
        {
            display("FALTEN ARGUMENTOS PARA LA COMANDA LOGIN\n");
        }
        else
        {
            display("SOBREN ARGUMENTOS PARA LA COMANDA LOGIN\n");
        }
    }
    else if (!strcmp(args[0], SEARCH))
    {
        if (num_args == SEARCH_ARG)
        {
            if (*fdsocket > 0)
            {
                bzero(datos, LEN_DATOS);
                strcat(datos, conexionData->nombre);
                datos[strlen(datos)] = '*';
                strcat(datos, conexionData->id);
                datos[strlen(datos)] = '*';
                strcat(datos, args[1]);
                encapsulaTrama(MACHINE_NAME, 'S', datos, trama);
                display("COMANDA OK\n");
                send(*fdsocket, trama, LEN_TRAMA, 0);

                int nbytes = read(*fdsocket, trama, LEN_TRAMA);
                if (nbytes > 0)
                {
                    if (trama[LEN_ORIGEN] == 'L')
                    {
                        // print listado de conexiones
                        extraeDatos(datos, trama);
                        sprintf(aux, "Recibiendo: %s\n", datos);
                        display(aux);
                    }
                    if (trama[LEN_ORIGEN] == 'K')
                    {
                        // print listado de conexiones
                        extraeDatos(datos, trama);
                        sprintf(aux, "La trama enviada contenia errors\n");
                        display(aux);
                    }
                }
            }
            else
            {
                display("NO ESTAS CONECTADO");
            }
        }
        else if (num_args < SEARCH_ARG)
        {
            display("FALTEN ARGUMENTOS PARA LA COMANDA SEARCH\n");
        }
        else
        {
            display("SOBREN ARGUMENTOS PARA LA COMANDA SEARCH\n");
        }
    }
    else if (!strcmp(args[0], SEND))
    {
        if (num_args == SEND_ARG)
        {
            bzero(datos, LEN_DATOS);
            strcat(datos, conexionData->nombre);
            datos[strlen(datos)] = '*';
            strcat(datos, conexionData->id);
            datos[strlen(datos)] = '*';
            strcat(datos, args[1]);
            encapsulaTrama(MACHINE_NAME, 'S', datos, trama);       
            send(*fdsocket, trama, LEN_TRAMA, 0);
            int nbytes = read(*fdsocket, trama, LEN_TRAMA);
            if (nbytes > 0)
            {
                if (trama[LEN_ORIGEN] == 'L')
                {
                    // print listado de conexiones
                    extraeDatos(datos, trama);
                    sprintf(aux, "Recibiendo: %s\n", datos);
                    display(aux);
                }
                if (trama[LEN_ORIGEN] == 'K')
                {
                    // print listado de conexiones
                    extraeDatos(datos, trama);
                    sprintf(aux, "La trama enviada contenia errors\n");
                    display(aux);
                }
            }
        }
        else if (num_args < SEND_ARG)
        {
            display("FALTEN ARGUMENTOS PARA LA COMANDA SEND\n");
        }
        else
        {
            display("SOBREN ARGUMENTOS PARA LA COMANDA SEND\n");
        }
    }
    else if (!strcmp(args[0], PHOTO))
    {
        if (num_args == PHOTO_ARG)
        {
            display("COMANDA OK\n");
            // DO SOMTHING
        }
        else if (num_args < PHOTO_ARG)
        {
            display("FALTEN ARGUMENTOS PARA LA COMANDA PHOTO\n");
        }
        else
        {
            display("SOBREN ARGUMENTOS PARA LA COMANDA PHOTO\n");
        }
    }
    else if (!strcmp(args[0], LOGOUT))
    {
        if (num_args == LOGOUT_ARG)
        {
            bzero(datos, LEN_DATOS);
            strcat(datos, conexionData->nombre);
            datos[strlen(datos)] = '*';
            strcat(datos, conexionData->id);
            datos[strlen(datos)] = '*';
            strcat(datos, args[1]);
            encapsulaTrama(MACHINE_NAME, 'Q', datos, trama);
            send(*fdsocket, trama, LEN_TRAMA, 0);
            raise(SIGINT);
        }
        else if (num_args > LOGOUT_ARG)
        {
            display("SOBRAN ARGUMENTOS PARA LA COMANDA LOGOUT\n");
        }
    }
    else
    {
        ejecutarShell(args, num_args);
    }
}

void gestionarComandos(char **input, Config_Data *c, int *fdsocket)
{
    int num_comandos, i, j, l;

    num_comandos = contarPalabras(&input);
    char *comandos[num_comandos];

    i = j = l = 0;
    while ((*input)[i] != '\0')
    {
        comandos[j] = (char *)malloc(sizeof(char) * MAX_STR);

        l = 0;
        while ((*input)[i] != '\0' && (*input)[i] != ' ')
        {
            comandos[j][l] = (*input)[i];
            i = i + 1;
            l++;
        }
        comandos[j][l] = '\0';
        j++;

        while ((*input)[i] == ' ')
        {
            i++;
        }
    }

    ejecutarComandos(comandos, num_comandos, c, fdsocket);
    for (l = 0; l < num_comandos; l++)
    {
        if (!comandos[l] != '\000') // cas de que l'usuari ingrese ls with space and without params
        {
            liberarMemoria(comandos[l]);
        }
    }
    if (input != NULL)
    {
        liberarMemoria(*input);
    }
}
