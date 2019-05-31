#include <stdio.h>
#include <wiringPi.h>
#define RAIN_PIN 4
int main(void){
	wiringPiSetupGpio();
	pinMode(RAIN_PIN,INPUT);

	while(1)
	{
		if(digitalRead(RAIN_PIN) == LOW){
			printf("Rainning\n");
		}
		else{
			printf("Not Rainning\n");
		}
		delay(5000);
	}
	return 0;
}
