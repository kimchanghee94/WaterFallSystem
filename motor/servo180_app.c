#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(void){
  char k;
  int fd, fp;
  while(1){
		fd = open("/dev/servo180_dev", O_RDWR);

		if(fd < 0){
			perror("Failed to open the device\n");
			return errno;
		}
		printf("0\n");
		k = '0';
		fp = write(fd, &k, 1);
		printf("1\n");
		sleep(2);

		k = '1';
		fp = write(fd, &k, 1);
		printf("2\n");
		close(fd);
  
		sleep(2);
	}
	return 0;
}
