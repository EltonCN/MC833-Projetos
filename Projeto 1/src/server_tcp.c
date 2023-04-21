#include <sys/types.h>
#include <sys/socket.h>
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
#define SERVER_IP "127.0.0.1"
#define SA struct sockaddr
#define LISTENQ 5

int sockfd = -1;

/// @brief Opens the TCP socket
void openSocket()
{
    if(sockfd == -1)
    {
        struct sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(PORT);
        serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd == -1)
        {
            printf("ERROR OPENING THE SOCKET. CAN'T DO NOTHING, SORRY.\n");
            exit(EXIT_FAILURE);
        }

        if ((bind(sockfd, (SA*)&serveraddr, sizeof(serveraddr))) != 0)
        {
            printf("SOCKET BIND FAILED!\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        if ((listen(sockfd, LISTENQ)) != 0)
        {
            printf("LISTEN FAILED!\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("OPEN SOCKET - %d\n", sockfd);
    }
}

/// @brief Treatment for zombie processes
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/// @brief Processing of incoming requests
void processRequest(int client_fd)
{
    int recv_response;
    Request request;
    Response* response;

    // Loop until the connection is closed by the client
    do
    {
        // Receives the request
        recv_response = recv(client_fd, (void *) &request, sizeof(Request), MSG_WAITALL);

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

        // Send the result
        send(client_fd, (void *) response, sizeof(Response) + (response->registries.nRegistry*sizeof(Registry)), 0);

        free(response);
    } while (recv_response != 0 && recv_response != -1);
}

int main(int argc, char **argv)
{
    int new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    struct sigaction sa;

    openSocket();

    for ( ; ; ) {
        clilen = sizeof(cliaddr);

        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }

        new_fd= accept(sockfd, (SA *) &cliaddr, &clilen);

        if ( (childpid = fork()) == 0) {    // child process
            printf("FORK\n");
            close(sockfd);                  // close listening socket
            processRequest(new_fd);         // process the request
            printf("CLOSE FORK\n");
            exit (EXIT_SUCCESS);            // close child process
        }
        else
        {
            signal(SIGCHLD,SIG_IGN);
        }

        close(new_fd); // parent closes connected socket
    }

    exit(EXIT_SUCCESS);
}
