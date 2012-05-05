/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  MultiFunciton Serial Driver (Channel Control)
 */

#include "mcu.h"
#include "MfsDev_FM3.h"


/*
 *  MFS Information Control Struct
 */
typedef struct {
    int32_t            Mode;          /* Mode (0xff = unused) */
//    volatile uint32_t  CritSec;       /* Critical Section     */
    uint32_t  CritSec;       /* Critical Section     */
    void (*IrqHandler)(uint32_t Ch);  /* Driver Irq Handler   */
} Mfs_INFO;


/*
 *  MFS Channel Information
 */
static Mfs_INFO MfsInfo[] = {
   /*{Mode,CritSec,IrqHandler}*/
/*0*/{0xff,      0,      NULL},
/*1*/{0xff,      0,      NULL},
/*2*/{0xff,      0,      NULL},
/*3*/{0xff,      0,      NULL},
/*4*/{0xff,      0,      NULL},
/*5*/{0xff,      0,      NULL},
/*6*/{0xff,      0,      NULL},
/*7*/{0xff,      0,      NULL},
};


/*
 *  Mfs_Lock
 */
int32_t  Mfs_Lock(uint32_t Ch)
{
    Mfs_INFO *p_info;
    uint32_t flag;
    uint32_t dummy;
    int32_t  exc;
    int32_t  ret;
    
    p_info = &MfsInfo[Ch];
    
    flag = __LDREXW(&(p_info->CritSec));
    if (flag == 0) {
        exc = __STREXW(1, &(p_info->CritSec));
        if (exc != 0) {
            do {
                dummy = __LDREXW(&(p_info->CritSec));
                if (dummy == 0) { /* Compiler warning */
                    /* Do nothing */
                }
                exc   = __STREXW(1, &(p_info->CritSec));
            } while (exc != 0);
        }
        
        ret = SUCCESS;
    } else {
        ret = ERROR;
    }
    
    return ret;
}

/*
 *  Mfs_UnLock
 */
void  Mfs_UnLock(uint32_t Ch)
{
    Mfs_INFO *p_info;
    uint32_t dummy;
    int32_t  exc;
    
    p_info = &MfsInfo[Ch];
    
    do {
        dummy = __LDREXW(&(p_info->CritSec));
        if (dummy == 0) { /* Compiler warning */
            /* Do nothing */
        }
        exc   = __STREXW(0, &(p_info->CritSec));
    } while (exc != 0);
}

/*
 *  Mfs_Open
 */
int32_t  Mfs_Open(uint32_t Ch, int32_t Mode)
{
    Mfs_INFO *p_info;
    int32_t  ret;
    
    p_info = &MfsInfo[Ch];
    
    if (p_info->Mode == 0xff) {
        p_info->Mode = Mode;
        switch (p_info->Mode) {
        case SMR_MD_UART:
            p_info->IrqHandler = uart_IrqHandler;
            ret = SUCCESS;
            break;
        case SMR_MD_SIO:
            p_info->IrqHandler = sio_IrqHandler;
            ret = SUCCESS;
            break;
        case SMR_MD_I2C:
            p_info->IrqHandler = i2c_IrqHandler;
            ret = SUCCESS;
            break;
        default:
            ret = ERROR;
        }
    } else {
        ret = ERROR;
    }
    
    return ret;
}

/*
 *  Mfs_Close
 */
void  Mfs_Close(uint32_t Ch)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[Ch];
    p_info->IrqHandler = NULL;
    p_info->Mode = 0xff;
}


/*
 *  Interrupt Handler for Ch0
 */
void  MFS0RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[0];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(0);
    }
}

void  MFS0TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[0];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(0);
    }
}

/*
 *  Interrupt Handler for Ch1
 */
void  MFS1RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[1];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(1);
    }
}

void  MFS1TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[1];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(1);
    }
}

/*
 *  Interrupt Handler for Ch2
 */
void  MFS2RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[2];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(2);
    }
}

void  MFS2TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[2];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(2);
    }
}

/*
 *  Interrupt Handler for Ch3
 */
void  MFS3RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[3];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(3);
    }
}

void  MFS3TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[3];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(3);
    }
}

/*
 *  Interrupt Handler for Ch4
 */
void  MFS4RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[4];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(4);
    }
}

void  MFS4TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[4];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(4);
    }
}

/*
 *  Interrupt Handler for Ch5
 */
void  MFS5RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[5];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(5);
    }
}

void  MFS5TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[5];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(5);
    }
}

/*
 *  Interrupt Handler for Ch6
 */
void  MFS6RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[6];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(6);
    }
}

void  MFS6TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[6];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(6);
    }
}

/*
 *  Interrupt Handler for Ch7
 */
void  MFS7RX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[7];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(7);
    }
}

void  MFS7TX_IRQHandler(void)
{
    Mfs_INFO *p_info;
    
    p_info = &MfsInfo[7];
    if (p_info->IrqHandler != NULL) {
        p_info->IrqHandler(7);
    }
}

