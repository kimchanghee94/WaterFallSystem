#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define PROBEPATH "/sys/bus/w1/devices"
#define MAXPROBES 5
#define PROBENAMELEN 80
#define BUFSIZE 256

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

			printf("Found DS18B20 compatible probe named '%s':\nDevice file '%s'\n",probename[count],probepath[count]);
			count++;
		}
	}

	closedir(pDir);
	return count;
}

int main(){
	int i;
	double temperature;
	char *temp;
	time_t now;
	struct tm *t;
	char buf[BUFSIZE];

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


