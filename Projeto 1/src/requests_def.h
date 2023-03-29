#include "common_def.h"

#ifndef REQUESTS_DEF
#define REQUESTS_DEF


struct RegisterRequest
{
    struct Registry registry;    
};

struct ListByCourseRequest
{
    char course[50];
};

struct ListBySkill
{
    char skill[10];
};

struct ListByYear
{
    int graduationYear;
};

struct ByMailRequest
{
    char mail[25];
};

typedef struct RegisterRequest RegisterRequest;
typedef struct ListByCourseRequest ListByCourseRequest;
typedef struct ListBySkill ListBySkill;
typedef struct ListByYear ListByYear;
typedef struct ByMailRequest ByMailRequest; 

union RequestBody
{
    RegisterRequest registerRequest;
    ListByCourseRequest listByCourseRequest;
    ListBySkill listBySkill;
    ListByYear listByYearRequest;
    ByMailRequest byMailRequest;
};

typedef union RequestBody RequestBody;

struct Request
{
    OperationType type;
    RequestBody body;
};

typedef struct Request Request;

#endif