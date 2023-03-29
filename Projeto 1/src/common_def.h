#ifndef COMMON_DEF
#define COMMON_DEF

enum OperationType
{
    REGISTER, 
    LIST_BY_COURSE, 
    LIST_BY_SKILL, 
    LIST_BY_YEAR, 
    LIST_ALL, 
    GET_BY_MAIL, 
    REMOVE_BY_MAIL, 
    GET_IMAGE_BY_MAIL 
};

struct Registry
{
    char mail[25];
    char name[25];
    char surname[50];
    char city[25];
    char course[50];
    int graduationYear;
    char skills[100];
};

typedef enum OperationType OperationType;
typedef struct Registry Registry;

#endif