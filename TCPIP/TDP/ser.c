#include<stdio.h>
#include <sys/types.h>	       
#include <sys/socket.h>
       #include <strings.h>
       #include <arpa/inet.h>
       #include <netinet/in.h>
       #include <arpa/inet.h>

int main(int argc, const char *argv[])
{
	int sockfd;
	int confd;

	char buf[128];
	bzero (buf,128);
	struct sockaddr_in seraddr;
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(50050);
	seraddr.sin_addr.s_addr=inet_addr("0.0.0.0");

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		return -1;

	}

	if(bind(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0)
	{
		perror("bind");
		return -1;
	}

	listen(sockfd,5);
	int ret;
	while(1)
	{
		if(	(confd=accept(sockfd,NULL,NULL))<0)
		{
			perror("confd");
			return -1;
		}

		while(1)
		{
			ret= recv(confd,buf,128,0);
			if(0==ret)//recv==0说明对方已经关闭；
			{
				break;
			}
			if(0>ret)
			{
				close(confd);
				close(sockfd);
				return -1;
			}
				printf("%s\n",buf);
		}
		close (confd);
		bzero(buf,128);
		return 0;
	}

}
