#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "requests_def.h"
#include "responses_def.h"
#include "server.h"

#define PORT 54321
#define SERVER_IP "127.0.0.1"
#define SA struct sockaddr
#define LISTENQ 5

int sockfd = -1;

void openSocket()
{
    if(sockfd == -1)
    {
        struct sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(PORT);
        serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if ((bind(sockfd, (SA*)&serveraddr, sizeof(serveraddr))) != 0)
        {
            printf("SOCKET BIND FAILED!\n");
            exit(EXIT_FAILURE);
        }

        if ((listen(sockfd, LISTENQ)) != 0)
        {
            printf("LISTEN FAILED!\n");
            exit(EXIT_FAILURE);
        }

        printf("OPEN SOCKET\n");
    }

    if(sockfd == -1)
    {
        printf("ERROR OPENING THE SOCKET. CAN'T DO NOTHING, SORRY.\n");
        exit(EXIT_FAILURE);
    }
}

void processRequest(int client_fd)
{
    int recv_response;
    Request request;
    Response* response;

    do
    {
        recv_response = recv(client_fd, (void *) &request, sizeof(Request), MSG_WAITALL);

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

        printf("Response - code: %d, nRegistry: %d", response->code, response->registries.nRegistry);

        send(client_fd, (void *) response, sizeof(Response) + (response->registries.nRegistry*sizeof(Registry)), 0);

        free(response);
    } while (recv_response != 0 && recv_response != -1);
}

void Test_BO(int argc, char **argv)
{
    printf("Compilado\n");

    /*Request request;
    request.type = REGISTER;

    sprintf(request.body.registerRequest.registry.mail, "test@gmail.com");
    printf(request.body.registerRequest.registry.mail);
    printf("\n");

    sprintf(request.body.registerRequest.registry.name, "name test");
    printf(request.body.registerRequest.registry.name);
    printf("\n");

    sprintf(request.body.registerRequest.registry.surname, "surname test");
    printf(request.body.registerRequest.registry.surname);
    printf("\n");

    sprintf(request.body.registerRequest.registry.city, "Campinas");
    printf(request.body.registerRequest.registry.city);
    printf("\n");

    sprintf(request.body.registerRequest.registry.course, "Eng. Computacao");
    printf(request.body.registerRequest.registry.course);
    printf("\n");

    request.body.registerRequest.registry.graduationYear = 2019;
    printf("%d", request.body.registerRequest.registry.graduationYear);
    printf("\n");

    sprintf(request.body.registerRequest.registry.skills, "programador");
    printf(request.body.registerRequest.registry.skills);*/
    printf("\n");

    Response* response;
    //response = REGISTER_handler(request);

    printf("Register responce code: %d\n", response->code);
    response = LIST_USER_handler(NULL);

    int nRegistry = response->registries.nRegistry;

    printf("%d", nRegistry);
    for(int i = 0; i<nRegistry; i++)
    {
        printf("Mail: %s\n", response->registries.registries[i].mail);
        printf("Name: %s\n", response->registries.registries[i].name);
        printf("Surname: %s\n", response->registries.registries[i].surname);
        printf("City: %s\n", response->registries.registries[i].city);
        printf("Course: %s\n", response->registries.registries[i].course);
        printf("Graduation year: %d\n", response->registries.registries[i].graduationYear);
        printf("Skills: %s\n", response->registries.registries[i].city);
        printf("\n");
    }
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
        new_fd= accept(sockfd, (SA *) &cliaddr, &clilen);

        if ( (childpid = fork()) == 0) {    // child process
            printf("FORK\n");
            close(sockfd);                  // close listening socket
            processRequest(new_fd);         // process the request
            exit (EXIT_SUCCESS);
        }

        close(new_fd); // parent closes connected socket
    }

    exit(EXIT_SUCCESS);
}
