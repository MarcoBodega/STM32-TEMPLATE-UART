#ifndef _UARTFC_H
#define _UARTFC_H


#include <stm32f10x_usart.h>

#define QUEUE_SIZE 128
# define HIGH_WATER ( QUEUE_SIZE - 6)

//#define HWFLOWCTRL

uint8_t _getchar(void);
uint8_t _putchar(const uint8_t c);


int uart_open(uint8_t uart, uint32_t baud, uint32_t flags);
int uart_close(uint8_t uart);
uint16_t uart_read (uint8_t uart, uint8_t *buf, uint16_t nbyte);
uint16_t uart_write(uint8_t uart, const uint8_t *buf, uint16_t nbyte);

#endif
