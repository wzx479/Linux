#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>	
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
  #include <sys/select.h>
#define SA struct sockaddr
int main(int argc, const char *argv[])
{
	int sockfd,confd;
	struct sockaddr_in seraddr;
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(50000);
	seraddr.sin_addr.s_addr=inet_addr("0.0.0.0");


	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("fail to socket");
		return -1;
	
	}
	if(bind(sockfd,(SA *)&seraddr,sizeof(seraddr))<0)
	{
		perror("fail to bind");
		return -1;
	}

	listen(sockfd,5);
	char buf[128];
	bzero(buf,128);
	
	fd_set global,current;

	int maxfd,ret,i;
	FD_ZERO(&global);
	FD_SET(sockfd,&global);
	FD_ZERO(&current);
	maxfd=sockfd;
	while(1)
	{
		current=global;//临时的把global给缓存； 

		if((ret=select(maxfd+1,&current,NULL,NULL,NULL))<0)
		{
			perror("fail to select");
			return -1;
	
		}
		if(ret >0 )
		{

			if(FD_ISSET(sockfd,&current)>0)
			{
				if((confd=accept(sockfd,NULL,NULL))<0)
				{
					perror("fail to accept");
					return -1;
				
				}
				FD_SET(confd,&global);
				if(confd>=maxfd)//更新最大值 
				{
					maxfd=confd;
				}
			
			}
			for(i=sockfd+1;i<=maxfd;i++)//遍历 
			{
				if(FD_ISSET(i,&current)>0)//不是从global，而是从current是怕申请队员突然退出。 
				{
					bzero(buf,128);

					recv(i,buf,128,0);
					printf("%s\n",buf);
					send(i,buf,128,0);

					bzero(buf,128);
				
				
				}
			
			
			
			}
		
		
		
		
		}
	
	
	
	
	
	}


	
	
	return 0;
}
