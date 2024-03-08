/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "UART.h"

void UART1_Init(void) {
	//unused
}

void UART2_Init(void) {
	//enable usart clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	//select system clock as clock source
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {
	//unused
}

void UART2_GPIO_Init(void) {
	//clock enable
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	//output mode to alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODE2;
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3;
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	//set afr
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_2;
	//Set pins to very high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	//set push pull output type
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	//pull up resistors
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	//Disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	//Set word length to 8 bits
	USARTx->CR1 &= ~USART_CR1_M0;
	//Oversampling by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	//1 stop bit
	USARTx->CR2 &= ~USART_CR2_STOP;
	//Set baud rate to 9600
	USARTx->BRR = 8333;
	//Enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;
	//Enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
