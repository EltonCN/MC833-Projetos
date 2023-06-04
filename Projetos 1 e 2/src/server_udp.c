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
        FragList* fragList = GET_photo(request.body.byMailRequest.mail);
        response->code = fragList->code;

        printf("Response - code: %d, image size: %d\n", response->code, fragList->size);

        if (response->code == 0 || response->size == 0) {
            response->data.image.isFragIfOne = 0;
            
            sendto (sockfd, (void *) response, sizeof(Response), 0, pcliaddr, clilen);
        }
        else {
            response->data.image.isFragIfOne = 1;

            for (int i = 0; i < fragList->size; i++) {
                //Copy fragment to request
                realloc(response, sizeof(Response)+(fragList->frags[i]->size*sizeof(char)));
                memcpy(&(response->data.image.image.frag), fragList->frags[i], sizeof(ImageFrag)+(fragList->frags[i]->size*sizeof(char)));

                //Send
                sendto (sockfd, (void *) response, sizeof(Response) + fragList->frags[i]->size*sizeof(char), 0, pcliaddr, clilen);
            }
        }

        free(fragList);
        
        return;
    }
    else if (request.type == SEND_IMAGE)
        response = INSERT_photo(request);
    else if (request.type == REGISTER)
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
