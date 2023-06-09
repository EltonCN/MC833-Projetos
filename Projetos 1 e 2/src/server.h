#include "requests_def.h"
#include "responses_def.h"
#include "common_def.h"

#ifndef SERVER
#define SERVER

Response* REGISTER_handler(Request* request);
Response* INSERT_photo(Request* request);
Response* REMOVER_handler(char where_cause[70]);
Response* LIST_USER_handler(char where_cause[70]);
Response* LIST_BY_COURSE_handler(Request* request);
Response* LIST_BY_SKILL_handler(Request* request);
Response* LIST_BY_YEAR_handler(Request* request);
Response* GET_BY_MAIL_handler(Request* request);
FragList* GET_photo(char mail[25]);
Response* REMOVE_BY_MAIL_handler(Request* request);

#endif
