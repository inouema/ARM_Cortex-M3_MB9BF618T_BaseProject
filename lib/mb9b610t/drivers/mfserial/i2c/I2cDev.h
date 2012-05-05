/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  I2C Driver
 */

#ifndef _I2CDEV_H_
#define _I2CDEV_H_


/*
 * Device Defines
 */
#define I2cDev_ADDRMODE_7                  1
/* 10bit Address Mode is not supported.          */
/* #define I2cDev_ADDRMODE_10                 0  */

#define I2cDev_MODE_MASTER                 1
#define I2cDev_MODE_SLAVE                  0

#define I2cDev_IRQSTATUS_SLAVE_TX          1
#define I2cDev_IRQSTATUS_SLAVE_RX          0


/*
 * I2C Device Configuration Structure
 */
typedef struct {
    uint32_t BaudRate;     /* Baudrate      */
    uint32_t SlaveAddr:10; /* Slave Address */
    uint32_t AddrMode:1;   /* Address Mode  */
    uint32_t Mode:1;       /* M/S Mode      */
    uint32_t Reserved:20;
} I2cDev_CFG;


/*
 * I2C Device IO Block Structure
 */
typedef struct {
    /* changed by the user application. */
    I2cDev_CFG   Cfg;
    
    /* Initialized by I2C driver. */
    int32_t  (*Init)(void);
    int32_t  (*UnInit)(void);
    int32_t  (*DataTxRx)(void *pDataTx, void *pDataRx, uint32_t Size);
    int32_t  (*DataTx)(void *pData, int32_t *pSize);
    int32_t  (*DataRx)(void *pData, int32_t *pSize);
    
    /* changed by the user application before call to Init. */
    void     (*CallBackIrq)(int32_t Status);
} I2cDev_IOB;

#endif /* _I2CDEV_H_ */
