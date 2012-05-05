/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  SPI Driver
 */

#ifndef _SPIDEV_FM3_H_
#define _SPIDEV_FM3_H_

#include "SioDev.h"

/*
 *  Driver Status
 */
#define SPI_UNINITIALIZED    0
#define SPI_INITIALIZED      1


/*
 *  Default Value
 */
#define SPI_BAUDRATE      500000                          /* Default Baudrate      */
#define SPI_POLARITY      SpiDev_CLOCK_POLARITY_IDLEHIGH  /* Default SCLK Polarity */
#define SPI_MODE          SpiDev_MODE_MASTER              /* Default Mode          */
#define SPI_BITODER       SpiDev_BITORDER_MSB             /* Default BitOrder      */


/*
 *  SIO Driver : IOB Struct
 */
extern SioDev_IOB SioDev[];


#endif /* _SPIDEV_FM3_H_ */

