#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 20162
#define BUFFER_SIZE 4096
#define BUFF_SIZE 100

#defien LISTEN_QUEUE_SIZE 5

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
	listenSockte.sin_port = htons(PORT);
