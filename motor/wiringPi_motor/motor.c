#include <wiringPi.h>

#define GPIO18 1

int main (void)
{
	wiringPiSetup();
	pinMode(GPIO18, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(2000);
	pwmSetClock(192);

	while(1){
		pwmWrite(GPIO18, 150);
		delay(1000);
		pwmWrite(GPIO18,200);

	}
	return 0;
}
