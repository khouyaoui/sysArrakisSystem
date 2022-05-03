#include "conexiones.h"
int inicializarListaConexiones(Conexion **conexiones, int **numConexiones)
{
    // indicador de num que hi han al llistat
    *numConexiones = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    // llistat on guardarem
    *conexiones = mmap(NULL, sizeof(Conexion) * MAX_CON, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
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
    bzero(*conexiones, sizeof(Conexion) * MAX_CON);

    **numConexiones = 0;

    return 1;
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
int existeConexion(int id, Conexion *conexiones, int *numConexiones, char *nomNovaConexion)
{
    if (id == 0)
    {
        int existe = 0;
        for (int i = 0; i < *numConexiones; i++)
        {
            if (0 == strcmp(nomNovaConexion, conexiones[i].nom))
            {
                existe = 1;
            }
        }
        return existe;
    }
    else
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

int atenderCliente(int sfd2, Conexion *conexiones, int *numConexiones, sem_t *semaforo)
{
    char trama[LEN_TRAMA] = {0};
    int contador = 0;
    Conexion *conexion = NULL;
    while (1)
    {
        bzero(trama, LEN_TRAMA);
        int nbytes = read(sfd2, trama + contador, sizeof(trama) - contador);
        if (nbytes < 0)
            return -1; // no hauria de passar...
        else if (nbytes == 0)
        {
            if (conexion != NULL)
            {
                desconectarCliente(conexiones, numConexiones, conexion->id);
            }
            // printf("ctrl+c detected from client\n");
            _exit(0);
        }
        else
        {
            contador += nbytes;
        }
        if (contador == sizeof(trama))
        {
            // tenim una trama sencera
            gestionarTrama(sfd2, trama, conexiones, semaforo, numConexiones, &conexion);
            contador = 0;
        }
        display(TERMINAL_PROMPT);
    }
}
void desconectarCliente(Conexion *conexiones, int *numConexiones, int id)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        if (conexiones[i].id == id)
        {
            conexiones[i].online = 0;
            display(conexiones[i].nom);
        }
    }
    display(" Desconnectat d’Atreides.\n");
    display(TERMINAL_PROMPT);
    // printConexiones(conexiones, numConexiones);
}

void gestionarTrama(int sfd2, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones, Conexion **conexion)
{
    File *file;
    char datos[LEN_DATOS];
    // int count;
    char aux[MAX_STR];
    char *cadena;
    switch (trama[LEN_ORIGEN])
    {
    case 'C':
        sem_wait(semaforo);
        *conexion = tratarNuevaConexion(trama, conexiones, numConexiones);
        sem_post(semaforo);

        // TODO si *conexion es null cal enviar una trama d'error, esta definida a
        if (*conexion == NULL)
        {
            // enviar trama d'error & exit 0
        }
        sprintf(aux, "\nRebut login %s %s\n", (*conexion)->nom, (*conexion)->codigoPostal);
        display(aux);
        sprintf(aux, "Assignat a ID %d.\nEnviada resposta\n\n", (*conexion)->id);
        display(aux);
        sprintf(datos, "%d", (*conexion)->id);
        encapsulaTrama(MACHINE_NAME, 'O', datos, trama);
        write(sfd2, trama, LEN_TRAMA);
        break;
    case 'S':
        sem_wait(semaforo);
        tratarComandaSearch(sfd2, trama, datos, conexiones, numConexiones, *conexion);
        sem_post(semaforo);
        break;
    case 'Q':
        if (*conexion != NULL)
        {
            sprintf(aux, "\nRebut logout de %s %d\n", (*conexion)->nom, (*conexion)->id);
            display(aux);
            desconectarCliente(conexiones, numConexiones, (*conexion)->id);
        }
        _exit(0);
        break;
    case 'F':
        file = malloc(sizeof(File));
        extraeDatos(datos, trama);
        cadena = strtok(datos, "*");
        strcpy(file->nom, cadena);
        cadena = strtok(NULL, "*");
        file->mida = atoi(cadena);
        cadena = strtok(NULL, "*");

        write(0, datos, LEN_DATOS);
        break;
    default:
        printf("ERROR trama Z\n\n");
        encapsulaTrama(MACHINE_NAME, 'Z', "“ERROR DE TRAMA”", trama);
        write(sfd2, trama, LEN_TRAMA);
        // desconexion
    }
}
