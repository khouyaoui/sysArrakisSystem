#ifndef TIPOS_H
#define TIPOS_H

#define HASH_LEN 32
#define MAX_STR 512
#define NUM_ARG 2
#define LOGIN "login"
#define LOGIN_ARG 3
#define SEARCH "search"
#define SEARCH_ARG 2
#define SEND "send"
#define SEND_ARG 2
#define PHOTO "photo"
#define PHOTO_ARG 2
#define LOGOUT "logout"
#define LOGOUT_ARG 1
#define MIN_PORT_RANGE 1
#define MAX_PORT_RANGE 65535
#define LEN_ORIGEN 15
#define LEN_TRAMA 256
#define LEN_DATOS 240
#define PORT_RANGE_ERR "ERROR, el rango del puerto es invalido\n"
#define MACHINE_NAME "FREMEN"

#define INIT_MSG "\nBenvingut a Fremen"
#define LOGOUT_MSG "\nDesconectat d'Atreides. Dew!\n"
#define DIR_EMPTY_ERR "No hi ha fitxers disponibles\n"
#define TERMINAL_PROMPT "\n$ "
#define FINAL_MSG "\nADEU a Fremen\n"

#define ARG_NUMBER_ERR "ERROR, el numero d' arguments no es correcte\n"
#define DIR_NOT_EXIST_ERR "ERROR, el directori especifcat no existeix\n"
#define FILE_NOT_OPEN_ERR "ERROR, no se pot obrir el fitxer\n"
#define EMPTY_FILE_ERR "ERROR, el fitxer indicat esta buit\n"
#define COMMNAND_ERR "Error, la comanda no s'ha pogut executar\n"

#define SOCKET_TCP_ERR "\nerror socket"
#define SOCKET_READ_ERR "ERROR, en la lectura del socket\n"

typedef struct {
    int segundos;
    char *ip_server;
    char *port_server;
    char *directorio;
} Config_Data;

typedef struct
{
    char *nom;
    int mida;
    char hash[32];
    int fd;
} File;

#endif