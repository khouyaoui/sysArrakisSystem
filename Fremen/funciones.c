#include "funciones.h"

// ----------------------------------  FREE Y APUNTAR A NULL  ----------------------------------
void liberarMemoria(void *ptr) {
    // Libermaos Memoria
    free(ptr);
    // Dejamos apuntando a NULL
    ptr = NULL;
}

// ----------------------------------  CONTROLAR PASO DE ARGUMENTOS  ----------------------------------
void errorNumArgumentos(int argc) {
    if (argc != NUM_ARG) {
        display(ARG_NUMBER_ERR);
        exit(EXIT_FAILURE);
    }
}

// ----------------------------------  CONTROLAR OBERTURA FICHERO  ----------------------------------
void errorAbrirFichero(int fd) {
    if (fd < 0) {
        display(FILE_NOT_OPEN_ERR);
        exit(EXIT_FAILURE);
    }
}

// ----------------------------------  CONTROLAR FICHERO VACIO  -------------------------------------
void errorFicheroVacio(int fd) {
    char precheck;
    if (read(fd, &precheck, 1) <= 0) {
        display(EMPTY_FILE_ERR);
        close(fd);
        exit(EXIT_FAILURE);
    }
    // En caso de no estar vacio reposicionamos el puntero por el byte leido anteriormente
    lseek(fd, -1, SEEK_CUR);
}

// ----------------------------------  MOSTRAR POR PANTALLA  ----------------------------------
void display(char *string) {
    write(1, string, sizeof(char) * strlen(string));
}

// ----------------------------------  LEER PALABRA FICHERO  ----------------------------------
char *readUntil(int fd, char end) {
    int i = 0;
    char c = '\0';
    char *string = (char *)malloc(sizeof(char));

    while (c != end) {
        read(fd, &c, sizeof(char));
        if (c != end) {
            string = (char *)realloc(string, sizeof(char) * (i + 2));
            string[i] = c;
        }
        i++;
    }

    string[i - 1] = '\0';
    return string;
}

// ----------------------------------  LEER CADENA TECLADO  ----------------------------------
char *readInput() {
    int i = 0;
    char *string = NULL;
    char caracter = ' ';  // Caracter neutro

    // Bucle para leer caracteres entrados por teclado hasta Enter
    while (caracter != '\n') {
        // Lectura de caracter por caracter
        read(0, &caracter, sizeof(char));

        // Redimensiona la cadena por cada caracter del usuario
        string = (char *)realloc(string, (sizeof(char) * (i + 1)));

        // Asigna el caracter a la cadena
        string[i] = caracter;

        // Cuando el caracter sea un salto de linea (enter) marcamos el fianl de la cadena
        if (caracter == '\n') {
            string[i] = '\0';
        }
        i++;
    }
    return string;
}

// ----------------------------------  EXTRAER PALABRAS DE UNA FRASE  ----------------------------------
char *extraerPalabra(char ***string, int *i) {
    int j = 0;
    // char *aux = NULL;
    char *palabra = NULL;

    // Bucle para recorrer el string hasta el final '\0'
    while ((**string)[*i] != '\0' && (**string)[*i] != ' ') {
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
int contarPalabras(char ***string) {
    int i = 0;
    int num_palabras = 1;
    int anterior_espacio = 0;

    // Bucle para recorrer el string hasta el final '\0'
    while ((**string)[i] != '\0') {
        if ((**string)[i] == ' ' && !anterior_espacio) {
            num_palabras++;
            anterior_espacio = 1;
        }
        if ((**string)[i] != ' ') anterior_espacio = 0;
        i++;
    }
    return num_palabras;
}

// ----------------------------------  PASAR FRASE AINUS  ----------------------------------
void pasarMinus(char *frase) {
    int i = 0;
    while (frase[i] != '\0') {
        if (frase[i] >= 'A' && frase[i] <= 'Z') {
            frase[i] = frase[i] - 'A' + 'a';
        }
        i++;
    }
}

void encapsulaTrama(char *origen, char tipo, char *datos, char *trama) {
    int j, i = LEN_ORIGEN + 1;
    int length = strlen(datos);

    bzero(trama, LEN_TRAMA);
    strcat(trama, origen);
    trama[LEN_ORIGEN] = tipo;

    for (j = 0; j < length; j++) {
        trama[i] = datos[j];
        i++;
    }
}


// ----------------------------------  CONTROLAR ERROR READ  ----------------------------------
void errorReadSocket(int num_bytes) {
    if (num_bytes < 0) {
        display(SOCKET_READ_ERR);
        raise(SIGINT);
    }
}
uint16_t controlaPuerto(char *user_port) {

    // Rango Puerto valido
    uint16_t port;
    int aux = atoi(user_port);
    if (aux < MIN_PORT_RANGE || aux > MAX_PORT_RANGE) {
        display(PORT_RANGE_ERR);
        raise(SIGINT);
    }

    // Si el puerto es valido lo asignamos a una variable uint16_t
    port = aux;

    return port;
}