/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  SIO Driver
 */

#ifndef _SIODEV_H_
#define _SIODEV_H_


/*
 * Device Defines
 */
#define SioDev_DATABITS_6                 6
#define SioDev_DATABITS_7                 7
#define SioDev_DATABITS_8                 8
/* #define SioDev_DATABITS_9                 9 */

#define SioDev_MODE_MASTER                0
#define SioDev_MODE_SLAVE                 1

#define SioDev_SCKPOLARITY_HIGH           0
#define SioDev_SCKPOLARITY_LOW            1

#define SioDev_BITORDER_MSB               1
#define SioDev_BITORDER_LSB               0

#define SioDev_IRQSTATUS_RX               0

/*
 * SIO Device Configuration Structure
 */
typedef struct { 
    uint32_t BaudRate;     /* Baudrate      */
    uint32_t DataBits:4;   /* Data bits     */
    uint32_t Mode:1;       /* M/S Mode      */
    uint32_t SckPolarity:1;/* SCLK Polarity */
    uint32_t BitOrder:1;   /* BitOrder      */
    uint32_t Reserved:25;
} SioDev_CFG;


/*
 * SIO Device IO Block Structure
 */
typedef struct {
    /* changed by the user application before call to Init. */
    SioDev_CFG   Cfg;
    
    /* Initialized by SIO driver. */
    int32_t  (*Init)(void);
    int32_t  (*UnInit)(void);
    int32_t  (*DataTx)(void *pData, int32_t *pSize);
    int32_t  (*DataRx)(void *pData, int32_t *pSize);
    
    /* changed by the user application before call to Init. */
    void     (*CallBackIrq)(int32_t Status);
} SioDev_IOB;


#endif /* _SIODEV_H_ */
