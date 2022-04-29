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