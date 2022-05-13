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
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        display("client error en abrir socket client\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        display("No s'ha pogut trobar el servidor\n");
        return -1;
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

void extraeDatosBinarios(char *datos, char *trama) {
    int i, j = 0;

    for (i = LEN_ORIGEN + 1; i < LEN_TRAMA; ++i) {
        datos[j] = trama[i];
        j++;
    }
}
