#include <stdio.h>
#include <unisted.h>
#include <fcntl.h>
#include <errno.h>

int main(){
	char to_dev;
	int w;
	int fd;
	while(1){
		fd = open("/dev/motor_dev",O_RDWR);
		if(fd<0){
			perror("opening device FAIL!\n");
			return errno;
		}
		printf("0 degree\n");
		// turning
		to_dev='0';
		w=write(fd,&to_dev,1);
		printf("180 degree\n");
		sleep(3);
		// returning
		to_dev='1';
		w=write(fd,&to_dev,1);
		printf("0 degree\n");
		
		close(fd)
		sleep(3);
	}
	return 0;
}
		
		
