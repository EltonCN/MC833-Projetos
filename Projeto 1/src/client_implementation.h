#include "requests_def.h"
#include "responses_def.h"

#ifndef CLIENT_IMPLEMENTATION
#define CLIENT_IMPLEMENTATION

Response sendAndReceive(Request request);
void closeSocket();

#endif