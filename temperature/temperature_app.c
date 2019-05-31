#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/temperature_dev"

#define BUFFER 1024


int main(void){
	int temperature_fd = open(DEV_PATH,O_RDWR|O_NONBLOCK);
	char buffer[BUFFER];
	char receive[BUFFER];
	int recev = 0;

	if(temperature_fd < 0){
		printf("Failure : while openning device node\n");
		printf("%d\n",temperature_fd);
		exit(1);
	}

	while(1){
		usleep(1000*500);
		read(temperature_fd,receive,BUFFER);
		printf("%d\n",recev);
	}

	close(temperature_fd);
	return 0;
}

