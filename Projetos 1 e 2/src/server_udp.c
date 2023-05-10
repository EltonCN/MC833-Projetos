#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <unp.h>

#include "requests_def.h"
#include "responses_def.h"
#include "server.h"

#define PORT 54321
#define SERVER_IP "0.0.0.0"

/// @brief Processing of incoming requests
Response* processRequest(Request request;)
{
    Response* response;

    // Process the request in the database
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

    printf("Response - code: %d, nRegistry: %d\n", response->code, response->registries.nRegistry);

    return response;
}

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    Request request;
    Response* response;

    for ( ; ; ) {
        len = clilen;
        n = recvfrom (sockfd, request, sizeof(Request), 0, pcliaddr, &len);
        response = processRequest(request);
        sendto (sockfd, (void *) response, sizeof(Response) + (response->registries.nRegistry*sizeof(Registry)), 0, pcliaddr, len);
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    bzero (&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (PORT);
    servaddr.sin_addr.s_addr = htonl (SERVER_IP);

    bind (sockfd, (SA *) &servaddr, sizeof(servaddr));

    dg_echo (sockfd, (SA *) &cliaddr, sizeof(cliaddr));

    exit(EXIT_SUCCESS);
}
