#include <stdio.h>
#include <stdlib.h>

#include "requests_def.h"
#include "responses_def.h"
#include "client_implementation.h"

void printDefaultResponse(Response* response)
{
    if(response->code == 1)
    {
        printf("Operation done!\n");
    }
    else
    {
        printf("Error :/ ...\n");
    }
}

void printListRegistriesResponse(Response* response)
{
    if(response->code != 1)
    {
        printf("Error :/ ...\n");
        return;
    }

    int nRegistry = response->registries.nRegistry;
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

void REGISTER_handler()
{
    Request request;
    request.type = REGISTER;

    printf("Enter the registry fields:\n");
    
    printf("mail (max 24 char): ");
    scanf("%s", &request.body.registerRequest.registry.mail);
    printf("\n");

    printf("name (max 24 char): ");
    scanf("%s", &request.body.registerRequest.registry.name);
    printf("\n");

    printf("surname (max 49 char): ");
    scanf("%s", &request.body.registerRequest.registry.surname);
    printf("\n");

    printf("city (max 24 char): ");
    scanf("%s", &request.body.registerRequest.registry.city);
    printf("\n");

    printf("course (max 49 char): ");
    scanf("%s", &request.body.registerRequest.registry.course);
    printf("\n");

    printf("graduation year: ");
    scanf("%d", &request.body.registerRequest.registry.graduationYear);
    printf("\n");

    int nSkills;
    printf("number of skills: ");
    scanf("%d", &nSkills);
    printf("\n");

    request.body.registerRequest.registry.skills[0] = "\0";

    int index = 0;
    for(int i = 0; i<nSkills; i++)
    {
        char skill[10];
        printf("Skill %d (max 9 char): ", i);
        scanf("%s", &skill);

        for(int j = 0; j<9; j++)
        {
            request.body.registerRequest.registry.skills[index] = skill[j];
            index += 1;
        }
        request.body.registerRequest.registry.skills[index] = ";";
        index += 1;
    }
    request.body.registerRequest.registry.skills[index-1] = "\0";

    Response* response = sendAndReceive(request);

    printDefaultResponse(response);
    free(response);
}

void LIST_BY_COURSE_handler()
{
    Request request;
    request.type = LIST_BY_COURSE;

    printf("Enter course (max 49 char): ");
    scanf("%s", &request.body.listByCourseRequest.course);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

void LIST_BY_SKILL_handler()
{
    Request request;
    request.type = LIST_BY_SKILL;

    printf("Enter skill (max 9 char): ");
    scanf("%s", &request.body.listBySkill.skill);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

void LIST_BY_YEAR_handler()
{
    Request request;
    request.type = LIST_BY_YEAR;
    
    printf("Enter course (max 49 char): ");
    scanf("%d", &request.body.listByYearRequest.graduationYear);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

void LIST_ALL_handler()
{
    Request request;
    request.type = LIST_ALL;
    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

void GET_BY_MAIL_handler()
{
    Request request;
    request.type = GET_BY_MAIL;
    
    printf("Enter mail (max 24 char): ");
    scanf("%s", &request.body.byMailRequest.mail);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

void REMOVE_BY_MAIL_handler()
{
    Request request;
    request.type = REMOVE_BY_MAIL;
    
    printf("Enter mail (max 24 char): ");
    scanf("%s", &request.body.byMailRequest.mail);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printDefaultResponse(response);
    free(response);
}

void exit_handler()
{
    printf("Bye!");
    closeSocket();
    exit(EXIT_SUCCESS);
}


void (*handlers[])() = {
                        exit_handler,
                        REGISTER_handler, 
                        LIST_BY_COURSE_handler, 
                        LIST_BY_SKILL_handler, 
                        LIST_BY_YEAR_handler, 
                        LIST_ALL_handler, 
                        GET_BY_MAIL_handler, 
                        REMOVE_BY_MAIL_handler,
                        };

int main()
{

    int option;
    
    while(1)
    {
        printf("Choose operation:\n");
        printf("1 Create new registry\n");
        printf("2 List registries by course\n");
        printf("3 List registries by skill\n");
        printf("4 List registries by year\n");
        printf("5 List all registries\n");
        printf("6 Get registry by mail\n");
        printf("7 Remove registry by mail\n");
        printf("0 Exit\n");
    
        
        scanf("%d", &option);

        if(option <=7 && option >= 0)
        {
            handlers[option]();
        }
    }
}