#include "conexiones.h"
int inicializarListaConexiones(Conexion **conexiones, int **numConexiones)
{
    // indicador de nom que hi han al llistat
    *numConexiones = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    // llistat on guardarem
    *conexiones = mmap(NULL, sizeof(Conexion) * 50, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (conexiones == MAP_FAILED)
    {
        perror("conexiones error regio memoria mmap");
        return -1;
    }
    if (numConexiones == MAP_FAILED)
    {
        perror("numConexiones error regio memoria mmap");
        return -1;
    }
    bzero(conexiones, sizeof(Conexion) * 50);

    **numConexiones = 10;

    return 1;
}

void insertarConexion(Conexion *conexion, Conexion **conexiones, int **numConexiones)
{
    printf("1212");
    // conexiones[**numConexiones]->id = 2333;
    printf("%d", **numConexiones);
    // strcpy(conexiones[*numConexiones]->codigoPostal, conexion->codigoPostal);

    // conexiones[*numConexiones]->id = conexion->id;
    //  conexiones[0].nom = (char *)malloc(sizeof(char *));
    // strcpy(conexiones[*numConexiones]->nom, conexion->nom);
    // conexiones[**numConexiones]->online = conexion->online;
    printf("12wwewe12");

    // printf("%s\n", conexiones[*numConexiones]->nom);
    //  printConexiones(conexiones);

    printf("%p", *numConexiones);
}
// devuelve una conexion (Fremen)
Conexion *obtenirConexion(int id, Conexion *conexiones, int *numConexiones)
{
    printf("obter CC");
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
void extraerConexiones(Conexion **conexiones, int ***numConexiones)
{
    FILE *file;
    if (existenConexiones() == 1)
    {
        file = fopen("conexiones.txt", "a");
        if (file == NULL)
        {
            display("\nError al abrir l'arxiu conexiones.txt\n");
            exit(1);
        }
        for (int n = 0; n < ***numConexiones; n++)
        {
            fwrite(conexiones[n], sizeof(Conexion), 1, file);
        }
        /*
        if (fwrite != 0)
        {
            display("conexiones escritas en el disco !\n");
        }
        else
        {
            display("error al escriber las conexiones en el disco !\n");
        }
        */
    }
    else
    {
        file = fopen("conexiones.txt", "w");
        if (file == NULL)
        {
            display("\nError al crear l'arxiu conexiones.txt\n");
            exit(1);
        }
        for (int n = 0; n < ***numConexiones; n++)
        {
            fwrite(conexiones[n], sizeof(Conexion), 1, file);
        }
        /*
        if (fwrite != 0)
        {
            display("conexiones escritas en el disco !\n");
        }
        else
        {
            display("error al escriber las conexiones en el disco !\n");
        }
        */

        // abrimos en modo crear ya que no existe.
    }
    fclose(file);
}
// leer las conexiones guardadas anteriormente, si el fichero existe
/* void cargarConexiones(Conexion *conexiones)
{
    if (puedoCargarConexiones() == 1)
    {
        // leer conexiones del file
    }
    else
    {
        // no existen conexiones para cargarlas, es la primera vez que se ejecuta el programa
        // return NULL;
    }
}  */

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
    numConexiones = 0;
    for (int i = 0; i <= *numConexiones; i++)
    {
        printf("%s", conexiones[i].nom);
        printf("%s", conexiones[i].codigoPostal);
        printf("%d", conexiones[i].id);
        printf("%d", conexiones[i].online);
    }
}

int atenderCliente(Config_Data config, int sfd2, Conexion *conexiones, int *numConexiones, sem_t *semaforo, int id)
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
            return 0; // connexió tancada
        else
            contador += nbytes;

        if (contador == sizeof(trama))
        {
            // tenim una trama sencera
            gestionarTrama(sfd2, config, trama, conexiones, semaforo, id, numConexiones);
        }
    }
    display(TERMINAL_PROMPT);
}

void gestionarTrama(int sfd2, Config_Data config, char trama[LEN_TRAMA], Conexion *conexiones, sem_t *semaforo, int id, int *numConexiones)
{
    char aux[LEN_TRAMA]; // only per printar
    Conexion *conexion;
    char datos[LEN_DATOS];
    printf("%s", config.ip_server);
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
        sprintf(aux, "Recibiendo: %s", datos);
        display(aux);
        sprintf(datos, "%d", id);
        encapsulaTrama(MACHINE_NAME, 'O', datos, trama);
        write(sfd2, trama, LEN_TRAMA);
        // registrar conexion Memoria compartida!
        conexion->id = id;
        conexion->online = 1;
        sem_wait(semaforo);
        // insertarConexion(conexion, conexiones, numConexiones);

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

int generarID()
{
    // TODO same tests here
    // consultar la lista para no reptr el mismo ID al nou client
    return 01;
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