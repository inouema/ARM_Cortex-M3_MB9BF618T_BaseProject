/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  SIO Driver
 */

#ifndef _SIODEV_FM3_H_
#define _SIODEV_FM3_H_

/*
 *  FIFO Depth
 */
#define SioDev_CH04_FIFO_DEPTH    16
#define SioDev_CH05_FIFO_DEPTH    16
#define SioDev_CH06_FIFO_DEPTH    16
#define SioDev_CH07_FIFO_DEPTH    16


/*
 *  Driver Status
 */
#define SIO_UNINITIALIZED    0
#define SIO_INITIALIZED      1


/*
 *  Default Value
 */
#define SIO_BAUDRATE      115200                   /* Default Baudrate      */
#define SIO_DATABITS      SioDev_DATABITS_8        /* Default Data bits     */
#define SIO_MODE          SioDev_MODE_MASTER       /* Default Mode          */
#define SIO_SCKPOLARITY   SioDev_SCKPOLARITY_HIGH  /* Default SCLK Polarity */
#define SIO_BITODER       SioDev_BITORDER_MSB      /* Default BitOrder      */


/*
 *  Register Bit Definition
 */
#define SMR_SOE          0x01U
#define SMR_SCKE         0x02U
#define SMR_BDS          0x04U
#define SMR_SCINV        0x08U
#define SMR_WUCR         0x10U
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U

#define SCR_TXE          0x01U
#define SCR_RXE          0x02U
#define SCR_TBIE         0x04U
#define SCR_TIE          0x08U
#define SCR_RIE          0x10U
#define SCR_SPI          0x20U
#define SCR_MS           0x40U
#define SCR_UPGL         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_REC          0x80U

#define ESCR_SOP         0x80U
#define ESCR_DATABITS_8  0x00U
#define ESCR_DATABITS_5  0x01U
#define ESCR_DATABITS_6  0x02U
#define ESCR_DATABITS_7  0x03U
#define ESCR_DATABITS_9  0x04U
#define ESCR_WTBITS_0    0x00U
#define ESCR_WTBITS_1    0x08U
#define ESCR_WTBITS_2    0x10U
#define ESCR_WTBITS_3    0x18U

#define FCR1_FSEL        0x01U
#define FCR1_FTIE        0x02U
#define FCR1_FDRQ        0x04U
#define FCR1_FRIIE       0x08U
#define FCR1_FLSTE       0x10U

#define FCR0_FE1         0x01U
#define FCR0_FE2         0x02U
#define FCR0_FCL1        0x04U
#define FCR0_FCL2        0x08U
#define FCR0_FSET        0x10U
#define FCR0_FLD         0x20U
#define FCR0_FLST        0x40U


/*
 *  Enable/Disable Interrupt Controller
 */
/* deviation from MISRA-C:2004 Rule 19.7 */
#define SIO_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define SIO_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))


#endif /* _SIODEV_FM3_H_ */

