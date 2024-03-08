/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "SysTimer.h"

static uint32_t volatile step;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter

	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	// TODO
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void) {
	++step;
}

void delay(uint32_t us) {
	step = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = 79;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while((SysTick->VAL & us) == 0);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startTimer(void) {
	//reset counter
	step = 0;
	//reset VAL to 0
	SysTick->VAL = 0;
	//Set LOAD
	SysTick->LOAD = 79;
	//Enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t endTimer(void) {
	//Disable SysTick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	//Read values from VAL and counter
	//Calculate the time using both numbers
	return step;
}
