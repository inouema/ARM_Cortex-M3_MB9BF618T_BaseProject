/********************************************************************************/
/*!
	@file			systick.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2010.03.05
	@brief          delay mSec-order routine using systick timer			@n
					delay uSec-order routine using __NOP();

    @section HISTORY
		2010.03.05	V1.00	Restart Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H 0x0001

/* General Inclusion */
#include "hw_config.h"


/* Systick Interval milliSecond order */
#define INTERVAL	1000

/* Inline Static Function */
extern __IO uint32_t TimingDelay;
static inline void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/* Externals */
extern void _delay_ms(__IO uint32_t mSec);
extern void _delay_us(__IO uint32_t uSec);
extern void SysTickInit(__IO uint32_t interval);

#endif /* __SYSTICK_H */
