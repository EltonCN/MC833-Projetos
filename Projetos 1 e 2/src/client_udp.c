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

int maxNumber(int a, int b)
{
    if(a>b)
    {
        return a;
    }
    return b;
}

// Public library functions (documentation in header) ---------------------------------

Response* sendAndReceive(Request *request)
{
    openSocket();

    //Send request-----------------------------------------------------------------   
    if(request->type == SEND_IMAGE)
    {
        //Image -> fragment and send

        //Compute nPackage
        int imageSize = request->body.imageRequest.image.image.imageSize;
        int nPackage = ceil(((float) imageSize)/MAX_IMAGE_SIZE_PER_PACKAGE);

        //Basic request
        Request* fragRequest = malloc(sizeof(Request)); 
        fragRequest->body.imageRequest.isFragIfOne = 1;
        fragRequest->type = SEND_IMAGE;

        for(int i = 0; i<nPackage; i++)
        {
            //Create fragment
            ImageFrag* frag = getImageFrag(nPackage, imageSize, MAX_IMAGE_SIZE_PER_PACKAGE, 
                            &(request->body.imageRequest.image.image), 
                            &(request->body.imageRequest.image.image.mail),
                            i);

            //Copy fragment to request
            fragRequest = realloc(fragRequest, sizeof(Request)+(frag->size));
            memcpy(&(fragRequest->body.imageRequest.image.frag), frag, sizeof(ImageFrag)+(frag->size));
            
            //Send
            send(sockfd, (void *) fragRequest, sizeof(Request)+(frag->size), 0);

            free(frag);
        }
        
        free(fragRequest);
    }
    else
    {
        //Normal request send
        send(sockfd, (void *) request, sizeof(Request), 0);
    }

    //Get Response----------------------------------------------------------------------------

    int maxSize = MAX_REGISTRY_PER_PACKAGE*sizeof(Registry);
    maxSize = maxNumber(maxSize, MAX_IMAGE_SIZE_PER_PACKAGE);
    
    int size = sizeof(Response)+maxSize;

    //Receive response
    Response *response = malloc(size);
    int return_code = recv(sockfd, (void *) response, size, MSG_WAITALL);

    //Check for error
    if(return_code < sizeof(Response))
    {
        printf("ERROR RECEIVING RESPONSE. OPERATION MAY HAVE BEEN PERFORMED.\n");

        response->code = 0;
        return response;
    }

    //Receive image if image
    if(response->type == GET_IMAGE_BY_MAIL)
    {
        //Receive image fragments and reconstruct image
        int nPackage = response->data.image.image.frag.nPackages;

        ImageFrag** frags = malloc(sizeof(ImageFrag*)*nPackage);
        frags[0] = &(response->data.image.image.frag);

        Response** responses = malloc(sizeof(Response*)*nPackage);
        responses[0] = response;

        for(int i = 1; i<nPackage; i++)
        {
            response = malloc(size);
            return_code = recv(sockfd, (void *) response, size, MSG_WAITALL);

            frags[i] = &(response->data.image.image.frag);
            responses[i] = response;
        }

        printf("IMAGE SIZE - RECEIVE FRAGMENTE: %d\n", frags[0]->imageSize);

        //Reconstruc image
        RegistryImage *img = restoreImage(frags);
        size = sizeof(RegistryImage)+(frags[0]->imageSize);
        
        //Create response and copy image
        response = malloc(sizeof(Response)+size);
        response->code = 1;
        response->type = GET_IMAGE_BY_MAIL;
        response->data.image.isFragIfOne = 0;
        memcpy(&(response->data.image.image.image), img, size);

        //Free every non-final-response memory
        for(int i = 0; i<nPackage; i++)
        {
            free(responses[i]);
        }
        free(img);
        free(frags);
        free(responses);

        printf("IMAGE SIZE - RECEIVE END: %d\n", response->data.image.image.image.imageSize);
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