#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#include "requests_def.h"
#include "responses_def.h"

#define MAX_IMAGE_SIZE_PER_PACKAGE 500.0

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
    
    if(request->type == SEND_IMAGE)
    {
        int imageSize = request->body.imageRequest.image.image.imageSize;
        int nPackage = ceil(((float) imageSize)/MAX_IMAGE_SIZE_PER_PACKAGE);

        Request* fragRequest = malloc(sizeof(Request)); 
        request->body.imageRequest.isFragIfOne = 1;
        request->type = SEND_IMAGE;

        for(int i = 0; i<nPackage; i++)
        {

            ImageFrag* frag = getImageFrag(nPackage, imageSize, MAX_IMAGE_SIZE_PER_PACKAGE, 
                            &(request->body.imageRequest.image.image), 
                            &(request->body.imageRequest.image.image.mail),
                            i);
            
            realloc(request, sizeof(Request)+(frag->size*sizeof(char)));
            memcpy(&(request->body.imageRequest.image.frag), frag, sizeof(ImageFrag)+(frag->size*sizeof(char)));
            free(frag);

            send(sockfd, (void *) fragRequest, sizeof(Request)+(frag->size*sizeof(char)), 0);
        }
    }
    else
    {
        send(sockfd, (void *) request, sizeof(Request), 0);
    }

    Response *response = malloc(sizeof(Response));
    int return_code = recv(sockfd, (void *) response, sizeof(Response), MSG_WAITALL);

    if(return_code != sizeof(Response))
    {
        printf("ERROR RECEIVING RESPONSE. OPERATION MAY HAVE BEEN PERFORMED.\n");

        Response *response = malloc(sizeof(Response));
        response->code = 0;
        return response;
    }

    if(response->type == LIST_BY_COURSE ||
        response->type == LIST_BY_SKILL ||
        response->type == LIST_BY_YEAR ||
        response->type == LIST_ALL)
    {
        int nRegistry = response->data.registries.nRegistry;

        response = realloc(response, sizeof(Response)+(nRegistry*sizeof(Registry)));
        int return_code = recv(sockfd, (void *) response->data.registries.registries, (nRegistry*sizeof(Registry)), MSG_WAITALL);

        if(return_code != nRegistry*sizeof(Registry))
        {
            printf("ERROR RECEIVING RESPONSE. OPERATION MAY HAVE BEEN PERFORMED.\n");

            Response *response = malloc(sizeof(Response));
            response->code = 0;
            return response;
        }
    }

    if(response->type == GET_IMAGE_BY_MAIL)
    {
        int nPackage = response->data.image.image.frag.nPackages;
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