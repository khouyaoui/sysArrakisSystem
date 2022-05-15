#include "sockets.h"

int crearConexion(uint16_t port)
{
    int sockfd;
    struct sockaddr_in server;
    // server config
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY; // cualquier cliente
    bzero(&(server.sin_zero), 8);        // rellenar con 0's
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        display("server error al abrir function => socket\n");
        exit(-1);
    }
    //reutilizar la ip ocupada por bind
    int opt = 1;
    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
      
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
    {
        display("Port ocupat per un altre proces\n");
        perror("bind");
        exit(-1);
    }
    if (listen(sockfd, 5) < 0)
    {
        display("server error al abrir function => listen\n");
        exit(-1);
    }
    return sockfd;
}

void extraeDatos(char *datos, char *trama)
{
    int i, j = 0;

    for (i = LEN_ORIGEN + 1; i < LEN_TRAMA; ++i)
    {
        datos[j] = trama[i];
        j++;
    }
    datos[j] = '\0';
}

int aceptarConexion(int sfd1)
{
    int sfd2;
    struct sockaddr_in client;
    socklen_t longitud_cliente = sizeof(client);
    sfd2 = accept(sfd1, (struct sockaddr *)&client, &longitud_cliente);
    if (sfd2 < 0)
    {
        display("server error en accept");
        exit(-1);
    }
    return sfd2;
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

void extraeDatosBinarios(char *datos, char *trama) {
    int i, j = 0;

    for (i = LEN_ORIGEN + 1; i < LEN_TRAMA; ++i) {
        datos[j] = trama[i];
        j++;
    }
}