/*
*Function：Service_InPc.c  任务是在树莓派里面，这里先在pc里面实现。
*Description：这个函数用来捕捉31003端口的mjpeg流。
*     具体网络接收时可以先接收一个FRAME_INFO大小的包，读出mjpeg的数据大小，然后根据mjpeg数据大小接收一帧mjpeg数据。
*	  FRAME_INFO也就是其数据格式
*Name：温怀雄
*Date：2017/12/20
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

/*Pony提供的端口为31003*/
#define PORT 31003

#define BUFFER_SIZE	1024	//buffer
#define MAX_QUE_CONN_NM	5	//listen的backlog：请求队列中允许的最大请求数，大多数系统缺省值为5

/*数据包格式*/
typedef struct _FRAME_INFO
{
	int nWidth;   			//mjpeg图片宽度
	int nHeight;  			//mjpeg图片高度
	unsigned long frameID;	//帧序列号
	unsigned long dateLen;	//mjpeg数据大小
};

int main(int argc, char const *argv[])
{
	struct sockaddr_in server_sockaddr,client_sockaddr;
	int sin_size,recvbytes;
	int sockfd,client_fd;
	char buf[BUFFER_SIZE];
	sin_size = sizeof(struct sockaddr_in);

	/*1：建立socket连接*/
	if(sockfd = socket(AF_INET,SOCK_STREAM,0) < 0)
	{
		perror("socket error!");
		exit(-1);
	}
	printf("sockfd id = %d\n",sockfd);

	/*1.1：设置sockaddr_in结构体中相关参数*/
	server_sockaddr.sin_family = AF_INET;			/*地址族*/
	server_sockaddr.sin_port = htons(PORT);			/*端口号*/
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;	/*IP地址*/
	bzero(&(server_sockaddr.sin_zero), 8);			/*填充0以保持与struct sockaddr同样大小*/

	int i = 1;  /*允许重复使用本地地址与套接字进行绑定*/
				/*关于setsockpot：http://blog.csdn.net/chary8088/article/details/2486377*/
	setsockpot(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

	/*1.2：绑定blind*/
	if(blind(sockfd,(struct sockaddr *)&server_sockaddr,
		sizeof(struct sockaddr)) == -1)
	{
		perror("error in blind!");
		exit(1);
	}
	printf("Blind success!\n");

	/*1.3：调用listen()函数，创建未处理请求的队列*/
	if(listen(sockfd,MAX_QUE_CONN_NM) == -1)
	{
		perror("error in listen!");
		exit(1);
	}
	printf("Listening...\n");

	/*1.4：调用accept()函数接收客户端的请求*/
	memset(buf,0,sizeof(buf));  /*buf清零*/

	if(client_fd = accept(sockfd,
		(struct sockaddr *)&client_sockaddr,&sin_size) == -1)
	{
		perror("error in accept!");
		exit(1);
	}

	/*1.5：调用recv()函数接收客户端的信息，
	也就是接收图片的信息，
	并且存入到特定的路径
	根据Pony的提示，这里先接收一帧的数据，得到数据大小。
	*/
	

	/*任务完成，关闭fd！*/
	close(sockfd);

	return 0;
}
