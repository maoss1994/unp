#include<stdio.h>
#include "unp.h"
#include <time.h>
#include "myerr.h"

main(int argc, char **argv)
{
    int listenfd,connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    time_t ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port   = htons(13);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1){
        err_sys("bind error");
    }

    if (listen(listenfd, LISTENQ) == -1){
        err_sys("listen error");
    }

    for( ; ; ){
        len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (SA *) &cliaddr, &len)) == -1){
            err_sys("accept error");
        } 

        printf("connection from %s, port %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        if (write(connfd, buff, strlen(buff)) < 0){
            err_sys("write error");
        }

        if (close(connfd) < 0){
            err_sys("close error");
        }
    }

}
