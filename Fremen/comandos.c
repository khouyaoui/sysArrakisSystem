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
    char aux[MAX_STR];
    int num_bytes;

    

    pasarMinus(args[0]);
    if (!strcmp(args[0], LOGIN))
    {
        if (num_args == LOGIN_ARG)
        {
            if (*fdsocket > 0)
            {
                display("Connexió establerta anteriorment\n");
            }
            else
            {
                *fdsocket = establecerConexion(c);
                bzero(datos, LEN_DATOS);
                strcat(datos, args[1]);
                datos[strlen(datos)] = '*';
                strcat(datos, args[2]);
                encapsulaTrama(MACHINE_NAME, 'C', datos, trama);
                num_bytes = write(*fdsocket, trama, LEN_TRAMA);
                num_bytes = read(*fdsocket, trama, LEN_TRAMA);
                if (num_bytes)
                {
                    if (trama[LEN_ORIGEN] == 'O')
                    {
                        extraeDatos(datos, trama);
                        // Pregunta: puede ser que los mallocs fueran sizeof(char) * tamaño
                        conexionData = malloc(sizeof(Conexion));
                        conexionData->nombre = malloc(sizeof(char *));
                        conexionData->codigoPostal = malloc(sizeof(char *));
                        conexionData->id = malloc(sizeof(char *));
                        strcpy(conexionData->nombre, args[1]);
                        strcpy(conexionData->codigoPostal, args[2]);
                        strcpy(conexionData->id, datos);
                        sprintf(aux, "Benvingut %s. Tens el ID %s\n", conexionData->nombre, conexionData->id);
                        display(aux);
                        display("Ara estàs connectat a Atreides\n");
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
                send(*fdsocket, trama, LEN_TRAMA, 0);

                int nbytes = read(*fdsocket, trama, LEN_TRAMA);
                if (nbytes > 0)
                {
                    extraeDatos(datos, trama);
                    if (trama[LEN_ORIGEN] == 'L')
                    {
                        // print listado de conexiones
                        char *cadenaAUX;
                        cadenaAUX = strtok(datos, "*");
                        int count = atoi(cadenaAUX);
                        if (count == 1)
                        {
                            sprintf(aux, "Hi ha una persona humana a %s\n", args[1]);
                            display(aux);
                            display("- ");
                            cadenaAUX = strtok(NULL, "*");
                            display(cadenaAUX);
                            cadenaAUX = strtok(NULL, "*");
                            display(" ");
                            display(cadenaAUX);
                            display("\n");
                        }
                        else if (count == 0)
                        {
                            sprintf(aux, "No hi ha cap persona humana a %s\n", args[1]);
                            display(aux);
                        }
                        else
                        {
                            sprintf(aux, "Hi ha %d persones humanes a %s\n", count, args[1]);
                            display(aux);
                            for (int i = 0; i < count; i++)
                            {
                                display("- ");
                                cadenaAUX = strtok(NULL, "*");
                                display(cadenaAUX);
                                cadenaAUX = strtok(NULL, "*");
                                display(" ");
                                display(cadenaAUX);
                                display("\n");
                                // cadenaAUX = strtok(NULL, "*");
                            }
                        }
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
        bzero(trama, LEN_TRAMA);
        bzero(datos, LEN_DATOS);
        if (num_args == SEND_ARG)
        {
            if (*fdsocket > 0)
            {
                if (existePhoto(args[1], c->directorio))
                {
                    char hashAUX[HASH_LEN];
                    int num_bytes = 0, fd_img = 0;
                    sprintf(aux, "%s/%s", c->directorio, args[1]);
                    fd_img = open(aux, O_RDONLY);
                    if (fd_img < 0)
                    {
                        display("\nError al obrir l'arxiu a enviar\n");
                    }
                    else
                    {
                        // primera trama a enviar !!
                        bzero(datos, LEN_DATOS);
                        strcat(datos, args[1]);
                        strcat(datos, "*");
                        char auxMida[20];
                        sprintf(auxMida, "%d", calcularMida(fd_img));
                        strcat(datos, auxMida);
                        strcat(datos, "*");
                        calcularHash(hashAUX, aux);
                        strcat(datos, hashAUX);
                        encapsulaTrama(MACHINE_NAME, 'F', datos, trama);
                        write(*fdsocket, trama, LEN_TRAMA);
                        //
                        // enviar tramas de datos
                        //
                        while ((num_bytes = read(fd_img, datos, LEN_DATOS)) > 0)
                        {
                            if (num_bytes > 0)
                            {
                                encapsulaTramaBinaria(MACHINE_NAME, 'D', datos, trama);
                                write(*fdsocket, trama, LEN_TRAMA);
                                bzero(datos, LEN_DATOS);
                            }
                        }
                        // read respuesta
                        num_bytes = read(*fdsocket, trama, LEN_TRAMA);
                        extraeDatos(datos, trama);
                        // tramas de datos img ...
                        if (num_bytes > 0)
                        {
                            if (0 == strcmp(datos, "IMAGE OK"))
                            {
                                display("Foto enviada amb èxit a Atreides.\n");
                            }
                            if (0 == strcmp(datos, "IMAGE KO"))
                            {
                                display("Error durant l'enviament d'imatge");
                            }
                        }
                        
                    }
                }
                else
                {
                    display("No hi ha foto registrada per enviar");
                }
            }
            else
            {
                display("NO ESTAS CONECTADO");
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
            if (0 == validarNomImagen(args[1]))
            {
                if (*fdsocket > 0)
                {
                    File *imagen = NULL;
                    bzero(datos, LEN_DATOS);
                    strcat(datos, args[1]);
                    encapsulaTrama(MACHINE_NAME, 'P', datos, trama);
                    // enviar solicitud de imagen
                    write(*fdsocket, trama, LEN_TRAMA);
                    // leer primera respuesta, si existe o no
                    read(*fdsocket, trama, LEN_TRAMA);
                     
                    if (trama[LEN_ORIGEN] == 'F' && 0 == strcmp("FILE NOT FOUND", trama + 16)) // imatge no trobada
                    {
                        display("\nNo hi ha foto registrada\n");
                    }
                    else
                    {
                        crearFichero(atoi(args[1]), c->directorio, &imagen, trama);
                        leerDatosIMG(*fdsocket, &imagen, trama);
                        display("\nFoto descarregada\n");
                    }
                }
                else
                {
                    display("NO ESTAS CONECTADO");
                }
            }
            else
            {
                display("ESTE NOMBRE DE IMAGEN NO ES VALIDO\n");
            }
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
            write(*fdsocket, trama, LEN_TRAMA);
             
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
    for (int i = 0; i < num_args; i++)
    {
        free(args[i]);
    }
}

void gestionarComandos(char **input, Config_Data *c, int *fdsocket)
{
    int num_comandos, i, j, l;

    num_comandos = contarPalabras(&input);
    char *comandos[num_comandos];

    i = j = l = 0;
    //comandos[j] = (char *)malloc(sizeof(char*));

    for (int i = 0; i < num_comandos; i++)
    {
        comandos[i] = NULL;
        comandos[i] = malloc(sizeof(char*));
    }
    
    while ((*input)[i] != '\0')
    {   
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
    if (input != NULL)
    {
        free(*(input)); 
    }
}