#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc, const char *argv[])
{
	int fd;
	int sockfd;
	int condfd;
	int a;
	char buf[128];
	long portno;
   int iSockOptVal = 1;
	if(argc < 1)
	{
		printf("error\n");
		return -1;
	}
	portno = atoi(argv[1]);
	bzero(buf,128);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(portno);
	serveraddr.sin_addr.s_addr = inet_addr(argv[2]);
	/*fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	*/
						
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1) 
	{
	   perror("setsockoptkopt fail");
	   close(sockfd);
	   exit(-1);

    }
	condfd = connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(condfd == -1)
	{
		perror("Connect");
		return -1;
	}
	while(1)
	{
		/*
		a = read(fd,buf,100);
		if(a <= 0)
		{
			break;
		}
		*/
		scanf("%s",buf);
		printf("send:%s\n",buf);

		send(sockfd,buf,128,0);
		bzero(buf,strlen(buf));
	}
	close(sockfd);
	close(condfd);
	return 0;
}
