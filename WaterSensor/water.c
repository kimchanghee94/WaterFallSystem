#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>


#define CS_MCP3008 6
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

int read_mcp3008_adc(unsigned char adcChannel){
	unsigned char buff[3];
	int adcValue=0;
	buff[0] = 0x06 | ((adcChannel & 0x07) >> 7);
	buff[1] = ((adcChannel & 0x07) << 6);
	buff[2]=0x00;
	digitalWrite(CS_MCP3008,0);
	wiringPiSPIDataRW(SPI_CHANNEL,buff,3);
	buff[1]=0x0f & buff[1];
	adcValue = (buff[1] <<8) | buff[2];
	digitalWrite(CS_MCP3008,1);
	return adcValue;
}

int main(void){
	int adcChannel=0;
	int adcValue =0;
	
	if(wiringPiSetup()==-1) return 1;
	if (wiringPiSPISetup(SPI_CHANNEL,SPI_SPEED) == -1) return 1;
	pinMode(CS_MCP3008,OUTPUT);
	
	while(1)
	{
		adcValue=read_mcp3008_adc(adcChannel);
		printf("adc0 Value = %u\n",adcValue);
		delay(1000);
	}
	return 0;
}
