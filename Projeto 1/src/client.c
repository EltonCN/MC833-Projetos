#include <stdio.h>
#include <stdlib.h>

#include "requests_def.h"
#include "responses_def.h"
#include "client_implementation.h"

/// @brief Print the response for operations that don't receive any data (see OperationType documentation).
/// @param response Server response.
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

/// @brief Print the response for operations that receive a list of registries (see OperationType documentation).
/// @param response 
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
        printf("Skills: %s\n", response->registries.registries[i].skills);
        printf("\n");
    }
}

/// @brief Handler for creating and sending a REGISTER request and showing the response.
void REGISTER_handler()
{
    Request request;
    request.type = REGISTER;

    char temp;

    printf("Enter the registry fields:\n");
    
    printf("mail (max 24 char): ");
    scanf("%s", &request.body.registerRequest.registry.mail);
    printf("\n");

    printf("name (max 24 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.registerRequest.registry.name);
    printf("\n");

    printf("surname (max 49 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.registerRequest.registry.surname);
    printf("\n");

    printf("city (max 24 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.registerRequest.registry.city);
    printf("\n");

    printf("course (max 49 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.registerRequest.registry.course);
    printf("\n");

    printf("graduation year: ");
    scanf("%d", &request.body.registerRequest.registry.graduationYear);
    printf("\n");

    int nSkills;
    printf("number of skills: ");
    scanf("%d", &nSkills);
    printf("\n");

    request.body.registerRequest.registry.skills[0] = '\0';

    int index = 0;
    for(int i = 0; i<nSkills; i++)
    {
        char skill[10];
        printf("Skill %d (max 9 char): ", i);
        scanf("%c",&temp);
        scanf("%[^\n]", &skill);

        for(int j = 0; j<9; j++)
        {
            if(skill[j] == NULL || skill[j] == '\0')
            {
                break;
            }

            request.body.registerRequest.registry.skills[index] = skill[j];
            index += 1;
        }
        request.body.registerRequest.registry.skills[index] = ';';
        index += 1;
    }
    if(index != 0)
    {
        request.body.registerRequest.registry.skills[index-1] = '\0';
    }

    //Debug prints. Please ignore.
    //printf("Name: %s\n", request.body.registerRequest.registry.name);
    //printf("Mail: %s\n", request.body.registerRequest.registry.mail);
    //printf("Surname: %s\n", request.body.registerRequest.registry.surname);
    //printf("City: %s\n", request.body.registerRequest.registry.city);
    //printf("Course: %s\n", request.body.registerRequest.registry.course);
    //printf("Graduation year: %d\n", request.body.registerRequest.registry.graduationYear);
    //printf("Skills: %s\n", request.body.registerRequest.registry.skills);
    //printf("\n");

    Response* response = sendAndReceive(request);

    printDefaultResponse(response);
    free(response);
}

/// @brief Handler for creating and sending a LIST_BY_COURSE request and showing the response.
void LIST_BY_COURSE_handler()
{
    Request request;
    request.type = LIST_BY_COURSE;
    
    char temp;
    printf("Enter course (max 49 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.listByCourseRequest.course);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

/// @brief Handler for creating and sending a LIST_BY_SKYLL request and showing the response.
void LIST_BY_SKILL_handler()
{
    Request request;
    request.type = LIST_BY_SKILL;

    char temp;
    printf("Enter skill (max 9 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.listBySkill.skill);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

/// @brief Handler for creating and sending a LIST_BY_YEAR request and showing the response.
void LIST_BY_YEAR_handler()
{
    Request request;
    request.type = LIST_BY_YEAR;
    
    char temp;
    printf("Enter course (max 49 char): ");
    scanf("%c",&temp);
    scanf("%[^\n]", &request.body.listByYearRequest.graduationYear);
    printf("\n");

    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

/// @brief Handler for creating and sending a LIST_ALL request and showing the response.
void LIST_ALL_handler()
{
    Request request;
    request.type = LIST_ALL;
    Response* response = sendAndReceive(request);
    
    printListRegistriesResponse(response);
    free(response);
}

/// @brief Handler for creating and sending a GET_BY_MAIL request and showing the response.
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

/// @brief Handler for creating and sending a REMOVE_BY_MAIL request and showing the response.
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

void changeServerIp_handler()
{
    char* serverIp = malloc(17*sizeof(char));
    printf("Enters server IP (with dots): ");
    scanf("%s", serverIp);

    changeServerIp(serverIp);
}

/// @brief Handler for the exit operation.
void exit_handler()
{
    printf("Bye!");
    exit(EXIT_SUCCESS);
}

// Registers all the handlers (in the same order as expected by the menu).
void (*handlers[])() = {
                        exit_handler,
                        REGISTER_handler, 
                        LIST_BY_COURSE_handler, 
                        LIST_BY_SKILL_handler, 
                        LIST_BY_YEAR_handler, 
                        LIST_ALL_handler, 
                        GET_BY_MAIL_handler, 
                        REMOVE_BY_MAIL_handler,
                        changeServerIp_handler,
                        };

int main()
{
    //Register the exit function (closes the socket in the exit).
    registerExit();
    
    int option;
    
    while(1)
    {
        // Menu
        printf("Choose operation:\n");
        printf("1 Create new registry\n");
        printf("2 List registries by course\n");
        printf("3 List registries by skill\n");
        printf("4 List registries by year\n");
        printf("5 List all registries\n");
        printf("6 Get registry by mail\n");
        printf("7 Remove registry by mail\n");
        printf("8 Change server IP (default is local host)\n");
        printf("0 Exit\n");
        
        scanf("%d", &option);

        // Call the operation handler.
        if(option <=8 && option >= 0)
        {
            handlers[option]();
        }
    }
}