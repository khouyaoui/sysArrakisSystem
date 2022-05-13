#define _GNU_SOURCE
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

// ----------------------------------  LEER CADENA TECLADO  ----------------------------------
char *readInput(void)
{
    int i = 0;
    char *string = NULL;
    char caracter = ' '; // Caracter neutro

    // Bucle para leer caracteres entrados por teclado hasta Enter
    while (caracter != '\n')
    {
        // Lectura de caracter por caracter
        read(0, &caracter, sizeof(char));

        // Redimensiona la cadena por cada caracter del usuario
        string = (char *)realloc(string, (sizeof(char) * (i + 1)));

        // Asigna el caracter a la cadena
        string[i] = caracter;

        // Cuando el caracter sea un salto de linea (enter) marcamos el fianl de la cadena
        if (caracter == '\n')
        {
            string[i] = '\0';
        }
        i++;
    }
    return string;
}

// ----------------------------------  EXTRAER PALABRAS DE UNA FRASE  ----------------------------------
char *extraerPalabra(char ***string, int *i)
{
    int j = 0;
    // char *aux = NULL;
    char *palabra = NULL;

    // Bucle para recorrer el string hasta el final '\0'
    while ((**string)[*i] != '\0' && (**string)[*i] != ' ')
    {
        palabra = (char *)realloc(palabra, (sizeof(char *) * (j + 1)));
        palabra[j] = (**string)[*i];
        *i = *i + 1;
        j++;
    }
    // char *toma = {'\0'};
    palabra[j] = '\0';
    // palabra = (char *)malloc(sizeof(char) * strlen(aux));
    // strcpy(palabra, aux);
    // printf("La palabra: %s\n", *palabra);
    return palabra;
}

// ----------------------------------  CONTAR PALABRAS DE UNA FRASE  ----------------------------------
int contarPalabras(char ***string)
{
    int i = 0;
    int num_palabras = 1;
    int anterior_espacio = 0;

    // Bucle para recorrer el string hasta el final '\0'
    while ((**string)[i] != '\0')
    {
        if ((**string)[i] == ' ' && !anterior_espacio)
        {
            num_palabras++;
            anterior_espacio = 1;
        }
        if ((**string)[i] != ' ')
            anterior_espacio = 0;
        i++;
    }
    return num_palabras;
}

// ----------------------------------  PASAR FRASE AINUS  ----------------------------------
void pasarMinus(char *frase)
{
    int i = 0;
    while (frase[i] != '\0')
    {
        if (frase[i] >= 'A' && frase[i] <= 'Z')
        {
            frase[i] = frase[i] - 'A' + 'a';
        }
        i++;
    }
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

// ----------------------------------  CONTROLAR ERROR READ  ----------------------------------
void errorReadSocket(int num_bytes)
{
    if (num_bytes < 0)
    {
        display(SOCKET_READ_ERR);
        raise(SIGINT);
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
        // int nbytes = read(canals[0], hash, 32);
        read(canals[0], hash, 32);
        close(canals[0]);
        break;
    }
}

void encapsulaTramaBinaria(char *origen, char tipo, char *datos, char *trama)
{
    int j, i = LEN_ORIGEN + 1;
    bzero(trama, LEN_TRAMA);
    strcat(trama, origen);
    trama[LEN_ORIGEN] = tipo;

    for (j = 0; j < LEN_DATOS; j++)
    {
        trama[i] = datos[j];
        i++;
    }
}

int calcularMida(int fd)
{
    struct stat st;
    fstat(fd, &st);
    return (int)st.st_size;
}
// TODO revisar
void crearFichero(int ID, char *directorio, File **file, char *trama)
{
    *file = malloc(sizeof(File));
    asprintf(&((*file)->nom), "%s/%d.jpg", directorio, ID); // calcular memoria i demanarla -> familia sprintf // fa el malloc
    // .jpg
    char *aux = trama + 16;
    for (; *aux != '*'; aux++)
        ;
    char *mida = aux + 1;
    for (aux++; *aux != '*'; aux++)
        ;
    *aux = 0;
    (*file)->mida = atoi(mida);
    memcpy((*file)->hash, aux + 1, HASH_LEN);
    (*file)->fd = open((*file)->nom, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if ((*file)->fd < 0)
    {
        // enviar trama de error
        //  fer frees
    }
}
// TODO revisar
void leerDatosIMG(int sfd, File **imagen, char *trama)
{
    while ((*imagen)->mida != 0)
    {
        read(sfd, trama, LEN_TRAMA);
        if ((*imagen)->mida >= LEN_DATOS)
        {
            write((*imagen)->fd, trama + 16, LEN_DATOS);
            (*imagen)->mida -= LEN_DATOS;
        }
        else
        {
            write((*imagen)->fd, trama + 16, (*imagen)->mida);
            (*imagen)->mida = 0;
        }
        if ((*imagen)->mida == 0)
        {
            close((*imagen)->fd);
            char downloadedHash[32];
            calcularHash(downloadedHash, (*imagen)->nom);
            if (0 == memcmp(downloadedHash, (*imagen)->hash, 32))
            {
                encapsulaTrama(MACHINE_NAME, 'I', "IMAGE OK", trama);
                write(sfd, trama, LEN_TRAMA);
            }
            else
            {
                encapsulaTrama(MACHINE_NAME, 'R', "“IMAGE KO", trama);
                write(sfd, trama, LEN_TRAMA);
            }           
        }
    }
    free((*imagen)->nom);
    free((*imagen));
}

int validarNomImagen(char nomImagen[])
{
    for (int i = 0; i < (int)strlen(nomImagen); i++)
    {
        if (!isdigit(nomImagen[i]))
        {
            return -1;
        }
    }
    return 0;
}
int ocultarDirectorios(const struct dirent *arg)
{
    // Para que no nos devuelva el directorio actual (.) y el anterior (..)
    if (strcmp(arg->d_name, ".") == 0 || strcmp(arg->d_name, "..") == 0)
    {
        return 0;
    }
    return 1;
}
int existePhoto(char *photo_id,char *directorio)
{
    int i, num_archivos;
    struct dirent **archivos;
    num_archivos = scandir(directorio, &archivos, ocultarDirectorios, alphasort);

    if (num_archivos <= 0)
    {
        display(DIR_EMPTY_ERR);
    }
    else
    {
        for (i = 0; i < num_archivos; i++)
        {
            // Identificamos el tipo de archivo segun la extension
            archivos[i]->d_type = archivos[i]->d_name[strlen(archivos[i]->d_name) - 1];
            if (archivos[i]->d_type == 'g')
            {
                if (!strcmp(archivos[i]->d_name, photo_id))
                {
                    // calcu
                    return 1;
                }
            }
        }
        // FREE de cada elemento del Struct dirent
        for (i = 0; i < num_archivos; i++)
        {
            liberarMemoria(archivos[i]);
        }
        liberarMemoria(archivos);
    }
    return 0;
}