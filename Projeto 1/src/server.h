#include "requests_def.h"
#include "responses_def.h"

#ifndef SERVER
#define SERVER

Response* REGISTER_handler(Request request);
Response REMOVER_handler(char where_cause[70]);
Response* LIST_USER_handler(char where_cause[70]);
Response* LIST_BY_COURSE_handler(Request request);
Response* LIST_BY_SKILL_handler(Request request);
Response* LIST_BY_YEAR_handler(Request request);
Response GET_BY_MAIL_handler(Request request);
Response REMOVE_BY_MAIL_handler(Request request);

#endif
