#include <stdio.h>
#include <stdlib.h>

#include "requests_def.h"
#include "responses_def.h"

void REGISTER_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void LIST_BY_COURSE_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void LIST_BY_SKILL_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void LIST_BY_YEAR_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void LIST_ALL_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void GET_BY_MAIL_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void REMOVE_BY_MAIL_handler()
{
    Request request;
    request.type = ;
    Response = send(request);
    
}

void exit_handler()
{
    printf("Bye!");
    exit(EXIT_SUCCESS);
}


void (*handlers[])() = {
                        REGISTER_handler, 
                        LIST_BY_COURSE_handler, 
                        LIST_BY_SKILL_handler, 
                        LIST_BY_YEAR_handler, 
                        LIST_ALL_handler, 
                        GET_BY_MAIL_handler, 
                        REMOVE_BY_MAIL_handler,
                        exit_handler,
                        };

int main()
{
    printf("Choose operation:\n");

    int option;

    while(1)
    {
        printf("1 Create new registry\n");
        printf("2 List registries by course\n");
        printf("3 List registries by skill\n");
        printf("4 List registries by year\n");
        printf("5 List all registries\n");
        printf("6 Get registry by mail\n");
        printf("7 Remove registry by mail\n");
        printf("0 Exit");
    
        
        scanf("%d", &option);

        if(option <=7 && option >= 0)
        {
            break;
        }
    }


}