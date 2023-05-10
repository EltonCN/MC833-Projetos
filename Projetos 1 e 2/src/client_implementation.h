#include "requests_def.h"
#include "responses_def.h"

#ifndef CLIENT_IMPLEMENTATION
#define CLIENT_IMPLEMENTATION

/// @brief Send request and wait for the response.
/// @param request Request being sent
/// @return Server response for the request. Must be deallocated after use.
Response* sendAndReceive(Request *request);

/// @brief Register the exit function for closing the connection.
void registerExit();

/// @brief Changes the server IP address.
void changeServerIp(char* serverIp);

#endif