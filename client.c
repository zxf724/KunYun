/*
*Function：Service_InPc.c  任务是在树莓派里面，这里先在pc里面实现。
*Description：这个函数用来捕捉31003端口的mjpeg流。
*具体网络接收时可以先接收一个FRAME_INFO大小的包，读出mjpeg的数据大小，然后根据mjpeg数据大小接收一帧mjpeg数据。
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

/*系统调用*/
#include <sys/types.h> /* 提供类型 pid_t 的定义 */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*Pony提供的端口为31003*/
#define PORT 31003
#define BUFFER_PIC_SIZE (1024*100)  //buffer for picture
#define FILE_NAME(i) (i)
#define TALL_NUM	(1024*1024)


typedef struct _FRAME_INFO
	{
		int nWidth;			//mjpeg图片宽度
		int nHeight;			//mjpeg图片高度
		unsigned long frameID;		//帧序列号
		unsigned long dataLen;		//mjpeg数据大小
	}FRAME_INFO;

int main(int argc, char *argv[])
{
	int sockfd;
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
	printf("socked created successed!\n");  //测试

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
	printf("connected successed!\n");  //测试

/*接收一个FRAME_INFO的大小,读出mjpeg的数据大小,根据mjpeg数据大小接收一帧mjpeg数据*/

	int recvbytes;
	int recvbytes_1;
	int buf_data;
	int j;	

	FILE *fp;
		
	for(j=1;j<=TALL_NUM;j++)
	{
		if((recvbytes = recv(sockfd,buf,sizeof(buf),0)) == -1)
		{
			printf("recv error!\n");
			exit(1);
		}
		printf("recvbytes = %d\n",recvbytes);

		/*把这个数组的平均四位分给frame1_info里面的成员*/
		frame1_info.nWidth = buf[0];
		frame1_info.nHeight = buf[1];
		frame1_info.frameID = (unsigned long)buf[2];
		frame1_info.dataLen = (unsigned long)buf[3];
	
		/*输出*/
		printf("frame1_info.nWidth = %d \nframe1_info.nHeight = %d\nframe1_info.frameID = %lu \nframe1_info.dataLen = %lu \n",buf[0],buf[1],buf[2],buf[3]);

		char file_num[16];
		int dest_file;
		int revcbytes;		

		sprintf(file_num,"./picture/NUM_%d.jpeg",j);

		if((dest_file = open(FILE_NAME(file_num),O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
		{
			printf("open jpg file error\n");
		}


	/*接受接下来frame1_info.datelen个字节的数据，并且放到first.jpg里面去*/
	if((recv(sockfd,buf_pic,frame1_info.dataLen,MSG_WAITALL)) == -1)
		{
			printf("error in recv dataLen!!");
		}

		if((buf_data = write(dest_file,buf_pic,frame1_info.dataLen)) == -1)
		{
			printf("error in recv datalen");
		}

	printf("the end!\n");	//测试
	close(dest_file);
	} //end of while

	close(sockfd);
	exit(0);
}
