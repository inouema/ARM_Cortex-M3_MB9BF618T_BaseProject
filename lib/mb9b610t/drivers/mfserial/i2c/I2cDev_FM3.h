/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  I2C Driver
 */

#ifndef _I2CDEV_FM3_H_
#define _I2CDEV_FM3_H_

/*
 *  FIFO Depth
 */
#define I2cDev_CH04_FIFO_DEPTH    16
#define I2cDev_CH05_FIFO_DEPTH    16
#define I2cDev_CH06_FIFO_DEPTH    16
#define I2cDev_CH07_FIFO_DEPTH    16


/*
 *  Driver Status
 */
#define I2C_UNINITIALIZED    0
#define I2C_INITIALIZED      1


/*
 *  Default Value
 */
#define I2C_BAUDRATE     400000               /* Default Baudrate      */
#define I2C_SLAVEADDR    0x00                 /* Default Slave Address */
#define I2C_ADDRMODE     I2cDev_ADDRMODE_7    /* Default Address Mode  */
#define I2C_MODE         I2cDev_MODE_MASTER   /* Default Mode          */


/*
 *  Register Bit Definition
 */
#define SMR_TIE          0x04U
#define SMR_RIE          0x08U
#define SMR_WUCR         0x10U
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U

#define IBCR_INT         0x01U
#define IBCR_BER         0x02U
#define IBCR_INTE        0x04U
#define IBCR_CNDE        0x08U
#define IBCR_WSEL        0x10U
#define IBCR_ACKE        0x20U
#define IBCR_ACT         0x40U
#define IBCR_SCC         0x40U
#define IBCR_MSS         0x80U

#define IBSR_BB          0x01U
#define IBSR_SPC         0x02U
#define IBSR_RSC         0x04U
#define IBSR_AL          0x08U
#define IBSR_TRX         0x10U
#define IBSR_RSA         0x20U
#define IBSR_RACK        0x40U
#define IBSR_FBT         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_TBIE         0x10U
#define SSR_DMA          0x20U
#define SSR_TSET         0x40U
#define SSR_REC          0x80U

#define ISMK_EN          0x80U

#define ISBA_SAEN        0x80U

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
#define I2C_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define I2C_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))


#endif /* _I2CDEV_FM3_H_ */

