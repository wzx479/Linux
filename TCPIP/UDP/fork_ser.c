#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SA struct sockaddr
int main(int argc, const char *argv[])
{
	int iSockOptVal = 1;
	char buf[128];
	bzero(buf,128);
	int ret;
	pid_t pid;
	int sockfd,confd;

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(50000);
	seraddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("fail to socket");
		return -1;
	}
#if 0
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1) {
		perror("setsockopt fail");
		close(sockfd);
		exit(-1);
	}
#endif
	if(bind(sockfd,(SA*)&seraddr,sizeof(seraddr))<0)
	{
		perror("fail to bind");
		return -1;
	}
	listen(sockfd,5);
    signal(SIGCHLD,SIG_IGN);/*SIGCHLD信号发送给其父进程，按系统默认将忽略此信号，如果父进程希望被告知其子系统的这种状态，则应捕捉此信号。*/

	while(1)
	{
		if((confd = accept(sockfd,NULL,NULL))<0)
		{
			perror("fail to accept");
			return -1;
		}
		pid = fork();
		if(pid < 0)
		{
			perror("fail to fork");
			close(confd);
			return -1;
		}
		if(0 == pid)
		{
			while(1)
			{
				printf("fork ok\n");
				bzero(buf,128);
				ret= recv(confd,buf,128,0);
				printf("ret = %d\n",ret);

				if(0 == ret)
				{
					break;
				}
				if(ret < 0 )
				{
					close(confd);
					close(sockfd);
					return -1;
				}
				printf("%s\n",buf);
				send(confd,buf,128,0);
			
			}
			close(confd);
		}
		else
			close(confd);

	}
	return 0;
}
