#include "common_def.h"

#ifndef RESPONSES_DEF
#define RESPONSES_DEF


/// @brief Response data. The code is 1 for successful operations. The registries is set only for operations that returns data (see OperationType documentation).
struct Response
{
    int code;
    int nRegistry;
    int imageSize;
    unsigned char data[]; //Registries or image data
};

typedef struct Response Response;

#endif