#include "unp.h"
#include "unp_package.h"
#include "myerr.h"

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    if (argc != 2)
        err_quit("Usage : <IPaddress>");
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    printf("connection established\n");
    str_cli(stdin, sockfd);

    exit(0);
    
}

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
//        printf("Fget 客户端已经输入\n");
//        printf("%d,sendline:%s",sockfd,sendline);
        Writen(sockfd, sendline, strlen(sendline));
//        printf("Writen 发送给服务器\n");
        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");
        Fputs(recvline, stdout);
//        printf("服务器写到标准输出\n");
    }
}
