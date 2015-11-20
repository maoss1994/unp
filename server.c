#include "unp.h"
#include "myerr.h"

main(int argc, char *argv[])
{
    int    listenfd,connfd;
    struct sockaddr_in servaddr;
    char   buff[MAXLINE];
    time_t ticks;
    
    if ((listenfd = socket(AF_INET,SOCK_STREAM, 0))== -1){
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1){
        err_sys("bind error");
    }
    
    if (listen(listenfd, LISTENQ) < 0){
        err_sys("listen error");
    }

    for ( ; ; ){
        if ((connfd = accept(listenfd, (SA * ) NULL, NULL)) == -1){
            err_sys("connect error");
        }

        ticks = time(NULL);
        snprintf(buff,sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if (write(connfd, buff, strlen(buff)) < 0){
            err_sys("write error");
        }

        if(close(connfd) < 0){
            err_sys("close error");
        }
    }
}
