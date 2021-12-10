#include "sensor.h"
#include "main.h"
#include <stdio.h>

uint32_t sensor_read(void)
{
	uint32_t temp = 0;
	temp = (rand() % 10) - 5;
	printf("Current Temperature: %d\n",temperature+ temp);
	temperature = temperature + temp;
	return temperature;
}
void sensor_init(void)
{
	srand(time(NULL)); 
	printf("Sensor Data Flow Has been Started....\n");
}
