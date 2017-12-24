本次升级包提供1080P 25fps mjpeg流。数据包走TCP协议，端口31003。每帧数据包格式如下：
FRAME_INFO+mjpeg数据

typedef struct _FRAME_INFO
{
	int nWidth;					//mjpeg图片宽度
	int nHeight;				//mjpeg图片高度
	unsigned long frameID;		//帧序列号
	unsigned long dataLen;		//mjpeg数据大小
}FRAME_INFO;

具体网络接收时可以先接收一个FRAME_INFO大小的包，读出mjpeg的数据大小，然后根据mjpeg数据大小接收一帧mjpeg数据。

Dear wen：
         附件中upGradeFile-017.FLS为按你们要求做的升级包，bin.rar为测试客户端，解压执行里面的picoNet.exe填入摄像机ip点击连接即可看到每秒25帧的mjpeg图片。后续再发给你们网络协议的详细描述。
hugh@poya.com.cn


visio2016产品密钥：W9WC2-JN9W2-H4CBV-24QR7-M4HB8