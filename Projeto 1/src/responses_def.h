#include "common_def.h"

#ifndef RESPONSES_DEF
#define RESPONSES_DEF

/**
 * @brief Response data for requests that returns data (see OperationType documentation).
 *
 * Use the nRegistry for reading the registries vector. 
 */
struct ListRegistriesResponse
{
    int nRegistry;
    Registry registries[]; 
};

typedef struct ListRegistriesResponse ListRegistriesResponse;

/// @brief Response data. The code is 1 for successful operations. The registries is set only for operations that returns data (see OperationType documentation).
struct Response
{
    int code;
    ListRegistriesResponse registries;
};

typedef struct Response Response;

#endif