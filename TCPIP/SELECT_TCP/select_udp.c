#include <stdio.h>
#include <sys/types.h>	
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 #include <strings.h>
#include <unistd.h>
#include <errno.h>

#include <sys/select.h>

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
	
	fd_set readtable;
	int ret;

	while(1)
	{
	FD_ZERO(&readtable);
	FD_SET(sockfd,&readtable);
	FD_SET(0,&readtable);

	if((ret=select(sockfd+1,&readtable,NULL,NULL,NULL))<0)
	{
		perror("fail to select");
		return -1;
	
	}

	if(ret>0)
	{
		if(FD_ISSET(0,&readtable)>0)
		{
			fgets(buf,128,stdin);
			printf("%s\n",buf);
		
		}

		if(FD_ISSET(sockfd,&readtable)>0)
		{
			bzero(buf,128);
			recvfrom(sockfd,buf,128,0,(struct sockaddr *)&peer_addr,&len);
			printf("%s\n",buf);
		
		}

	
	
	
	}
	}

	return 0;
}
