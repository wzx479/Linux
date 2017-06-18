#include <stdio.h>
#include <sys/types.h>	
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 #include <strings.h>


int main(int argc, const char *argv[])
{
	int sockfd;

	char buf[128];
	bzero(buf,128);

	struct sockaddr_in addr,peer_addr;
	addr.sin_family=AF_INET;//填充ipv4域
	addr.sin_port=htons(50000);//填充端口号，需要转换大小端
	//htons   host to  network short  主机转网络字节序 短类型
	addr.sin_addr.s_addr=inet_addr("0.0.0.0");//填充地址
	//u32 类型的ip地址   inet_addr -》 字符串转换成 网络字节序地址 
	//0.0.0.0代表本机任意地址
	bzero(&peer_addr,sizeof(peer_addr));//初始化结构体，清零
	int len =sizeof(peer_addr);
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)//（ipv4域，数据报，协议）
	{
		//socket 创建文件描述符实现网络通信
		//sockfd为 -1出错
		perror("socket:");
		return -1;
	}
	
	if(bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))<0)
	{
		//bind（1.套接字 2.地址结构体指针 3.结构体长度）
		//为了让套接字绑定本机地址和指定端口号
		//sockaddr为通用地址结构体,实际使用sockaddr_in
		//实际使用sockaddr_in
		//传参需要强制转换，为了避免编译错误
		perror("bind:");
		return -1;
	
	}

	while(1)
	{
		recvfrom(sockfd,buf,128,0,(struct sockaddr*)&peer_addr,&len);
		//本函数用来接收消息
		//1. 套接字，2. 用来作为存储接收的缓存，3.大小，4.flag 默认为零
		//5. 用来放置发送端的地址结构体指针，6.指定此结构体长度
		printf("%s:%d,%s\n",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port),buf);
		//inet_ntoa network to ascii 网络地址转换成字符串  
		//ntohs network to host short  网络字节序转换成 本机字节序 短  
		//buf  打印输出
		//
		//
		//
		//sendto(sockfd,buf,128,0,(struct sockaddr *)&peer_addr,len);
		char sentbuf[128];
		bzero(sentbuf,128);
		sprintf(sentbuf,"%s:%d,%s\n",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port),buf);
		int i;
		for(i=2;i<255;i++)
		{
			peer_addr.sin_port=htons(50001);
			peer_addr.sin_addr.s_addr=htonl((192<<24)+(168<<16)+(1<<8)+i);
			sendto(sockfd,sentbuf,128,0,(struct sockaddr *)&peer_addr,len);
		
		}
		bzero(buf,128);  //清零 buf
	}//循环实现  接收数据，打印，清空buf  继续下一次操作
	return 0;
}
