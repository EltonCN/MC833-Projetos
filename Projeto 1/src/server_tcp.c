#include <stdio.h>
#include <unistd.h>
#include <mysql.h>

MYSQL connectDB()
{
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, "localhost", "root", "1234",
            NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    return conn
}

int insert()
{
    if (mysql_query(conn, "INSERT INTO Perfil_Table"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
}

void select()
{
    mysql_query(conn, "SELECT email, name from Perfil_Table WHERE curso = ")
}

void processRequest(int sock_fd)
{
    MYSQL *conn = connectDB();



    mysql_close(conn);
}

int main(int argc, char **argv)
{
    int sock_fd, new_fd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    sock_fd= Socket (PF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);
    Bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));
    Listen(sock_fd, LISTENQ);
    
    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        new_fd= Accept(sock_fd, (SA *) &cliaddr, &clilen);

        if ( (childpid = Fork()) == 0) { /* child process */
            Close(sock_fd); /* close listening socket */
            processRequest(new_fd); /* process the request */
            exit (0);
        }

        Close(new_fd); /* parent closes connected socket */
    }

    exit(0);
}