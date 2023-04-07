#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include "requests_def.h"
#include "responses_def.h"
#include "server.h"

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
        Bind(sockfd, (SA *) &server, sizeof(server));
        Listen(sockfd, LISTENQ);
    }

    if(sockfd == -1)
    {
        printf("ERROR OPENING THE SOCKET. CAN'T DO NOTHING, SORRY.");
        exit(EXIT_FAILURE);
    }
}

void processRequest(int client_fd)
{
    Request request;
    Response response;

    recv(client_fd, (void *) &request, sizeof(Request), MSG_WAITALL);

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

    send(client_fd, (void *) &response, sizeof(Response), 0);
}

int main(int argc, char **argv)
{
    int new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    openSocket();
    
    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        new_fd= Accept(sockfd, (SA *) &cliaddr, &clilen);

        if ( (childpid = Fork()) == 0) {    // child process
            Close(sockfd);                  // close listening socket
            processRequest(new_fd);         // process the request
            exit (EXIT_SUCCESS);
        }

        Close(new_fd); // parent closes connected socket
    }

    exit(EXIT_SUCCESS);
}