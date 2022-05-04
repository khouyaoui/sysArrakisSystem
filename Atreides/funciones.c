#include "funciones.h"

// ----------------------------------  FREE Y APUNTAR A NULL  ----------------------------------
void liberarMemoria(void *ptr)
{
    // Libermaos Memoria
    free(ptr);
    // Dejamos apuntando a NULL
    ptr = NULL;
}

// ----------------------------------  CONTROLAR PASO DE ARGUMENTOS  ----------------------------------
void errorNumArgumentos(int argc)
{
    if (argc != NUM_ARG)
    {
        display(ARG_NUMBER_ERR);
        exit(EXIT_FAILURE);
    }
}

// ----------------------------------  CONTROLAR OBERTURA FICHERO  ----------------------------------
void errorAbrirFichero(int fd)
{
    if (fd < 0)
    {
        display(FILE_NOT_OPEN_ERR);
        exit(EXIT_FAILURE);
    }
}

// ----------------------------------  CONTROLAR FICHERO VACIO  -------------------------------------
void errorFicheroVacio(int fd)
{
    char precheck;
    if (read(fd, &precheck, 1) <= 0)
    {
        display(EMPTY_FILE_ERR);
        close(fd);
        exit(EXIT_FAILURE);
    }
    // En caso de no estar vacio reposicionamos el puntero por el byte leido anteriormente
    lseek(fd, -1, SEEK_CUR);
}

// ----------------------------------  MOSTRAR POR PANTALLA  ----------------------------------
void display(char *string)
{
    write(1, string, sizeof(char) * strlen(string));
}

// ----------------------------------  LEER PALABRA FICHERO  ----------------------------------
char *readUntil(int fd, char end)
{
    int i = 0;
    char c = '\0';
    char *string = (char *)malloc(sizeof(char));

    while (c != end)
    {
        read(fd, &c, sizeof(char));
        if (c != end)
        {
            string = (char *)realloc(string, sizeof(char) * (i + 2));
            string[i] = c;
        }
        i++;
    }

    string[i - 1] = '\0';
    return string;
}
// sockets
// ---------------------------------- CONEXIONES ----------------------------------
void esperarConexiones(Config_Data *c)
{
    int s_fd;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        display("Error al abrir el socket!");
    }
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = atoi(c->port_server);
    direccion.sin_addr.s_addr = INADDR_ANY;

    if (bind(s_fd, (struct sockaddr *)&direccion, sizeof(direccion)) == -1)
    {
        printf("Error en el bind\n");
    }
    if (listen(s_fd, 1) == -1)
    {
        printf("Error\n");
    }
}

// ----------------------------------  CONTROLAR ERROR READ  ----------------------------------
void errorReadSocket(int num_bytes)
{
    if (num_bytes < 0)
    {
        display(SOCKET_READ_ERR);
        raise(SIGINT);
    }
}

// ----------------------------------  CONTROLAR PURTO VALIDO  --------------------------------
uint16_t controlaPuerto(char *user_port)
{

    // Rango Puerto valido
    uint16_t port;
    int aux = atoi(user_port);
    if (aux < MIN_PORT_RANGE || aux > MAX_PORT_RANGE)
    {
        display(PORT_RANGE_ERR);
        raise(SIGINT);
    }

    // Si el puerto es valido lo asignamos a una variable uint16_t
    port = aux;

    return port;
}
// -------------  PEDIR MEMORIA COMPARTIDA PARA LA LISTA DE CONEXIONES  ------------------------
sem_t *inicializarSemaforo()
{
    sem_t *semaforo = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (semaforo == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    sem_init(semaforo, 1, 1);
    sem_post(semaforo);
    return semaforo;
}
// --------------------------  TRATAR LA COMANDA LOGIN  -----------------------------------------
Conexion *tratarNuevaConexion(char *trama, Conexion *conexiones, int *numConexiones)
{
    char *login = trama + 16;
    char *aux;
    for (aux = login; *aux != '*'; aux++)
        ;

    // he assumit que la trama és correcta
    *aux = 0;
    char *cp = aux + 1;
    // assumim que després del codi postal hi ha tot \0
    // assumim qeu numConexiones és un int *
    return recuperarConexion(login, cp, conexiones, numConexiones);
}
// -------------------  RECUPERAR UNA CONEXION DESDE LA LISTA  ------------------------------------
Conexion *recuperarConexion(char *login, char *cp, Conexion *conexiones, int *numConexiones)
{
    // assumeixo que no tinc ni NULL ni valors negatius
    for (int i = 0; i < *numConexiones; i++)
        if (strcmp(login, conexiones[i].nom) == 0)
        {
            conexiones[i].online = 1;
            strcpy(conexiones[i].codigoPostal, cp);
            return conexiones + i;
        }
    // si hem arribat aquí, és que no està a la llista
    // i si no queda lloc per al nou usuari?
    if (*numConexiones == MAX_CON)
        return NULL;
    // sí queda lloc per al nou usuari
    strcpy(conexiones[*numConexiones].nom, login);
    strcpy(conexiones[*numConexiones].codigoPostal, cp);
    conexiones[*numConexiones].id = 1000 + *numConexiones;
    conexiones[*numConexiones].online = 1;
    (*numConexiones)++;
    return conexiones + *numConexiones - 1;
}
// ---------------------------  TRATAR LA COMANDA SEARCH  ----------------------------------------
void tratarComandaSearch(int sfd2, char *trama, char *datos, Conexion *conexiones, int *numConexiones, Conexion *conexion)
{
    char *cadena;
    char aux[MAX_STR];
    extraeDatos(datos, trama);
    cadena = strtok(datos, "*");
    cadena = strtok(NULL, "*");
    cadena = strtok(NULL, "*");
    char codigoPostal[10];
    strcpy(codigoPostal, cadena);
    sprintf(aux, "Rebut search per el codi postal %s de %s amb id %d\n", codigoPostal, conexion->nom, conexion->id);
    display(aux);
    display("Feta la cerca\n");
    bzero(datos, LEN_DATOS);
    int count = buscarPorCodigoPostal(codigoPostal, conexiones, numConexiones);
    if (count == 1)
    {
        sprintf(aux, "Hi ha una persona humana a %s\n", codigoPostal);
    }
    else if (count == 0)
    {
        sprintf(aux, "No hi ha cap persona humana a %s\n", codigoPostal);
    }
    else
    {
        sprintf(aux, "Hi ha %d persones humanes a %s\n", count, codigoPostal);
    }
    display(aux);
    if (count > 0)
    {
        char *auxID = malloc(sizeof(char));
        sprintf(datos, "%d*", count);
        for (int i = 0; i < *numConexiones && i < LEN_DATOS; i++)
        {
            if (0 == strcmp(conexiones[i].codigoPostal, codigoPostal))
            {
                sprintf(aux, "- %d %s\n", conexiones[i].id, conexiones[i].nom);
                display(aux);
                strcat(datos, conexiones[i].nom);
                strcat(datos, "*");
                sprintf(auxID, "%d", conexiones[i].id);
                strcat(datos, auxID);
                if (i < count && i < (int)strlen(datos))
                {
                    strcat(datos, "*");
                }
            }
        }
        free(auxID);
    }
    else
    {
        sprintf(datos, "%d*", "0");
    }
    encapsulaTrama(MACHINE_NAME, 'L', datos, trama);
    write(sfd2, trama, LEN_TRAMA);
}
// -----------------------  BUSCAR EN LA LISTA SEGUN PARAMENTRO DE ENTRADA --------------------------
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

void tratarComandasFiles(int sfd2, char *trama, char *datos, Conexion *conexiones, int *numConexiones, Conexion *conexion)
{
    if (trama[LEN_ORIGEN] == 'F')
    {
        char *cadena;
        char hashAUX[HASH_LEL];
        File *file = malloc(sizeof(File));
        calcularHash(hashAUX, "ATREIDES.exe");
        extraeDatos(datos, trama);
        cadena = strtok(datos, "*");
        strcpy(file->nom, cadena);
        cadena = strtok(NULL, "*");
        file->mida = atoi(cadena);
        cadena = strtok(NULL, "*");
        strcpy(file->hash, cadena);
        fprintf(stdout, "\n %s\n %d\n %s\n NUEVO HASH %s\n", file->hash, file->mida, file->nom, hashAUX);
    }
    if (trama[LEN_ORIGEN] == 'D')
    {
        write(0, "\nrebuda trama Data\n", strlen("\nrebuda trama Data\n"));
        extraeDatosBinarios(datos, trama);
        int fd_bin = open("./imagen_bin.jpg", O_WRONLY | O_APPEND | O_CREAT, 0666);
        int num_bytes2 = write(fd_bin, datos, LEN_DATOS);
        printf("2) num_bytes2 binario = %d\n", num_bytes2);
    }
}
void calcularHash(char *hash, char *fileName)
{
    int child_status;
    int canals[2];
    if (pipe(canals) == -1)
        exit(-1);
    int ret = fork();
    switch (ret)
    {
    case 0:
        dup2(canals[1], STDOUT_FILENO);
        close(canals[0]);
        close(canals[1]);
        execl("/usr/bin/md5sum", "md5sum", fileName, (char *)NULL);
        exit(0);
        break;
    case -1:
        write(0, "Error fork calcularHash funciones.c", strlen("Error fork calcularHash funciones.c"));
        break;
    default:
        waitpid(ret, &child_status, 0);
        close(canals[1]);
        int nbytes = read(canals[0], hash, 32);
        close(canals[0]);
        break;
    }
}