#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "requests_def.h"
#include "responses_def.h"

#define PORT 54321
char* SERVER_IP = "127.0.0.1";

int sockfd = -1;

/// @brief Opens the UDP socket
void openSocket()
{
    struct addrinfo addr_info;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in));

    struct timeval tv;
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

}

// Public library functions (documentation in header) ---------------------------------

Response* sendAndReceive(Request *request)
{
    openSocket();

    int len = sizeof(Request);
    
    if(request->type == SEND_IMAGE)
    {
        len += request->body.imageRequest.image.imageSize;
    }

    send(sockfd, (void *) request, len, 0);

    Response *response = malloc(sizeof(Response));
    int return_code = recv(sockfd, (void *) response, sizeof(Response), MSG_WAITALL);

    if(return_code != sizeof(Response))
    {
        printf("ERROR RECEIVING RESPONSE. OPERATION MAY HAVE BEEN PERFORMED.\n");

        Response *response = malloc(sizeof(Response));
        response->code = 0;
        return response;
    }

    int nRegistry = response->nRegistry;
    int imageSize = response->imageSize;

    int dataSize = 0;
    if(nRegistry != 0)
    {
        dataSize = nRegistry*sizeof(Registry);
    }
    else if(imageSize != 0)
    {
        dataSize = imageSize;
    }

    if(dataSize > 0)
    {
        response = realloc(response, sizeof(Response)+dataSize);
        int return_code = recv(sockfd, (void *) response->data, dataSize, MSG_WAITALL);

        if(return_code != dataSize)
        {
            printf("ERROR RECEIVING RESPONSE. OPERATION MAY HAVE BEEN PERFORMED.\n");

            Response *response = malloc(sizeof(Response));
            response->code = 0;
            return response;
        }

    }

    return response;
}

void registerExit()
{
    
}

void changeServerIp(char* serverIp)
{
    SERVER_IP = serverIp; 

    if(sockfd != -1)
    {
        openSocket();
    }
}