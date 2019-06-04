#define _REENTRANT
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define PROBEPATH "/sys/bus/w1/devices"
#define MAXPROBES 5
#define PROBENAMELEN 80
#define BUFSIZE 256

void *send_msg(void * arg);
void *recv_msg(void * arg);
void error_handling(char *msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

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
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	int i;
	double temperature;
	char *temp;
	time_t now;
	struct tm *t;
	char buf[BUFSIZE];

	if(argc!=4){
		printf("Usage: %s <IP> <port> <name>\n",argv[0]);
		exit(1);
	}

	sprintf(name,"[%s]",argv[3]);
	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1){
		error_handling("socket() error");
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}

	pthread_create(&snd_thread,NULL,send_msg,(void*)&sock);
	pthread_create(&rcv_thread,NULL,recv_msg,(void*)&sock);
	pthread_join(snd_thread,&thread_return);
	pthread_join(rcv_thread,&thread_return);
	close(sock);

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

			now = time(NULL);
			t = localtime(&now);

			printf("%2.3f\n",temperature);

			fclose(probefd);
		}
	}
	return 0;
}

void * send_msg(void * arg){
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1){
		fgets(msg,BUF_SIZE,stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")){
			close(sock);
			exit(0);
		}
		sprintf(name_msg,"%s %s",name,msg);
		write(sock,name_msg,strlen(name_msg));
	}
	return NULL;
}

void * recv_msg(void *arg){
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1){
		str_len = read(sock,name_msg,NAME_SIZE+BUF_SIZE-1);
		if(str_len==-1){
			return (void*)-1;
		}
		name_msg[str_len]=0;
		fputs(name_msg,stdout);
	}
	return NULL;
}

void error_handling(char * message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

