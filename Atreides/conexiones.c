#include "conexiones.h"
int inicializarListaConexiones(Conexion **conexiones, int **numConexiones)
{
    // indicador de nom que hi han al llistat
    *numConexiones = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    // llistat on guardarem
    *conexiones = mmap(NULL, sizeof(Conexion) * 50, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (*conexiones == MAP_FAILED)
    {
        perror("conexiones error regio memoria mmap");
        return -1;
    }
    if (*numConexiones == MAP_FAILED)
    {
        perror("numConexiones error regio memoria mmap");
        return -1;
    }
    bzero(*conexiones, sizeof(Conexion) * 50);

    **numConexiones = 0;

    return 1;
}

void insertarConexion(Conexion *conexion, Conexion *conexiones, int *numConexiones)
{
    strcpy(conexiones[*numConexiones].codigoPostal, conexion->codigoPostal);
    conexiones[*numConexiones].id = conexion->id;
    strcpy(conexiones[*numConexiones].nom, conexion->nom);
    conexiones[*numConexiones].online = conexion->online;
    (*numConexiones)++;
}
Conexion *obtenirConexion(int id, Conexion *conexiones, int *numConexiones)
{
    Conexion *conexion;
    conexion = malloc(sizeof conexion);
    for (int i = 0; i <= *numConexiones; i++)
    {
        if (conexiones[i].id == id)
        {
            strcpy(conexion->nom, conexiones[i].nom);
            strcpy(conexion->codigoPostal, conexiones[i].codigoPostal);
            conexion->id = conexiones[i].id;
            conexion->online = conexiones[i].online;
        }
    }
    return conexion;
}
// devuelve todas las conexiones, lista de Fremen's
Conexion *obtenirListaConexiones(Conexion *conexiones, int *numConexiones)
{
    if (*numConexiones > 1)
    {
        return conexiones;
    }
    return NULL;
}
// devuelve 1 si existe, y si esta conectado (online=1 e)
int existeConexion(int id, Conexion *conexiones, int *numConexiones)
{
    int existe = 0;
    for (int i = 0; i < *numConexiones; i++)
    {
        if (conexiones[i].id == id)
        {
            existe = 1;
        }
    }
    return existe;
}

void printConexiones(Conexion *conexiones, int *numConexiones)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        printf("client %d\n", i + 1);
        printf("nom ->%s\n", conexiones[i].nom);
        printf("codi postal->%s\n", conexiones[i].codigoPostal);
        printf("id ->%d\n", conexiones[i].id);
        printf("online ->%d\n", conexiones[i].online);
    }
}

int atenderCliente(Config_Data config, int sfd2, Conexion *conexiones, int *numConexiones, sem_t *semaforo)
{
    char trama[LEN_TRAMA] = {0};
    int contador = 0;
    while (1)
    {
        bzero(trama, LEN_TRAMA);
        int nbytes = read(sfd2, trama + contador, sizeof(trama) - contador);
        if (nbytes < 0)
            return -1; // no hauria de passar...
        else if (nbytes == 0)
        {
            printf("client colsed\n");
            desconectarCliente(conexiones, numConexiones);
            _exit(0);
        }
        else
        {
            contador += nbytes;
        }
        if (contador == sizeof(trama))
        {
            // tenim una trama sencera
            gestionarTrama(sfd2, config, trama, conexiones, semaforo, numConexiones);
            contador = 0;
        }
        display(TERMINAL_PROMPT);
    }
}
void desconectarCliente(Conexion *conexiones, int *numConexiones)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        if (conexiones[i].id == id)
        {
            conexiones[i].online = 0;
        }
    }
    // printConexiones(conexiones, numConexiones);
}

void gestionarTrama(int sfd2, Config_Data config, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones)
{
    Conexion *conexion;
    char datos[LEN_DATOS];
    int count;

    // printf("write trama\n");
    // write(0,trama,256);
    // printf("\n");
    switch (trama[LEN_ORIGEN])
    {
    case 'C':
        conexion = malloc(sizeof(Conexion));
        extraeDatos(datos, trama);
        // save data in vars
        char *cadena = strtok(datos, "*");
        if (cadena != NULL)
        {
            strcpy(conexion->nom, cadena);
            while (cadena != NULL)
            {
                strcpy(conexion->codigoPostal, cadena);
                cadena = strtok(NULL, "*");
            }
        }
        free(cadena);
        id = generarID(conexiones, numConexiones, conexion->nom);
        sprintf(datos, "%d", id);
        fprintf(stdout, "\nRebut login %s %s\n", conexion->nom, conexion->codigoPostal);
        fprintf(stdout, "Assignat a ID %d.\nEnviada resposta\n\n", id);
        encapsulaTrama(MACHINE_NAME, 'O', datos, trama);
        write(sfd2, trama, LEN_TRAMA);
        // registrar conexion Memoria compartida!
        conexion->id = id;
        conexion->online = 1;
        sem_wait(semaforo);
        insertarConexion(conexion, conexiones, numConexiones);
        sem_post(semaforo);
        free(conexion);
        break;
    case 'S':
        conexion = malloc(sizeof(Conexion));
        extraeDatos(datos, trama);
        cadena = strtok(datos, "*");
        strcpy(conexion->nom, cadena);
        cadena = strtok(NULL, "*");
        conexion->id = atoi(cadena);
        cadena = strtok(NULL, "*");
        strcpy(conexion->codigoPostal, cadena);

        printf("Rebut search %s de %s %d", conexion->codigoPostal, conexion->nom, conexion->id);
        display("Feta la cerca\n");
        bzero(datos, LEN_DATOS);
        sem_wait(semaforo);
        count = buscarPorCodigoPostal(conexion->codigoPostal, conexiones, numConexiones);
        sem_post(semaforo);
        fprintf(stdout, "Hi ha %d persones humanes a %s", count, conexion->codigoPostal);
        if (count > 0)
        {
            char *aux = malloc(sizeof(char *));
            sprintf(datos, "%d*", count);
            sem_wait(semaforo);
            for (int i = strlen(datos); i < *numConexiones; i++)
            {
                if (0 == strcmp(conexiones[i].codigoPostal, conexion->codigoPostal))
                {
                    fprintf(stdout, "%d %s", conexiones[i].id, conexiones[i].nom);
                    strcat(datos, conexiones[i].nom);
                    strcat(datos, "*");
                    sprintf(aux, "%d", conexiones[i].id);
                    strcat(datos, aux);
                    if (i < count)
                    {
                        strcat(datos, "*");
                    }
                }
            }
            sem_post(semaforo);
        }
        // display(datos);
        encapsulaTrama(MACHINE_NAME, 'L', datos, trama);
        write(sfd2, trama, LEN_TRAMA);
        break;
    case 'Q':
        conexion = malloc(sizeof(Conexion));
        extraeDatos(datos, trama);
        cadena = strtok(datos, "*");
        strcpy(conexion->nom, cadena);
        cadena = strtok(NULL, "*");
        conexion->id = atoi(cadena);
        desconectarCliente(conexiones, numConexiones);
        fprintf(stdout, "\nRebut logout de %s %d\n", conexion->nom, conexion->id);
        display("Desconnectat d’Atreides.\n");
        close(sfd2);
        exit(0);
        break;
    default:
        printf("ERROR trama Z\n\n");
        encapsulaTrama(MACHINE_NAME, 'Z', "“ERROR DE TRAMA”", trama);
        write(sfd2, trama, LEN_TRAMA);
        // desconexion
    }
}

int generarID(Conexion *conexiones, int *numConexiones, char *nom)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        if (0 == strcmp(conexiones[i].nom, nom))
        {
            return conexiones[i].id;
        }
    }

    return 1000 + *numConexiones;
}

int buscarPorCodigoPostal(char *codigoPostal, Conexion *conexiones, int *numConexiones)
{
    int count = 0;
    for (int i = 0; i < *numConexiones; i++)
    {
        if (0 == strcmp(conexiones[i].codigoPostal, codigoPostal))
        {
            count++;
        }
    }
    return count;
}