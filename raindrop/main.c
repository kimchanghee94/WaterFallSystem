#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 20162
#define BUFFER_SIZE 4096
#define BUFF_SIZE 100


int main(int argc, char ** argv) {



	if(argc!=2){
		printf("Usage: %s IPv4-address\n",argv[0]);
		return -1;
	}

	struct sockaddr_in connectSocket;

	memset(&connectSocket,0,sizeof(connectSocket));

	connectSocket.sin_family = AF_INET;
	inet_aton(argv[1],(struct in_addr*)&connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(PORT);

	int connectFD = socket(AF_INET,SOCK_STREAM,0);

	int rain_fd = open("/dev/rains_dev",O_RDONLY);
	if(rain_fd<0){
		perror("file open failed");
		exit(-1);
	}
	int rain_detect;
	if(connect(connectFD,(struct sockaddr*)&connectSocket,sizeof(connectSocket))==-1){
		printf("connect() error");
		return -1;
	}
	else{
		int readBytes, writtenBytes;
		char sendBuffer[BUFFER_SIZE];
		char receiveBuffer[BUFFER_SIZE];


	while(1){
		usleep(500000);
		read(rain_fd, &rain_detect, sizeof(int));
		if(rain_detect == 0){
			printf("Rainning\n");
			write(connectFD,"3\n",2);

		}
		else{
			printf("Not Rainning\n");
			write(connectFD,"4\n",2);

		}
	
		
	}
	}
	close(connectFD);
	close(rain_fd);
	
	return 0;
}

