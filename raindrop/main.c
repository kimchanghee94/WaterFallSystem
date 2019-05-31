#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(void) {


	int rain_fd = open("/dev/rains_dev", O_RDONLY);

	if(rain_fd<0){
		perror("file open failed");
		exit(-1);
	}
	
	int	rain_detect;
	
	
	while(1){
		usleep(500000);
		read(rain_fd, &rain_detect, sizeof(int));
		if(rain_detect == 0)
			printf("Rainning\n");
		else
			printf("Not Rainning\n");
	
		
	}
	close(rain_fd);
	
	return 0;
}

