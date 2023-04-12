#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>

#include "requests_def.h"
#include "responses_def.h"

#define DATABASE_IP "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASSWORD "12345678"

MYSQL* connectDB()
{
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, DATABASE_IP, DATABASE_USER, DATABASE_PASSWORD,
            NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Connected MySQL Server\n");

    if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTS ServerDB") ||
    mysql_query(conn, "USE ServerDB") ||
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Users(Mail VARCHAR(25), name VARCHAR(25), surname VARCHAR(50), city VARCHAR(25), course VARCHAR(50), graduationYear INT, skills VARCHAR(100))"))
    {
        fprintf(stderr, "ERROR IN CONNECT DATABASE - %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Connected DataBase ServerDB\n");

    return conn;
}

Response REGISTER_handler(Request request)
{
    MYSQL *conn = connectDB();
    Response response;
    char *query;

    response.code = 1;

    asprintf(&query, "INSERT INTO Users(Mail, Name, Surname, City, Course, GraduationYear, Skills) VALUES ('%s', '%s', '%s', '%s', '%s', %i, '%s')",
        request.body.registerRequest.registry.mail,
        request.body.registerRequest.registry.name,
        request.body.registerRequest.registry.surname,
        request.body.registerRequest.registry.city,
        request.body.registerRequest.registry.course,
        request.body.registerRequest.registry.graduationYear,
        request.body.registerRequest.registry.skills);

    printf(query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN INSERT USER - %s\n", mysql_error(conn));
        response.code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

Response REMOVER_handler(char where_cause[70])
{
    MYSQL *conn = connectDB();
    Response response;
    char *query;

    response.code = 1;

    if (where_cause == NULL)
        asprintf(&query, "DELETE FROM Users");
    else
        asprintf(&query, "DELETE FROM Users WHERE %s", where_cause);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN DELETE USER - %s\n", mysql_error(conn));
        response.code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

Response* LIST_USER_handler(char where_cause[70])
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + (1*sizeof(Registry)));
    char *query;

    if (where_cause == NULL)
        asprintf(&query, "SELECT * FROM Users");
    else
        asprintf(&query, "SELECT * FROM Users WHERE %s", where_cause);

    printf(query);
    printf("\n");

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN SELECT USER - %s\n", mysql_error(conn));
        response->code = 0;
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(conn);

        response->code = 1;
        response->registries.nRegistry = 0;

        if (result != NULL)
        {
            int rows_size = mysql_num_rows(result);

            Response* response2  = malloc(sizeof(Response) + (rows_size*sizeof(Registry)));

            response2->code = 1;

            response2->registries.nRegistry = rows_size;

            MYSQL_ROW row;
            int index = 0;

            while ((row = mysql_fetch_row(result)))
            {
                printf("%s\n\n", row[0]);

                strcpy(response2->registries.registries[index].mail, row[0]);
                strcpy(response2->registries.registries[index].name, row[1]);
                strcpy(response2->registries.registries[index].surname, row[2]);
                strcpy(response2->registries.registries[index].city, row[3]);
                strcpy(response2->registries.registries[index].course, row[4]);
                response2->registries.registries[index].graduationYear = row[5];
                strcpy(response2->registries.registries[index].skills, row[6]);

                index += 1;
            }

            mysql_free_result(result);

            mysql_close(conn);
            free(query);

            // TODO fix memory
            return response2;
        }
    }

    mysql_close(conn);
    free(query);

    return response;

}

Response* LIST_BY_COURSE_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Course = '%s'", request.body.listByCourseRequest.course);

    return LIST_USER_handler(where_cause);
}

Response* LIST_BY_SKILL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Skills like '%s'", request.body.listBySkill.skill);

    return LIST_USER_handler(where_cause);
}

Response* LIST_BY_YEAR_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "GraduationYear = '%i'", request.body.listByYearRequest.graduationYear);

    return LIST_USER_handler(where_cause);
}

Response* GET_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return LIST_USER_handler(where_cause);
}

Response REMOVE_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return REMOVER_handler(where_cause);
}
