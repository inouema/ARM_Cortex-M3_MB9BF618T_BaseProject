/********************************************************************************/
/*!
	@file			uart.h
	@author       Masato INOUE
	@version        1.00
	@date           2012.05.03
	@brief          Based on xxx.

    @section HISTORY
		2012.05.03	V1.00	Start Here.
		
    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

#ifndef __UART_H__
#define __UART_H__

/* Functions -----------------------------------------------------------------*/
void    putch(uint8_t);
uint8_t getch(void);
void    UartInit(void);



#endif // __UART_H__

/* End Of File ---------------------------------------------------------------*/
