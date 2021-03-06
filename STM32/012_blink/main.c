/*
 * main.c
 *
 *  Created on: Feb 12, 2022
 *      Author: HP
 */


typedef unsigned long uint32_t;

#define FLASH_BASE			  0x08000000UL
#define SRAM_BASE             0x20000000UL
#define SRAM2_BASE            0x2001C000UL
#define PERIPH_BASE			  0x40020000UL

//define APB1PERIPH_BASE       PERIPH_BASE
//#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
//#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
//#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000UL)
#define GPIOD_BASE            (PERIPH_BASE + 0x0C00UL)

#define SRAM_SIZE  		(112)*1024
#define SRAM_END        (SRAM_BASE + SRAM_SIZE)

#define RCC_BASE 		(PERIPH_BASE + 0x3800UL)
#define RCC_AHB1ENR		((uint32_t *)(RCC_BASE +0x30))

#define GPIOD_MODER		((uint32_t*) (GPIOD_BASE + 0x00))
#define GPIOD_ODR		((uint32_t*) (GPIOD_BASE + 0x14))
#define GPIOD_OTYPER	((uint32_t*) (GPIOD_BASE + 0x04))
#define GPIOD_OSPEEDR	((uint32_t*) (GPIOD_BASE + 0x08))
#define GPIOD_PUPDR		((uint32_t*) (GPIOD_BASE + 0x0C))

int main(void);
void delay(void);

uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
    (uint32_t *)SRAM_END,   // initial stack pointer
    (uint32_t *)main        // main as Reset_Handler
};




void delay(void){
	uint32_t i = 0x000FFFFF;

	while(i > 0){
		i--;
	}
}
int main(void)
{
    /* Loop forever */
	*RCC_AHB1ENR = (1<<3);

	*GPIOD_MODER 	= 0x55000000;
	*GPIOD_OTYPER 	= 0xFF000000;
	*GPIOD_OSPEEDR 	= 0xFF000000;
	*GPIOD_PUPDR    = 0x00000000;

	while(1){

		*GPIOD_ODR = 0x0000F000;
		delay();
		*GPIOD_ODR = 0x00000000;
		delay();
	}
}
