/********************************************************************************/
/*!
	@file			systick.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2011.01.20
	@brief          delay mSec-order routine using systick timer			@n
					delay uSec-order routine using __NOP();

    @section HISTORY
		2011.01.20	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"

/* Defines -------------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Grobals -------------------------------------------------------------------*/
__IO uint32_t TimingDelay;

/* Prototypes ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*! 
    @brief	SysTickTimer Initialize(every 1mSec)
	@param	interval: Set Systick timer interval ratio.
    @retval	none
*/
/**************************************************************************/
void SysTickInit(__IO uint32_t interval)
{
  if (SysTick_Config(SystemCoreClock / interval)) { /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                  	/* Capture error */
  }

}


/**************************************************************************/
/*! 
    @brief	Delay Millisecond Order
	@param	mSec:
    @retval	none
*/
/**************************************************************************/
void _delay_ms(__IO uint32_t mSec)
{ 
	TimingDelay = mSec;

	while(TimingDelay != 0);
}

/**************************************************************************/
/*! 
    @brief	Delay Microsecond Order(no-accuracy)
	@param	uSec 
    @retval	none
*/
/**************************************************************************/
void _delay_us(__IO uint32_t uSec)
{
	while(uSec--){ 
					__NOP();
					__NOP();
					__NOP();
					__NOP();
					__NOP();
					}
}


/**************************************************************************/
/**
  * @brief  This function handles SysTick Handler.
  * @param  : None
  * @retval : None
  */
/**************************************************************************/
void SysTick_Handler(void)
{
	/* every 1mSec for _delay_ms() */
	TimingDelay_Decrement();
	
	/* another function */
	Cyclic_Exec();

}

/* End Of File ---------------------------------------------------------------*/
