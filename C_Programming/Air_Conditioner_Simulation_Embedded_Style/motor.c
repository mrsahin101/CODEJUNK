#include "motor.h"
#include "main.h"
#include <stdio.h>



void cooling_motor_on(void)
{
	while(temperature >= 25)
	{
		printf("Cooling system is on. Current Temperature : %d\n",temperature);
		temperature--;
		mydelay();
	}
}
void heating_motor_on(void)
{
	while(temperature <= 25)
	{
		printf("Heating system is on. Current Temperature : %d\n",temperature);
		temperature++;
		mydelay();
	}
}
void motor_init(void)
{
	printf("AC cooling--heating system has been initialized..\n");
}
