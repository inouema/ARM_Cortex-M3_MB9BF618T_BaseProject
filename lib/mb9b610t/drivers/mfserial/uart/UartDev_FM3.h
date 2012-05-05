/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  UART Driver
 */

#ifndef _UARTDEV_FM3_H_
#define _UARTDEV_FM3_H_

/*
 *  FIFO Depth
 */
#define UartDev_CH04_FIFO_DEPTH    16
#define UartDev_CH05_FIFO_DEPTH    16
#define UartDev_CH06_FIFO_DEPTH    16
#define UartDev_CH07_FIFO_DEPTH    16


/*
 *  Driver Status
 */
#define UART_UNINITIALIZED    0
#define UART_INITIALIZED      1


/*
 *  Default Value
 */
#define UART_BAUDRATE    115200               /* Default Baudrate    */
//#define UART_BAUDRATE    9600               /* Default Baudrate    */
#define UART_DATABITS    UartDev_DATABITS_8   /* Default Data bits   */
#define UART_STOPBITS    UartDev_STOPBITS_1   /* Default Stop bits   */
#define UART_PARITY      UartDev_PARITY_NONE  /* Default Parity      */
#define UART_BITODER     UartDev_BITORDER_MSB /* Default BitOrder    */
#define UART_BUFSIZE     128                  /* Default Buffer Size */


/*
 *  Register Bit Definition
 */
#define SMR_SOE          0x01U
#define SMR_BDS          0x04U
#define SMR_SBL          0x08U
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
#define SCR_UPGL         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_FRE          0x10U
#define SSR_PE           0x20U
#define SSR_REC          0x80U

#define ESCR_P           0x08U
#define ESCR_PEN         0x10U
#define ESCR_INV         0x20U
#define ESCR_ESBL        0x40U
#define ESCR_FLWEN       0x80U
#define ESCR_DATABITS_8  0x00U
#define ESCR_DATABITS_5  0x01U
#define ESCR_DATABITS_6  0x02U
#define ESCR_DATABITS_7  0x03U
#define ESCR_DATABITS_9  0x04U

#define BGR_EXT          0x8000U

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
 *  Enable/DISABLE Interrupt Controller
 */
/* deviation from MISRA-C:2004 Rule 19.7 */
#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))


#endif /* _UARTDEV_FM3_H_ */

