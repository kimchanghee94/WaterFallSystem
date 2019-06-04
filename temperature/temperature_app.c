#define _REENTRANT
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define PROBEPATH "/sys/bus/w1/devices"
#define MAXPROBES 5
#define PROBENAMELEN 80
#define BUFSIZE 256

#define BUFF_SIZE 100
#define PORT 20162
#define BUFFER_SIZE 4096

char probepath[MAXPROBES][PROBENAMELEN];
char probename[MAXPROBES][PROBENAMELEN];
char alias[MAXPROBES][BUFSIZE];

FILE *probefd;
int numOfSensor;

int findprobes(void){
	struct dirent *pDirent;
	DIR *pDir;
	int count;

	count = 0;

	pDir = opendir(PROBEPATH);
	if(pDir==NULL){
		printf("Cannot open directory '%s'\n",PROBEPATH);
		return 0;
	}

	while((pDirent = readdir(pDir)) != NULL){
		if(pDirent->d_name[0] == '2' && pDirent->d_name[1]=='8'
				&& pDirent->d_name[2] == '-'){

			snprintf(probepath[count],PROBENAMELEN-1,"%s/%s/w1_slave",PROBEPATH,pDirent->d_name);
			snprintf(probename[count],PROBENAMELEN-1,"%s",pDirent->d_name);

			count++;
		}
	}

	closedir(pDir);
	return count;
}



int main(int argc, char * argv[]){
	int i;
	double temperature;
	char *temp;
	time_t now;
	struct tm *t;
	char buf[BUFSIZE];

	if(argc != 2){
		printf("Usgae: %s IPv4-address\n",argv[0]);
		return -1;
	}

	struct sockaddr_in connectSocket;

	memset(&connectSocket,0,sizeof(connectSocket));

	connectSocket.sin_family = AF_INET;
	inet_aton(argv[1],(struct in_addr*)&connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(PORT);

	int connectFD = socket(AF_INET,SOCK_STREAM,0);

	if(connect(connectFD,(struct sockaddr*)&connectSocket,sizeof(connectSocket))==-1){
		printf("Can not connect.\n");
		return -1;
	}
	else{
		int readBytes, writtenBytes;
		char sendBuffer[BUFFER_SIZE];
		char receiveBuffer[BUFFER_SIZE];


	numOfSensor = findprobes();
	if(numOfSensor==0){
		printf("Error: No DS18B20 compatible probes located.\n");
		exit(-1);
	}

	while(1){
		for(i=0;i<numOfSensor;i++){
			probefd = fopen(probepath[i],"r");

			if(probefd==NULL){
				printf("Error\n");
				exit(-1);
			}

			fgets(buf,sizeof(buf)-1,probefd);
			memset(buf,0,sizeof(buf));

			fgets(buf,sizeof(buf)-1,probefd);
			temp = strtok(buf,"t=");
			temp = strtok(NULL,"t=");
			temperature = atof(temp)/1000;

			if(temperature>30.0){
				write(connectFD,"1\n",2);
			}else{
				
				write(connectFD,"2\n",2);
			}

			now = time(NULL);
			t = localtime(&now);

			printf("%2.3f\n",temperature);

			fclose(probefd);
			fflush(stdout);
		}
	}
	}
	close(connectFD);
	return 0;
}

