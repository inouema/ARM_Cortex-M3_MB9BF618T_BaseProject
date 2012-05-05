/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  SPI Driver
 */

#ifndef _SPIDEV_H_
#define _SPIDEV_H_


/*
 * Device Defines
 */
#define SpiDev_CLOCK_POLARITY_IDLELOW     1
#define SpiDev_CLOCK_POLARITY_IDLEHIGH    0

#define SpiDev_MODE_MASTER                0
#define SpiDev_MODE_SLAVE                 1

#define SpiDev_BITORDER_MSB               1
#define SpiDev_BITORDER_LSB               0


/*
 * SPI Device Configuration Structure
 */
typedef struct { 
    uint32_t Baudrate;      /* Baudrate      */
    uint32_t Polarity:1;    /* SCLK Polarity */
    uint32_t Phase:1;       /* No support    */
    uint32_t SlaveSelect:1; /* No support    */
    uint32_t Mode:1;        /* M/S Mode      */
    uint32_t BitOrder:1;    /* BitOrder      */
    uint32_t Reserved:27;
} SpiDev_CFG;


/*
 * SPI Device IO Block Structure
 */
typedef struct {
    /* changed by the user application before call to Init. */
    SpiDev_CFG   Cfg;
    /* changed by the user application before call to Init. */
    int32_t  (*Init)(void);
    int32_t  (*UnInit)(void);
    int32_t  (*BufTxRx)(void *pDataTx, void *pDataRx, uint32_t Size);
    int32_t  (*SetBaudrate)(int32_t Baudrate);
    int32_t  (*DataTx)(void *pData, int32_t *pSize);
    int32_t  (*DataRx)(void *pData, int32_t *pSize);
} SpiDev_IOB;


#endif /* _SPIDEV_H_ */
