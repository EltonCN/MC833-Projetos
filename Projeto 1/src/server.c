#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    printf("Connected MySQL Server");

    if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTIS ServerDB; USE ServerDB;") ||
    mysql_query(conn, "CREATE TABLE IF NOT EXISTIS Users(Mail VARCHAR(25), name VARCHAR(25), surname VARCHAR(50), city VARCHAR(25), course VARCHAR(50), graduationYear INT, skills VARCHAR(100))"))
    {
        fprintf(stderr, "ERROR IN CONNECT DATABASE - %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Connected DataBase ServerDB");

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

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN INSERT USER - %s\n", mysql_error(conn));
        response.code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

Response REMOVER_handler(char where_cause[])
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

Response LIST_USER_handler(char where_cause[])
{
    MYSQL *conn = connectDB();
    Response response;
    char *query;

    response.code = 1;

    if (where_cause == NULL)
        asprintf(&query, "SELECT * FROM Users");
    else
        asprintf(&query, "SELECT * FROM Users WHERE %s", where_cause);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN SELECT USER - %s\n", mysql_error(conn));
        response.code = 0;
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(conn);

        if (result != NULL)
        {
            response.registries.nRegistry = mysql_num_rows(result);

            MYSQL_ROW row;
            Registry registrie;
            int index = 0;

            while ((row = mysql_fetch_row(result)))
            {
                registrie.mail = row[0];
                registrie.name = row[1];
                registrie.surname = row[2];
                registrie.city = row[3];
                registrie.course = row[4];
                registrie.graduationYear = row[5];
                registrie.skills = row[6];

                response.registries.registries[index] = registrie;

                index += 1;
            }
        }

        mysql_free_result(result);
    }

    mysql_close(conn);
    free(query);
    
    return response;
}

Response LIST_BY_COURSE_handler(Request request)
{
    char where_cause[70];
    sprintf(&where_cause, "Course = '%s'", request.body.listByCourseRequest.course);

    return LIST_USER_handler(where_cause);
}

Response LIST_BY_SKILL_handler(Request request)
{
    char where_cause[70];
    sprintf(&where_cause, "Skills like '%s'", request.body.listBySkill.skill);

    return LIST_USER_handler(where_cause);
}

Response LIST_BY_YEAR_handler(Request request)
{
    char where_cause[70];
    sprintf(&where_cause, "GraduationYear = '%i'", request.body.listByYearRequest.graduationYear);

    return LIST_USER_handler(where_cause);
}

Response GET_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(&where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return LIST_USER_handler(where_cause);
}

Response REMOVE_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(&where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return REMOVER_handler(where_cause);
}