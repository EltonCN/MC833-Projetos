#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>

#include "requests_def.h"
#include "responses_def.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define DATABASE_IP "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASSWORD "12345678"

/// @brief Open connection with database
MYSQL* connectDB()
{
    MYSQL *conn = mysql_init(NULL);

    // Connection with mysql
    if (mysql_real_connect(conn, DATABASE_IP, DATABASE_USER, DATABASE_PASSWORD,
            NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Connected MySQL Server\n");

    // Creates the database and table if they don't exist
    // If they already exist, connect to them.
    if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTS ServerDB") ||
    mysql_query(conn, "USE ServerDB") ||
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Photos(Mail VARCHAR(25), Photo VARCHAR(MAX), PRIMARY KEY (Mail))") ||
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Users(Mail VARCHAR(25), name VARCHAR(25), surname VARCHAR(50), city VARCHAR(25), course VARCHAR(50), graduationYear INT, skills VARCHAR(100), PRIMARY KEY (Mail))"))
    {
        fprintf(stderr, "ERROR IN CONNECT DATABASE - %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Connected DataBase ServerDB\n");

    return conn;
}

/// @brief Add a new user to the database
Response* REGISTER_handler(Request request)
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    response->code = 1;
    response->registries.nRegistry = 0;

    asprintf(&query, "INSERT INTO Users(Mail, Name, Surname, City, Course, GraduationYear, Skills) VALUES ('%s', '%s', '%s', '%s', '%s', %d, '%s')",
        request.body.registerRequest.registry.mail,
        request.body.registerRequest.registry.name,
        request.body.registerRequest.registry.surname,
        request.body.registerRequest.registry.city,
        request.body.registerRequest.registry.course,
        request.body.registerRequest.registry.graduationYear,
        request.body.registerRequest.registry.skills);

    printf("Execute query: %s\n", query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN INSERT USER - %s\n", mysql_error(conn));
        response->code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

/// @brief Add a new photo to the database
Response* INSERT_photo(Request request)
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    response->code = 1;
    response->registries.nRegistry = 0;

    asprintf(&query, "INSERT INTO Photos(Mail, Photo) VALUES ('%s', '%s')",
        request.body.registerRequest.registry.mail,
        request.body.registerRequest.registry.name);

    printf("Execute query: %s\n", query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN INSERT PHOTO - %s\n", mysql_error(conn));
        response->code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

/// @brief Remove photo to the database
Response* REMOVER_photo(char mail[25])
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    response->code = 1;
    response->registries.nRegistry = 0;

    asprintf(&query, "DELETE FROM Photos WHERE Mail = '%s'", mail);

    printf("Execute query: %s\n", query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN DELETE PHOTO - %s\n", mysql_error(conn));
        response->code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

/// @brief Get a photo to the database
Response* GET_photo(char mail[25])
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    asprintf(&query, "SELECT * FROM Users WHERE Mail = '%s'", mail);

    printf("Execute query: %s\n", query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN SELECT PHOTO - %s\n", mysql_error(conn));
        response->code = 0;
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(conn);

        response->code = 1;
        response->registries.nRegistry = 0;

        if (result != NULL && mysql_num_rows(result) > 0)
        {
            Response* response2  = malloc(sizeof(Response) + (rows_size*sizeof(Registry)));

            response2->code = 1;

            response2->registries.nRegistry = rows_size;

            MYSQL_ROW row = mysql_fetch_row(result);

            // TODO: corrigir!
            strcpy(response2->registries.registries[0].mail, row[1]);

            mysql_free_result(result);

            mysql_close(conn);
            free(response);
            free(query);

            return response2;
        }
    }

    mysql_close(conn);
    free(query);

    return response;

}

/// @brief Remove users to the database
Response* REMOVER_handler(char where_cause[70])
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    response->code = 1;
    response->registries.nRegistry = 0;

    if (where_cause == NULL)
        asprintf(&query, "DELETE FROM Users");
    else
        asprintf(&query, "DELETE FROM Users WHERE %s", where_cause);

    printf("Execute query: %s\n", query);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "ERROR IN DELETE USER - %s\n", mysql_error(conn));
        response->code = 0;
    }

    mysql_close(conn);
    free(query);

    return response;
}

/// @brief Get users to the database
Response* LIST_USER_handler(char where_cause[70])
{
    MYSQL *conn = connectDB();
    Response *response = malloc(sizeof(Response) + sizeof(Registry));
    char *query;

    if (where_cause == NULL)
        asprintf(&query, "SELECT * FROM Users");
    else
        asprintf(&query, "SELECT * FROM Users WHERE %s", where_cause);

    printf("Execute query: %s\n", query);

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

            printf("%d rows.\n", rows_size);

            Response* response2  = malloc(sizeof(Response) + (rows_size*sizeof(Registry)));

            response2->code = 1;

            response2->registries.nRegistry = rows_size;

            MYSQL_ROW row;
            int index = 0;

            while ((row = mysql_fetch_row(result)))
            {
                printf("Row %d: %s, %s, %s, %s, %s, %s, %s\n", index, row[0], row[1], row[2], row[3], row[4], row[5], row[6]);

                strcpy(response2->registries.registries[index].mail, row[0]);
                strcpy(response2->registries.registries[index].name, row[1]);
                strcpy(response2->registries.registries[index].surname, row[2]);
                strcpy(response2->registries.registries[index].city, row[3]);
                strcpy(response2->registries.registries[index].course, row[4]);
                response2->registries.registries[index].graduationYear = atoi(row[5]);
                strcpy(response2->registries.registries[index].skills, row[6]);

                index += 1;
            }

            mysql_free_result(result);

            mysql_close(conn);
            free(response);
            free(query);

            return response2;
        }
    }

    mysql_close(conn);
    free(query);

    return response;

}

/// @brief Get user based on course
Response* LIST_BY_COURSE_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Course = '%s'", request.body.listByCourseRequest.course);

    return LIST_USER_handler(where_cause);
}

/// @brief Get user based on skill
Response* LIST_BY_SKILL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Skills like '%c%s%c'", '%', request.body.listBySkill.skill, '%');

    return LIST_USER_handler(where_cause);
}

/// @brief Get user based on graduation year
Response* LIST_BY_YEAR_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "GraduationYear = '%i'", request.body.listByYearRequest.graduationYear);

    return LIST_USER_handler(where_cause);
}

/// @brief Get user based on email
Response* GET_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return LIST_USER_handler(where_cause);
}

/// @brief Remove user based on email
Response* REMOVE_BY_MAIL_handler(Request request)
{
    char where_cause[70];
    sprintf(where_cause, "Mail = '%s'", request.body.byMailRequest.mail);

    return REMOVER_handler(where_cause);
}
