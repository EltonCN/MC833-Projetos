#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include "requests_def.h"
#include "responses_def.h"

#define PORT 54321
#define SERVER_IP "192.etc"

int sockfd = -1;

void openSocket()
{
    if(sockfd == -1)
    {
        struct addrinfo addr_info;

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = inet_addr(SERVER_IP);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
    }

    if(sockfd == -1)
    {
        printf("ERROR OPENING THE SOCKET. CAN'T DO NOTHING, SORRY.");
        exit(EXIT_SUCCESS);
    }
}

void closeSocket()
{
    if(sockfd != -1)
    {
        close(sockfd);
    }
}

Response sendAndReceive(Request request)
{
    openSocket();

    int len = sizeof(Request);
    send(sockfd, (void *) &request, len, 0);

    Response response;
    recv(sockfd, (void *) &response, sizeof(Response), MSG_WAITALL);

    int nRegistry = response.registries.nRegistry;
    if(nRegistry != 0)
    {
        recv(sockfd, (void *) &response.registries.registries, nRegistry*sizeof(Registry), MSG_WAITALL);
    }
}