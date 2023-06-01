#include <sys/types.h>
#include <sys/socket.h>
#include "sys/un.h"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "requests_def.h"
#include "responses_def.h"
#include "server.h"

#define PORT 54321
#define SA struct sockaddr
#define SERVER_IP "0.0.0.0"

/// @brief Processing of incoming requests
void processRequest(Request request, int sockfd, SA *pcliaddr, socklen_t clilen)
{
    Response* response;
    response->type = request.type;

    // Process the request in the database
    if (request.type == GET_IMAGE_BY_MAIL) {
        ImageData* imageData = GET_photo(request.body.byMailRequest.mail);
        response->code = imageData->code;
        response->data.image.isFragIfOne = 1;

        printf("Response - code: %d, image size: %d\n", response->code, imageData->size);

        for (int i = 0; i < imageData->size; i++) {
            ImageFrag* frag = malloc(sizeof(ImageFrag)+(sizeof(imageData->data[i])*sizeof(char)));

            frag->size = sizeof(imageData->data[i]);
            frag->nPackages = imageData->size;
            frag->maxSizePerPackage = MAX_IMAGE_SIZE_PER_PACKAGE;
            frag->packageIndex = i;
            frag->imageSize = imageData->imageSize;

            strcpy(frag->mail, request.body.byMailRequest.mail);
            memcpy(frag->imageFrag, &(imageData->data[i]), sizeof(imageData->data[i]));

            //Copy fragment to request
            realloc(response, sizeof(Response)+(frag->size*sizeof(char)));
            memcpy(&(response->data.image.image.frag), frag, sizeof(ImageFrag)+(frag->size*sizeof(char)));
            free(frag);

            //Send
            sendto (sockfd, (void *) response, sizeof(Response) + sizeof(imageData->data[i])*sizeof(char), 0, pcliaddr, clilen);
        }
        return;
    }
    if (request.type == REGISTER)
        response = REGISTER_handler(request);
    else if (request.type == REMOVE_BY_MAIL)
        response = REMOVE_BY_MAIL_handler(request);
    else if (request.type == LIST_ALL)
        response = LIST_USER_handler(NULL);
    else if (request.type == LIST_BY_COURSE)
        response = LIST_BY_COURSE_handler(request);
    else if (request.type == LIST_BY_SKILL)
        response = LIST_BY_SKILL_handler(request);
    else if (request.type == LIST_BY_YEAR)
        response = LIST_BY_YEAR_handler(request);
    else if (request.type == GET_BY_MAIL)
        response = GET_BY_MAIL_handler(request);

    printf("Response - code: %d, nRegistry: %d\n", response->code, response->data.registries.nRegistry);

    sendto (sockfd, (void *) response, sizeof(Response) + sizeof(response->data), 0, pcliaddr, clilen);
}

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    Request request;
    Response* response;

    for ( ; ; ) {
        len = clilen;
        n = recvfrom (sockfd, (void *) &request, sizeof(Request), 0, pcliaddr, &len);

        if (n != -1) {
            processRequest(request, sockfd, pcliaddr, len);
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    bzero (&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    bind (sockfd, (SA *) &servaddr, sizeof(servaddr));

    dg_echo (sockfd, (SA *) &cliaddr, sizeof(cliaddr));

    exit(EXIT_SUCCESS);
}
