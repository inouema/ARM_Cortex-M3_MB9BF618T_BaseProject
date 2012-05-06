/********************************************************************************/
/*!
	@file			main.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
	@version        1.00
	@date           2012.04.01
	@brief          KickStart program For MB9BF618T JunkBoard!			@n

    @section HISTORY
		2012.04.01	V1.00 Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "uart.h"
#include "xprintf.h"


/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/




/* Functions -----------------------------------------------------------------*/

/**
 *
 * @brief	Main Function
 * @param	none
 * @retval	none
 */
int main(void)
{
	/* Set 1mSec Timer */
	SysTickInit(INTERVAL);

	/* Initialize GPIO for the LED. */
	FM3_GPIO->PDORF_f.P3 = 0;		/* Set Clr(at first)*/
	FM3_GPIO->PFRF_f.P3  = 0;		/* Use PF3 as GPIO	*/
	FM3_GPIO->DDRF_f.P3  = 1;		/* Mode Output	  	*/
	FM3_GPIO->PZRF_f.P3  = 1;		/* Port Tristate  	*/

	/* Initialize UART */
	UartInit();

	/* To Use xprintf */
	xdev_out(putch);
	xdev_in(getch);


    /* Test */
	xprintf("Hello FM3 \r\n");
    _delay_ms(1000);

#if 0
	while (1)		/* loop forever */
	{
		FM3_GPIO->PDORF_f.P3 = ~FM3_GPIO->PDORF_f.P3;	/* Invert Pin */
		_delay_ms(500);
	}
#endif
}



/* End Of File ---------------------------------------------------------------*/
