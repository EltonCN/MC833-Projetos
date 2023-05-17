#include "common_def.h"

#ifndef RESPONSES_DEF
#define RESPONSES_DEF

struct RegistryArray
{
    int nRegistry;
    Registry registries[];
};

typedef struct RegistryArray RegistryArray;

union RegistriesOrImage
{
    Image image;
    RegistryArray registries;
};

typedef union RegistriesOrImage RegistriesOrImage;

/// @brief Response data. The code is 1 for successful operations. The registries is set only for operations that returns data (see OperationType documentation).
struct Response
{
    int code;
    OperationType type;
    RegistriesOrImage data;
};

typedef struct Response Response;

#endif