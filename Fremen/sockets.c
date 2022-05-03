#include "sockets.h"

void errorSocket(int sockfd) {
    if (sockfd < 0) {
        display(SOCKET_TCP_ERR);
    }
}

int establecerConexion(Config_Data *c) {
    int puerto, sockfd;  // numbytes
    // buffer[LEN_TRAMA];
    puerto = atoi(c->port_server);
    struct hostent *he;  // datos recibidos
    struct sockaddr_in server;

    if ((he = gethostbyname(c->ip_server)) == NULL) {
        display("client error en gethostbyname\n");
        exit(-1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        display("client error en abrir socket client\n");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);
    server.sin_addr = *((struct in_addr *)he->h_addr);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        display("client error en abrir socket connect\n");
        exit(-1);
    } else {
        return sockfd;
    }
    /*
    if ((numbytes = recv(sockfd, buffer, LEN_TRAMA, 0)) < 0)
    {
        display("client error en abrir socket recv\n");
        exit(-1);
    }
    buffer[numbytes] = '\n';
    display("mensaje del server\n");
    display(buffer);
    */
    return sockfd;
}

void extraeDatos(char *datos, char *trama) {
    int i, j = 0;

    for (i = LEN_ORIGEN + 1; i < LEN_TRAMA; ++i) {
        datos[j] = trama[i];
        j++;
    }
    datos[j] = '\0';
}