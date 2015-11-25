#include "unp.h"
#include "myerr.h"
#include "operate.h"
#include "unp_package.h"

int main(int argc, char **argv)
{
    int       listenfd, connfd;
    pid_t     childpid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port   = htons(SERV_PORT);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        err_sys("bind error");
    }

    if (listen(listenfd, LISTENQ) == -1) {
        err_sys("listen error");
    }

    for( ; ; ) {
        clilen = sizeof(cliaddr);
        if (connfd = accept (listenfd, (SA *) &cliaddr, &clilen) == -1) {
            err_sys("accept error");
        }
//        if ((childpid = fork() < 0)) {
//            err_sys("fork error");
//       } else if (childpid == 0) {

        if ((childpid = Fork()) == 0) {
            Close(listenfd);
            printf("close the child listenfd\n");
            str_echo(connfd);
            exit(0);
        }
        printf("close the connfd\n");
        Close(connfd);

    }
}

void str_echo(int sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];

//    printf("server begin writen\n");
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
    {
//        printf("server begin writen read\n");
        Writen(sockfd, buf, n);
          
    } 

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        err_sys("str_echo: read error");
}
