//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "diag/Trace.h"
//#include "Timer.h"

#include "uart.h"
#include <stm32f10x.h>


// ----- Timing definitions -------------------------------------------------

//#define TICKS  (TIMER_FREQUENCY_HZ )

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via STDOUT).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the STDOUT output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//#define myUSART USART1
#define myUSART 1


uint8_t buf[QUEUE_SIZE];
uint8_t nbyte = 32; //size_t

uint8_t cmd[16];
uint8_t cmdi; //size_t

uint8_t cmd_feedback[32];

// avoid newlib
int strlen(char *s);
int strcmp(const char* s1, const char* s2);
char *strcpy(char *dst, char *src);


int main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.
	//trace_printf("System clock: %uHz\n", SystemCoreClock);

	uart_open(myUSART, 9600, 0);
	uint8_t ri;
	cmdi = 0;

	// prompt
	uart_write(myUSART, "# ", strlen("# "));

	// Infinite loop
	while (1)
	{
		// read
		ri = uart_read(myUSART, buf, nbyte);

		if(ri > 0) {

			for(int j=0; j<ri; j++, cmdi++)
				cmd[cmdi] = buf[j];

			// \r = 0x0A, \n = 0x0D
			if(buf[ri-1] == '\r' /*|| buf[ri-1] == '\n'*/) {

				//echo it back!!
				ri++;
				buf[ri-1] = '\n';
				uart_write(myUSART, buf, ri);

				// replace \r or \n with string end
				cmd[cmdi-1] = '\0';

				cmd_feedback[0] = '\0';
				if(strcmp(cmd, "PLAY") == 0) {
					strcpy(cmd_feedback,"Received PLAY :) \n\r");
				}
				else if(strcmp(cmd, "PAUSE") == 0) {
					strcpy(cmd_feedback,"Received PAUSE :) \n\r");
				}
				else if(strcmp(cmd, "STOP") == 0) {
					strcpy(cmd_feedback,"Received STOP :) \n\r");
				}
				else if(strcmp(cmd, "RESET") == 0) {
					strcpy(cmd_feedback,"Received RESET :) \n\r");
				}

				if(strlen(cmd) > 0)
					uart_write(myUSART, cmd_feedback, strlen(cmd_feedback));

				// clean cmd
				cmd[0] = '\0';
				cmdi = 0;

				// prompt
				uart_write(myUSART, "# ", strlen("# "));
			}
			else {
				//echo it back!!
				uart_write(myUSART, buf, ri);
			}
		}
	}
	uart_close(myUSART);
}






	/*
	// At this stage the system clock should have already been configured
	// at high speed.
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_USART2, ENABLE );
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE );

	timer_start();

	uart_open(USART2, 9600, 0);

	uint32_t seconds = 0;
	uint16_t Data;
	//test write

	// Infinite loop
	while (1)
	{
		//timer_sleep(TICKS);

		// read
		//int c = uart_getc(USART3);
		while ( USART_GetFlagStatus (USART2 , USART_FLAG_RXNE ) == RESET);
		Data = USART_ReceiveData(USART2);

		//echo it back!!
		//uart_putc(Data, USART3);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, Data);
		//USART_SendData(USART3, 't');

		trace_printf("Read char %c with code %d\n", Data, Data);

		++seconds;

		// Count seconds on the trace device.
		trace_printf("Second %u\n", seconds);
	}

	uart_close(USART2);
}*/


int strlen(char *s)
{
    char *p = s;
    while (*p != '\0')
        p++;
    return p - s;
}

char *strcpy(char *dst, char *src)
{
    while((*dst++ = *src++)!= '\0')
        ; // <<== Very important!!!
    return dst;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}




#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    // Infinite loop
    // Use GDB to find out why we're here 
    while (1);
}
#endif



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
