#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"
#include "motor.h"

/*This is simple project to test how git working and learn git.
mrsahin101 31.10.2021 */

uint32_t temperature = 25;

int main(int argc, char *argv[]) {
	
	motor_init();
	sensor_init();
	
	while(1)
	{
		sensor_read();
		mydelay();
		if(temperature > 30)
		{
			cooling_motor_on();
		}
		if(temperature < 20)
		{
			heating_motor_on();
		}
	}
	return 0;
}
void mydelay(void)
{
	long int i= 0;
	for(i = 0; i < 400000000;i++)
	{
		
	}
	return;
}
