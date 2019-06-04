#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PORT 20162
#define BUFFER_SIZE 4096
#define BUFF_SIZE 100

#define LISTEN_QUEUE_SIZE 5

void childHandler(int signal)
{
	int status;
	pid_t spid;

	while((spid = waitpid(-1,&status,WNOHANG))>0){
	}
}

int main(){
	signal(SIGCHLD, (void*)childHandler);

	struct sockaddr_in listenSocket;

	memset(&listenSocket, 0, sizeof(listenSocket));

	listenSocket.sin_family = AF_INET;
	listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	listenSocket.sin_port = htons(PORT);

	int listenFD = socket(AF_INET, SOCK_STREAM,0);
	int connectFD;

	ssize_t receivedBytes;
	char readBuff[BUFFER_SIZE];
	char sendBuff[BUFFER_SIZE];
	pid_t pid;

	if(bind(listenFD, (struct sockaddr *)&listenSocket, sizeof(listenSocket))==-1){
		printf("Can not bind.\n");
		return -1;
	}

	if(listen(listenFD, LISTEN_QUEUE_SIZE) == -1){
		printf("Listen fail.\n");
		return -1;
	}

	printf("Wating for clients....\n");

	while(1){
		struct sockaddr_in connectSocket, peerSocket;

		socklen_t connectSocketLength = sizeof(connectSocket);

		while((connectFD = accept(listenFD,(struct sockaddr*)&connectSocket,(socklen_t *)&connectSocketLength))>=0){
			getpeername(connectFD,(struct sockaddr*)&peerSocket, &connectSocketLength);
			char peerName[sizeof(peerSocket.sin_addr)+1]={0};
			sprintf(peerName,"%s",inet_ntoa(peerSocket.sin_addr));

			if(strcmp(peerName,"0.0.0.0")!=0){
				printf("Client : %s\n",peerName);
			}

			if(connectFD<0){
				printf("Server: accept failed\n");
				exit(0);
			}

			if(pid==0){
				close(listenFD);

				ssize_t receivedBytes;

				while((receivedBytes = read(connectFD,readBuff,BUFF_SIZE))>0){
					readBuff[receivedBytes]='\0';
					fputs(readBuff,stdout);
					fflush(stdout);

					sprintf(sendBuff,"%s",readBuff);
					write(connectFD,sendBuff,strlen(sendBuff));
				}

				close(connectFD);
				return 0;
			}
			else{
				close(connectFD);
			}
		}
	}
	close(listenFD);
	return 0;
}


