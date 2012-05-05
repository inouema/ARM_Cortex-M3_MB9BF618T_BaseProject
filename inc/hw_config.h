/********************************************************************************/
/*!
	@file			hw_config.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2012.02.02
	@brief          For FUJITSU MB9BF618T Versaion.

    @section HISTORY
		2012.02.02	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H 0x0100

/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

/* Function Inclusion */
#include "mcu.h"
#include "systick.h"

/* High Level Function */
#include "uart.h"

/* Macros */
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define Cyclic_Exec()	{		\
								\
						}

/* Externals */


#endif  /*__HW_CONFIG_H*/
