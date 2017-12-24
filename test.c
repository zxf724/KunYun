/*client.c*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 31003
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
int sockfd,sendbytes;
char buf[BUFFER_SIZE];
struct hostent *host;
struct sockaddr_in serv_addr;

if(argc < 3)
{
fprintf(stderr,"USAGE: ./client Hostname(or ip address)\n");
exit(1);
}

/*地址解析函数*/
if ((host = gethostbyname(argv[1])) == NULL)
{
perror("gethostbyname");
exit(1);
}

memset(buf, 0, sizeof(buf));

/*创建 socket*/
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
perror("socket");
exit(1);
}

/*设置 sockaddr_in 结构体中相关参数*/
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
bzero(&(serv_addr.sin_zero), 8);

/*调用 connect 函数主动发起对服务器端的连接*/
if(connect(sockfd,(struct sockaddr *)&serv_addr,
sizeof(struct sockaddr))== -1)
{
perror("connect");
exit(1);
}

/*发送消息给服务器端*/
if ((sendbytes = send(sockfd, buf, strlen(buf), 0)) == -1)
{
perror("send");
exit(1);
}
close(sockfd);
exit(0);
}