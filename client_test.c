/*
*Function：Service_InPc.c  任务是在树莓派里面，这里先在pc里面实现。
*Description：这个函数用来捕捉31003端口的mjpeg流。
*     具体网络接收时可以先接收一个FRAME_INFO大小的包，读出mjpeg的数据大小，然后根据mjpeg数据大小接收一帧mjpeg数据。
*	  FRAME_INFO也就是其数据格式
*Name：温怀雄
*Date：2017/12/20
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> /* 提供类型 pid_t 的定义 */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*Pony提供的端口为31003*/
#define PORT 31003

#define BUFFER_SIZE	1024	//buffer
#define BUFFER_PIC_SIZE 102400 //buffer for picture

#define DEST_FILE_NAME "./first.jpg"

typedef struct _FRAME_INFO
	{
		int nWidth;					//mjpeg图片宽度
		int nHeight;				//mjpeg图片高度
		unsigned long frameID;		//帧序列号
		unsigned long dataLen;		//mjpeg数据大小
	}FRAME_INFO;



int main(int argc, char *argv[])
{
	int sockfd,recvbytes;
	int buf[4];
	char buf_pic[BUFFER_PIC_SIZE];
	struct hostent *host;
	struct sockaddr_in serv_addr;
	FRAME_INFO frame1_info;

	int dest_file;
	int i;		//局部变量

	if(argc < 2)
	{
		fprintf(stderr,"usage: ./client Hostname(or ip address)\n");
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
	printf("socked created success!\n");  //测试

	/*打开第一个存储文件*/
	if((dest_file = open(DEST_FILE_NAME,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
	{
		printf("open jpg file error\n");
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
	printf("connected success!\n");  //测试

	/*接收一个FRAME_INFO的大小,读出mjpeg的数据大小,根据mjpeg数据大小接收一帧mjpeg数据*/

	if(recvbytes = recv(sockfd,buf,BUFFER_SIZE,0) < 0)
	{
		printf("recv error!\n");
		exit(1);
	}
	printf("recvbytes = %d\n",recvbytes);

	/*输出整形数组*/
	/*for(i=0;i<=BUFFER_SIZE-1;i++)
	{
		printf("%d\n",buf[i]);
	}
	*/

	/*把这个数组的平均四位分给frame1_info里面的成员*/
	frame1_info.nWidth = buf[0];
	frame1_info.nHeight = buf[1];
	frame1_info.frameID = (unsigned long)buf[2];
	frame1_info.dataLen = (unsigned long)buf[3];

	/*输出*/
	printf("frame1_info.nWidth = %d \nframe1_info.nHeight = %d\nframe1_info.frameID = %lu \nframe1_info.dataLen = %lu \n",buf[0],buf[1],buf[2],buf[3]);

	/*接受接下来frame1_info.datelen个字节的数据，并且放到first.jpg里面去*/
	if((recvbytes = recv(sockfd,buf_pic,frame1_info.dataLen,0)) == -1)
	{
		printf("error in recv datalen!!");
		exit(-1);				
	}
	
	int buf_data;
	if((buf_data = write(dest_file,buf_pic,frame1_info.dataLen)) == -1)
	{
		printf("error in write data to dest_file!!");	
	}
	
		
	printf("the end!\n");	//测试

	close(sockfd);
	exit(0);
}
