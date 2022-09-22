/*
 ******************************************************************************
 * @file           : main.c
 * @author         : Abdallah Ahmed Mohammed
 * @brief          : Main program body
  * @Assignment     : Lab 2
 ******************************************************************************
 */
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// register Base addresses
#define GPIOA_BASE 0x40010800
#define RCC_BASE 0x40021000

// register offset addresses
#define RCC_APB2ENR  *(volatile uint32_t *)(RCC_BASE + 0x18)
#define GPIOA_CRH    *(volatile uint32_t *)(GPIOA_BASE + 0x04)
#define GPIOA_ODR    *(volatile uint32_t *)(GPIOA_BASE + 0x0C)
#define RCC_CFGR     *(volatile uint32_t *)(RCC_BASE + 0x04)


int main(void)
{

	//Bits 10:8 PPRE1: APB low-speed prescaler (APB1)
	//100: HCLK divided by 2
	RCC_CFGR |= (0b100 << 8);

	//Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	//101: HCLK divided by 4
	RCC_CFGR |= (0b101 << 11);


	RCC_APB2ENR |= (1<<2);
	GPIOA_CRH &= 0xFF0FFFFF;
	GPIOA_CRH |= 0x00200000;
	
	while(1)
	{
		GPIOA_ODR |=(1<<13);
		for(int i=0; i<1000;i++);
		GPIOA_ODR &=~(1<<13);
		for(int i=0;i<1000;i++);
	}
}