#include "common_def.h"

#ifndef RESPONSES_DEF
#define RESPONSES_DEF

struct ListRegistriesResponse
{
    int nRegistry;
    Registry registries[]; 
};

typedef struct ListRegistriesResponse ListRegistriesResponse;

struct Response
{
    int code;
    ListRegistriesResponse registries;
};

typedef struct Response Response;

#endif