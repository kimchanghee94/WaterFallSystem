#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define DETECT			1
#define UNDETECT		0

int main(void) {


	int rain_fd = open("/dev/rains_dev", O_RDONLY);

	
	int	rain_detect;
	
	
	while(1){
		usleep(500000);
		read(rain_fd, &rain_detect, sizeof(int));
		printf("%d\n", rain_detect);
		
	}
	return 0;
}

