#include "common_def.h"

#ifndef REQUESTS_DEF
#define REQUESTS_DEF

/// @brief Data of a REGISTER request (see OperationType for operation definition).
struct RegisterRequest
{
    struct Registry registry;    
};

/// @brief Data of a LIST_BY_COURSE request (see OperationType for operation definition).
struct ListByCourseRequest
{
    char course[50];
};

/// @brief Data of a LIST_BY_SKILL request (see OperationType for operation definition).
struct ListBySkill
{
    char skill[10];
};

/// @brief Data of a LIST_BY_YEAR request (see OperationType for operation definition).
struct ListByYear
{
    int graduationYear;
};


/// @brief Data of a GET_BY_MAIL or REMOVE_BY_MAIL request (see OperationType for operation definition).
struct ByMailRequest
{
    char mail[25];
};

typedef struct RegisterRequest RegisterRequest;
typedef struct ListByCourseRequest ListByCourseRequest;
typedef struct ListBySkill ListBySkill;
typedef struct ListByYear ListByYear;
typedef struct ByMailRequest ByMailRequest; 
typedef struct ImageRequest ImageRequest;

/// @brief Request data. Can be any of the defined requests.
union RequestBody
{
    RegisterRequest registerRequest;
    ListByCourseRequest listByCourseRequest;
    ListBySkill listBySkill;
    ListByYear listByYearRequest;
    ByMailRequest byMailRequest;
    Image imageRequest;
};

typedef union RequestBody RequestBody;

/// @brief Request data. The type defines what will be the data structure used in the body
struct Request
{
    OperationType type;
    RequestBody body;
};

typedef struct Request Request;

#endif