#include "common_def.h"

#ifndef RESPONSES_DEF
#define RESPONSEs_DEF

struct Response
{
    int code;
};

struct ListRegistriesResponse
{
    int nRegistry;
    Registry registries[]; 
};

typedef struct Response Response;
typedef struct ListRegistriesResponse ListRegistriesResponse;

#endif