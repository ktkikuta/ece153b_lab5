/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

static volatile uint32_t Software_ComputedCRC;
static volatile uint32_t Initial_CRC = INITIAL_CRC_VALUE;
static volatile uint32_t time;


int main(void) {

	// Switch System Clock = 80 MHz
	System_Clock_Init();
	// initialize modules
	SysTick_Init();
	LED_Init();
	UART2_Init();

	volatile uint32_t i = 0;

	while(i <= BUFFER_SIZE) {
		LED_Toggle();
		// initialize CRC
		// start timer
		startTimer();
		// compute CRC
		Software_ComputedCRC = CrcSoftwareFunc(Initial_CRC, DataBuffer[i], POLYNOME);
		// end timer
		time = endTimer();
		// check CRC
		if(Software_ComputedCRC != uwExpectedCRCValue){
			LED_Off();
		}
		// print time
		printf("Time = %d ms", time);
		// delay 1 sec
		delay(1000);
		i++;
	}
}
