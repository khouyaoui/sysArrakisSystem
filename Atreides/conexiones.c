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
// escriber la lista de conexiones en un fichero txt o binario :<
void extraerConexiones(Conexion *conexiones, int *numConexiones)
{
    int fd = open("conexiones.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        display("\nError al abrir l'arxiu conexiones.txt\n");
        exit(1);
    }

    for (int n = 0; n < *numConexiones; n++)
    {
        write(fd, conexiones + n, sizeof(Conexion));
    }
    close(fd);
}

// leer las conexiones guardadas anteriormente, si el fichero existe
int cargarConexiones(Conexion *conexiones)
{

    int fd = open("conexiones.txt", O_RDONLY);
    if (fd < 0)
    {
        display("\nError al abrir l'arxiu conexiones.txt\n");
        exit(1);
    }
    int n = 0;
    while (0 < read(fd, conexiones + n, sizeof(Conexion)))
    {
        n++;
    }

    close(fd);
    return n;
}

int existenConexiones()
{
    int existe = 0;
    FILE *file;
    file = fopen("conexiones.txt", "r");
    if (file)
    {
        existe = 1;
        fclose(file);
    }
    return existe;
}

void printConexiones(Conexion *conexiones, int *numConexiones)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        printf("client %d\n", i + 1);
        printf("%s\n", conexiones[i].nom);
        printf("%s\n", conexiones[i].codigoPostal);
        printf("%d\n", conexiones[i].id);
        printf("%d\n", conexiones[i].online);
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
            desconectarCliente(conexiones, numConexiones, config);
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
    }
    display(TERMINAL_PROMPT);
}
void desconectarCliente(Conexion *conexiones,int* numConexiones, Config_Data config)
{
    for (int i = 0; i < *numConexiones; i++)
    {
        if (conexiones[i].id == id)
        {
        }
        printf("client %d\n", i + 1);
        printf("%s\n", conexiones[i].nom);
        printf("%s\n", conexiones[i].codigoPostal);
        printf("%d\n", conexiones[i].id);
        printf("%d\n", conexiones[i].online);
    }
}

void gestionarTrama(int sfd2, Config_Data config, char *trama, Conexion *conexiones, sem_t *semaforo, int *numConexiones)
{
    Conexion *conexion;
    char datos[LEN_DATOS];
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
        int id = generarID(conexiones, numConexiones, conexion->nom);
        sprintf(datos, "%d", id);
        encapsulaTrama(MACHINE_NAME, 'O', datos, trama);
        write(sfd2, trama, LEN_TRAMA);
        // registrar conexion Memoria compartida!
        conexion->id = id;
        conexion->online = 1;
        sem_wait(semaforo);
        insertarConexion(conexion, conexiones, numConexiones);
        printConexiones(conexiones, numConexiones);
        sem_post(semaforo);

        break;
    case 'S':
        // buscar en la lista segun el codigo postal recibido
        muestraDatos(trama);
        // Conexion *c;
        //  zona critica, consulta lista que esta memoria compartida
        sem_wait(semaforo);
        // c = obtenirConexion(00001,conexiones);
        sem_post(semaforo);
        extraeDatos(datos, trama);
        // display("Rebut search 08301 de Moha 744\n");
        // display("Hi ha 1 persones humanes a 08022\n");
        // display("Feta la cerca\n");
        // encapsulaTrama(MACHINE_NAME, 'L', datos, trama);
        // write(sfd2, trama, LEN_TRAMA);
        //  enviar las conexiones de ese codigo postal
        break;
    case 'Q':
        muestraDatos(trama);
        printf("Adeu Fremen !!\n\n");
        close(sfd2);
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

void encapsulaTrama(char *origen, char tipo, char *datos, char *trama)
{
    int j, i = LEN_ORIGEN + 1;
    int length = strlen(datos);

    bzero(trama, LEN_TRAMA);
    strcat(trama, origen);
    trama[LEN_ORIGEN] = tipo;

    for (j = 0; j < length; j++)
    {
        trama[i] = datos[j];
        i++;
    }
}