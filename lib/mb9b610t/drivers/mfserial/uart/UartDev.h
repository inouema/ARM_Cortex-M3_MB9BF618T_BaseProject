/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  UART Driver
 */

#ifndef _UARTDEV_H_
#define _UARTDEV_H_


/*
 * Device Defines
 */
#define UartDev_DATABITS_6                 6
#define UartDev_DATABITS_7                 7
#define UartDev_DATABITS_8                 8
/* #define UartDev_DATABITS_9                 9 */
 
#define UartDev_STOPBITS_1                 1
#define UartDev_STOPBITS_2                 2

#define UartDev_PARITY_NONE                0
#define UartDev_PARITY_ODD                 1
#define UartDev_PARITY_EVEN                2

#define UartDev_BITORDER_MSB               1
#define UartDev_BITORDER_LSB               0

#define UartDev_FLAG_BLOCKING              0
#define UartDev_FLAG_NONBLOCKING           1

#define UartDev_NRZ_NORMAL                 0
#define UartDev_NRZ_INV                    1


/*
 * UART Device Configuration Structure
 */
typedef struct {
    uint32_t BaudRate;    /* Baudrate    */
    uint32_t DataBits:4;  /* Data bits   */
    uint32_t StopBits:2;  /* Stop bits   */
    uint32_t Parity:2;    /* Parity      */
    uint32_t BitOrder:1;  /* BitOrder    */
    uint32_t Inv:1;       /* Invert      */
    uint32_t Reserved:20;
    uint32_t BufSize;     /* Buffer Size */
} UartDev_CFG;


/*
 * UART Device IO Block Structure
 */
typedef struct {
    /* changed by the user application before call to Init. */
    UartDev_CFG   Cfg;
    /* Initialized by UART driver. */
    int32_t  (*Init)(void);
    int32_t  (*UnInit)(void);
    int32_t  (*BufTx)(void *pData, int32_t *pSize, uint32_t flags);
    int32_t  (*BufRx)(void *pData, int32_t *pSize, uint32_t flags);
    int32_t  (*BufFlush)(void);
} UartDev_IOB;


#endif /* _UARTDEV_H_ */
