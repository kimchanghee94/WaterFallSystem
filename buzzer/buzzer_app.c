#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(void){
	char input;
	int fd,fp;
	fd = open("/dev/buzzer_dev",O_RDWR);
	if(fd<0){
		perror("Failed\n");
		return -1;
	}

	while(1){

		scanf("%c",&input);
		int len = write(fd, &input,1);
	}

	return 0;
}

		
