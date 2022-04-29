#ifndef TIPOS_H
#define TIPOS_H

// Zona de CONSTANTES
#define MAX_PORT_RANGE 65535
#define MIN_PORT_RANGE 1
#define MAX_STR 512
#define NUM_ARG 2
#define LOGIN "login"
#define LOGIN_ARG 2
#define SEARCH "search"
#define SEARCH_ARG 1
#define SEND "send"
#define SEND_ARG 1
#define PHOTO "photo"
#define PHOTO_ARG 1
#define LOGOUT "logout"
#define LOGOUT_ARG 1
//tramas
#define LEN_ORIGEN 15
#define LEN_TRAMA 256
#define LEN_DATOS 240
// Mensajes
#define MACHINE_NAME "ATRAIDES"
#define INIT_MSG "\nSERVIDOR ATREIDES\nLlegit el fitxer de configuració\n"
#define LOGOUT_MSG "\nDesconectat d'Atreides. Dew!\n"
#define DIR_EMPTY_ERR "No hi ha fitxers disponibles\n"
#define TERMINAL_PROMPT "\nEsperant connexions...\n\n"
#define FINAL_MSG "\nADEU a Atreides\n"
#define SOCKET_TCP_ERR "\nerror socket"
#define PORT_RANGE_ERR "ERROR, el rango del puerto es invalido\n"

// ERROR_MSG
#define ARG_NUMBER_ERR "ERROR, el numero d' arguments no es correcte\n"
#define DIR_NOT_EXIST_ERR "ERROR, el directori especifcat no existeix\n"
#define FILE_NOT_OPEN_ERR "ERROR, no se pot obrir el fitxer\n"
#define EMPTY_FILE_ERR "ERROR, el fitxer indicat esta buit\n"
#define SOCKET_READ_ERR "ERROR, en la lectura del socket Accept\n"

typedef struct {
    char *ip_server;
    char *port_server;
    char *directorio;
} Config_Data;
typedef struct
{
  int id;			    // identificador asignado por atraides
  char codigoPostal[10];		// codigo postal del Fremen
  char nom[20];			// nombre del Fremen
  int online;			// 0 | 1 Esta conectado B??
} Conexion;
Config_Data c;
Conexion *conexiones;
int *numConexiones;
#endif