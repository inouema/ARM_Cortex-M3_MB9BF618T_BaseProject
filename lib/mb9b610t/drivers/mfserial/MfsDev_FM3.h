/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  MultiFunciton Serial Driver (Channel Control)
 */

#ifndef _MFSDEV_FM3_H_
#define _MFSDEV_FM3_H_


/*
 *  Channel Control Function
 */
int32_t  Mfs_Lock(uint32_t Ch);
void  Mfs_UnLock(uint32_t Ch);
int32_t  Mfs_Open(uint32_t Ch, int32_t Mode);
void  Mfs_Close(uint32_t Ch);


/*
 *  Interrupt Handler
 */
void  MFS0RX_IRQHandler(void);
void  MFS0TX_IRQHandler(void);
void  MFS1RX_IRQHandler(void);
void  MFS1TX_IRQHandler(void);
void  MFS2RX_IRQHandler(void);
void  MFS2TX_IRQHandler(void);
void  MFS3RX_IRQHandler(void);
void  MFS3TX_IRQHandler(void);
void  MFS4RX_IRQHandler(void);
void  MFS4TX_IRQHandler(void);
void  MFS5RX_IRQHandler(void);
void  MFS5TX_IRQHandler(void);
void  MFS6RX_IRQHandler(void);
void  MFS6TX_IRQHandler(void);
void  MFS7RX_IRQHandler(void);
void  MFS7TX_IRQHandler(void);


/*
 *  Extern Interrupt Handling Funtion of Driver
 */
extern void  uart_IrqHandler(uint32_t Ch);
extern void  i2c_IrqHandler(uint32_t Ch);
extern void  sio_IrqHandler(uint32_t Ch);


/*
 *  Register Bit Definition
 */
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U


#endif /* _MFSDEV_FM3_H_ */

