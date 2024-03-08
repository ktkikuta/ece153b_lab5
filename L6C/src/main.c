/*
 * ECE 153B
 *
 * Name(s): Alex Xu, Kole Kikuta
 * Section: Ryan Chau, Wed 19:00
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "DMA.h"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;
static volatile uint32_t ComputedCRC;
static volatile uint32_t ComputationDone = 0;

void completeCRC(uint32_t crc){
	// 1. Store the calculated CRC value in a volatile static variable
	ComputedCRC = crc;

	// 2. Mark the computation as completed
	ComputationDone = 1;
}

int main(void) {
  	uint32_t time;

	// Switch System Clock = 80 MHz
	System_Clock_Init();
	// systick init
	SysTick_Init();
	// uart init
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	// led, crc, dma init
	LED_Init();
	CRC_Init();
	DMA_Init();

	printf("Starting CRC\n");

	while(1) {
		// 1. Toggle the LED to show that the CRC calculation is running
		LED_Toggle();

		// 2. Start timer
		startTimer();

		// 3. Mark the computation as not complete
		ComputationDone = 0;

		// 4. Reset the CRC in the control register
		CRC->CR |= CRC_CR_RESET;

		// 5. Set number of data to transfer in CNDTR in DMA
		DMA1_Channel6->CNDTR = BUFFER_SIZE;

		// 6. Enable DMA channel to start data transfer and CRC computation
		DMA1_Channel6->CCR |= DMA_CCR_EN;

		// 7. Wait till the computation is completed
		while (ComputationDone != 1);

		// 8. Disable DMA channel
		DMA1_Channel6->CCR &= ~DMA_CCR_EN;

		// 9. Stop timer and store the time to calculate
		time = endTimer();

		// 10. Check if calculated CRC matches the expected value. if not, turn off the LED and exit.
		if(ComputedCRC != uwExpectedCRCValue){
			LED_Off();
			exit(0);
		}

		// 11. Print the time to calculate in microseconds
		printf("Time: %d us\n", time);

		// 12. Delay 1 second
		delay(1000000); // delay is in us
	}
}
