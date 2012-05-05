/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  UART Driver
 */

#include "mcu.h"
#include "UartDev.h"
#include "UartDev_FM3.h"
#include "MfsDev_FM3.h"
#include <stdlib.h>
#include "clock_def.h"

/*
 * AAAAA
 */
//#define USE_UART_MALLOC


/*
 *  Channel Common Function 
 */
static int32_t  UartDev_Init(uint32_t Ch);
static int32_t  UartDev_UnInit(uint32_t Ch);
static int32_t  UartDev_BufTx(uint32_t Ch, void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_BufRx(uint32_t Ch, void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_BufFlush(uint32_t Ch);


/*
 *  Channel Function for Ch0
 */
static int32_t  UartDev_Ch0_Init(void);
static int32_t  UartDev_Ch0_UnInit(void);
static int32_t  UartDev_Ch0_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch0_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch0_BufFlush(void);

/*
 *  Channel Function for Ch1
 */
static int32_t  UartDev_Ch1_Init(void);
static int32_t  UartDev_Ch1_UnInit(void);
static int32_t  UartDev_Ch1_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch1_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch1_BufFlush(void);

/*
 *  Channel Function for Ch2
 */
static int32_t  UartDev_Ch2_Init(void);
static int32_t  UartDev_Ch2_UnInit(void);
static int32_t  UartDev_Ch2_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch2_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch2_BufFlush(void);

/*
 *  Channel Function for Ch3
 */
static int32_t  UartDev_Ch3_Init(void);
static int32_t  UartDev_Ch3_UnInit(void);
static int32_t  UartDev_Ch3_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch3_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch3_BufFlush(void);

/*
 *  Channel Function for Ch4
 */
static int32_t  UartDev_Ch4_Init(void);
static int32_t  UartDev_Ch4_UnInit(void);
static int32_t  UartDev_Ch4_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch4_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch4_BufFlush(void);

/*
 *  Channel Function for Ch5
 */
static int32_t  UartDev_Ch5_Init(void);
static int32_t  UartDev_Ch5_UnInit(void);
static int32_t  UartDev_Ch5_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch5_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch5_BufFlush(void);

/*
 *  Channel Function for Ch6
 */
static int32_t  UartDev_Ch6_Init(void);
static int32_t  UartDev_Ch6_UnInit(void);
static int32_t  UartDev_Ch6_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch6_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch6_BufFlush(void);

/*
 *  Channel Function for Ch7
 */
static int32_t  UartDev_Ch7_Init(void);
static int32_t  UartDev_Ch7_UnInit(void);
static int32_t  UartDev_Ch7_BufTx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch7_BufRx(void *pData, int32_t *pSize, uint32_t flags);
static int32_t  UartDev_Ch7_BufFlush(void);


/*
 *  IOB Channel Information
 */
/* deviation from MISRA-C:2004 Rule 8.8 */
UartDev_IOB UartDev[] = {
  /* {{     BaudRate,      DataBits,      StopBits,      Parity,     BitOrder,                Inv,Reserved,BufSize},
                  Init,             UnInit,             BufTx,             BufRx,             BufFlush}, */
/*0*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch0_Init, UartDev_Ch0_UnInit, UartDev_Ch0_BufTx, UartDev_Ch0_BufRx, UartDev_Ch0_BufFlush},

/*1*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch1_Init, UartDev_Ch1_UnInit, UartDev_Ch1_BufTx, UartDev_Ch1_BufRx, UartDev_Ch1_BufFlush},

/*2*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch2_Init, UartDev_Ch2_UnInit, UartDev_Ch2_BufTx, UartDev_Ch2_BufRx, UartDev_Ch2_BufFlush},

/*3*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch3_Init, UartDev_Ch3_UnInit, UartDev_Ch3_BufTx, UartDev_Ch3_BufRx, UartDev_Ch3_BufFlush},

/*4*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch4_Init, UartDev_Ch4_UnInit, UartDev_Ch4_BufTx, UartDev_Ch4_BufRx, UartDev_Ch4_BufFlush},

/*5*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch5_Init, UartDev_Ch5_UnInit, UartDev_Ch5_BufTx, UartDev_Ch5_BufRx, UartDev_Ch5_BufFlush},

/*6*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch6_Init, UartDev_Ch6_UnInit, UartDev_Ch6_BufTx, UartDev_Ch6_BufRx, UartDev_Ch6_BufFlush},

/*7*/{{UART_BAUDRATE, UART_DATABITS, UART_STOPBITS, UART_PARITY, UART_BITODER, UartDev_NRZ_NORMAL, 0, UART_BUFSIZE},
      UartDev_Ch7_Init, UartDev_Ch7_UnInit, UartDev_Ch7_BufTx, UartDev_Ch7_BufRx, UartDev_Ch7_BufFlush},
};


/*
 *  Buffer Information Struct
 */
typedef struct {
   uint32_t  WIdx; /* Write Index (to Buffer)         */
   uint32_t  RIdx; /* Read  Index (from Buffer)       */
   uint32_t  Mask;          /* Buffer Mask Size ( BufSize - 1) */
   uint8_t   *pBuf;         /* Buffer                          */
} Uart_BUF;

/*
 *  Driver Information Control Struct
 */
typedef struct {
    int32_t      State;     /* State            */
    int32_t      RxSuspend; /* Rx Suspend Flag  */
    int32_t      Fifo;      /* FIFO Depth       */
#ifdef USE_CH03
    FM3_MFS03_UART_TypeDef *pReg;   /* Register Struct  */
#else
    FM3_MFS47_UART_TypeDef *pReg;   /* Register Struct  */
#endif
    UartDev_IOB  *pIOB;     /* IOB Struct       */
    Uart_BUF     *pRxBuf;   /* RX Buffer Struct */
    Uart_BUF     *pTxBuf;   /* TX Buffer Struct */
    IRQn_Type    RxIRQn;    /* RX IRQ Number    */
    IRQn_Type    TxIRQn;    /* TX IRQ Number    */
} Uart_INFO;



/*
 *  RX Buffer Channel Information
 */
static Uart_BUF UartRxBuf[] = {
   /*{WIdx,RIdx,Mask, pBuf}*/
/*0*/{   0,   0,   0, NULL},
/*1*/{   0,   0,   0, NULL},
/*2*/{   0,   0,   0, NULL},
/*3*/{   0,   0,   0, NULL},
/*4*/{   0,   0,   0, NULL},
/*5*/{   0,   0,   0, NULL},
/*6*/{   0,   0,   0, NULL},
/*7*/{   0,   0,   0, NULL},
};

/*
 *  TX Buffer Channel Information
 */
static Uart_BUF UartTxBuf[] = {
   /*{WIdx,RIdx,Mask, pBuf}*/
/*0*/{   0,   0,   0, NULL},
/*1*/{   0,   0,   0, NULL},
/*2*/{   0,   0,   0, NULL},
/*3*/{   0,   0,   0, NULL},
/*4*/{   0,   0,   0, NULL},
/*5*/{   0,   0,   0, NULL},
/*6*/{   0,   0,   0, NULL},
/*7*/{   0,   0,   0, NULL},
};

/*
 *  Driver Channel Information
 */
/* pReg : deviation from MISRA-C:2004 Rule 11.3 */
static Uart_INFO UartInfo[] = {
   /*{             State,RxSuspend,             Fifo,  pReg,        pIOB,        pRxBuf,        pTxBuf,      RxIRQn,      TxIRQn}*/
#ifdef USE_CH03
/*0*/{UART_UNINITIALIZED, 0,                       0, FM3_MFS0_UART, &UartDev[0], &UartRxBuf[0], &UartTxBuf[0], MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{UART_UNINITIALIZED, 0,                       0, FM3_MFS1_UART, &UartDev[1], &UartRxBuf[1], &UartTxBuf[1], MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{UART_UNINITIALIZED, 0,                       0, FM3_MFS2_UART, &UartDev[2], &UartRxBuf[2], &UartTxBuf[2], MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{UART_UNINITIALIZED, 0,                       0, FM3_MFS3_UART, &UartDev[3], &UartRxBuf[3], &UartTxBuf[3], MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{UART_UNINITIALIZED, 0, UartDev_CH04_FIFO_DEPTH,             0, &UartDev[4], &UartRxBuf[4], &UartTxBuf[4], MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{UART_UNINITIALIZED, 0, UartDev_CH05_FIFO_DEPTH,             0, &UartDev[5], &UartRxBuf[5], &UartTxBuf[5], MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{UART_UNINITIALIZED, 0, UartDev_CH06_FIFO_DEPTH,             0, &UartDev[6], &UartRxBuf[6], &UartTxBuf[6], MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{UART_UNINITIALIZED, 0, UartDev_CH07_FIFO_DEPTH,             0, &UartDev[7], &UartRxBuf[7], &UartTxBuf[7], MFS7RX_IRQn, MFS7TX_IRQn},
#else
/*0*/{UART_UNINITIALIZED, 0,                       0,             0, &UartDev[0], &UartRxBuf[0], &UartTxBuf[0], MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{UART_UNINITIALIZED, 0,                       0,             0, &UartDev[1], &UartRxBuf[1], &UartTxBuf[1], MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{UART_UNINITIALIZED, 0,                       0,             0, &UartDev[2], &UartRxBuf[2], &UartTxBuf[2], MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{UART_UNINITIALIZED, 0,                       0,             0, &UartDev[3], &UartRxBuf[3], &UartTxBuf[3], MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{UART_UNINITIALIZED, 0, UartDev_CH04_FIFO_DEPTH, FM3_MFS4_UART, &UartDev[4], &UartRxBuf[4], &UartTxBuf[4], MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{UART_UNINITIALIZED, 0, UartDev_CH05_FIFO_DEPTH, FM3_MFS5_UART, &UartDev[5], &UartRxBuf[5], &UartTxBuf[5], MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{UART_UNINITIALIZED, 0, UartDev_CH06_FIFO_DEPTH, FM3_MFS6_UART, &UartDev[6], &UartRxBuf[6], &UartTxBuf[6], MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{UART_UNINITIALIZED, 0, UartDev_CH07_FIFO_DEPTH, FM3_MFS7_UART, &UartDev[7], &UartRxBuf[7], &UartTxBuf[7], MFS7RX_IRQn, MFS7TX_IRQn},
#endif
};


/*
 *  Internal Function
 */
static int32_t  uart_Init(Uart_INFO *pInfo);
static void  uart_UnInit(uint32_t Ch, Uart_INFO *pInfo);
static void  uart_StartRx(Uart_INFO *pInfo);
static void  uart_DataRx(Uart_INFO *pInfo);
static void  uart_StopRx(Uart_INFO *pInfo);
static void  uart_StartTx(Uart_INFO *pInfo);
static void  uart_DataTx(Uart_INFO *pInfo);
static void  uart_StopTx(Uart_INFO *pInfo);


/*
 *  Internal Function for Buffer Handling
 */
static uint32_t  atomic_inc(uint32_t *pVar);
static uint8_t*  uart_malloc_buffer(int32_t Size);
static void  uart_free_buffer(uint8_t* pBuf);

/* deviation from MISRA-C:2004 Rule 19.7 */
#define UART_BUF_RESET(buf)         ((buf)->RIdx = (buf)->WIdx = 0)
#define UART_BUF_WRITE(buf, data)   ((buf)->pBuf[(buf)->Mask & atomic_inc(&((buf)->WIdx))] = (data))
#define UART_BUF_READ(buf)          ((buf)->pBuf[(buf)->Mask & atomic_inc(&((buf)->RIdx))])
#define UART_BUF_EMPTY(buf)         (((buf)->Mask & (buf)->RIdx) == ((buf)->Mask & (buf)->WIdx))
#define UART_BUF_FULL(buf)          (((buf)->Mask & (buf)->RIdx) == ((buf)->Mask & ((buf)->WIdx + 1)))
#define UART_BUF_COUNT(buf)         ((buf)->Mask & ((buf)->WIdx - (buf)->RIdx))

static uint32_t  atomic_inc(uint32_t *pVar)
{
    uint32_t var;
    int32_t  exc;
    
    do {
        var = __LDREXW(pVar);
        exc = __STREXW((var+1), pVar);
    } while (exc != 0);
    
    return *pVar;
}

static uint8_t*  uart_malloc_buffer(int32_t Size)
{
    /* deviation from MISRA-C:2004 Rule 20.4 */
    return (uint8_t*)malloc(Size);
}

static void  uart_free_buffer(uint8_t* pBuf)
{
    /* deviation from MISRA-C:2004 Rule 20.4 */
    free(pBuf);
}


/*
 *  UartDev_Init
 */
static int32_t  UartDev_Init(uint32_t Ch)
{
    Uart_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
        p_info = &UartInfo[Ch];
        
        if (Mfs_Open(Ch, SMR_MD_UART) == ERROR) {
            ret = ERROR; /* open error */
        } else {
        /* When Mfs_Open is SUCCESS, p_info->State is UART_UNINITIALIZED. */

            /* initialize hardware */
            ret = uart_Init(p_info);

            if (ret == SUCCESS) {
                /* enable NVIC */
                UART_ENABLE_IRQ(p_info->RxIRQn);
                UART_ENABLE_IRQ(p_info->TxIRQn);
                /* change state */
                p_info->State = UART_INITIALIZED;

                uart_StartRx(p_info);

                ret = SUCCESS;

            } else {
                Mfs_Close(Ch);
                ret = ERROR; /* uart_Init error */
            }
        }
        
        Mfs_UnLock(Ch);
    } else {
        ret = ERROR; /* Mfs_Lock error */
    }
    
    return ret;
}


/*
 *  UartDev_UnInit
 */
static int32_t  UartDev_UnInit(uint32_t Ch)
{
    Uart_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
    
        p_info = &UartInfo[Ch];
        if (p_info->State != UART_INITIALIZED) {
            ret = ERROR; /* not initialized */
            
        } else {
            uart_StopRx(p_info);
            
            /* change state */
            p_info->State = UART_UNINITIALIZED;
            
            /* disable NVIC */
            UART_DISABLE_IRQ(p_info->RxIRQn);
            UART_DISABLE_IRQ(p_info->TxIRQn);
            
            /* uninitialize */
            uart_UnInit(Ch, p_info);
            
            Mfs_Close(Ch);
            
            ret = SUCCESS;
        }
        
        Mfs_UnLock(Ch);
        
    } else {
        ret = ERROR; /* Mfs_Lock error */
    }
    
    return ret;
}


/*
 *  UartDev_BufTx
 */
static int32_t  UartDev_BufTx(uint32_t Ch, void *pData, int32_t *pSize, uint32_t flags)
{
    Uart_INFO *p_info;
    int32_t ret;
    int32_t count;
    uint8_t *p_char;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &UartInfo[Ch];
    
    if (p_info->State != UART_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        count  = 0;
        p_char = (uint8_t *)pData;
        
        if (flags == UartDev_FLAG_BLOCKING) {
        
            while (count < *pSize) {
                /* Block if tx buffer is full. */
                while (UART_BUF_FULL(p_info->pTxBuf)) { /* true */
                    /* sleep */
                }
                
                do {
                    /* Tx buffer is not full. */
                    
                    if (count < *pSize) {
                        UART_BUF_WRITE(p_info->pTxBuf, p_char[count]);
                        count += 1;
                    } else {
                        /* Writing in tx buffer was completed. */
                        break;
                    }
                } while (!UART_BUF_FULL(p_info->pTxBuf)); /* false */
                
                /* start tx : interruption occur */
                uart_StartTx(p_info);
            }
            while (!UART_BUF_EMPTY(p_info->pTxBuf)){
                continue;
            }
        } else { /* flags == UartDev_FLAG_NONBLOCKING */
            
            while (!UART_BUF_FULL(p_info->pTxBuf)) { /* false */
                /* Tx buffer is not full */
                
                if (count < *pSize) {
                    UART_BUF_WRITE(p_info->pTxBuf, p_char[count]);
                    count += 1;
                } else {
                    /* Writing in tx buffer was completed. */
                    break;
                }
            }
            
            /* Tx buffer was full or writing in tx buffer was completed. */
            
            /* return size */
            *pSize = count;
            
            /* start tx : interruption occur */
            uart_StartTx(p_info);
        }
        
        ret = SUCCESS;
    }
    
    return ret;
}


/*
 *  UartDev_BufRx
 */
static int32_t  UartDev_BufRx(uint32_t Ch, void *pData, int32_t *pSize, uint32_t flags)
{
    Uart_INFO *p_info;
    int32_t ret;
    int32_t count;
    uint8_t *p_char;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &UartInfo[Ch];
    
    if (p_info->State != UART_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        count  = 0;
        p_char = (uint8_t *)pData;
        
        if (flags == UartDev_FLAG_BLOCKING) {
        
            while (count < *pSize) {
                /* Block if rx buffer is empty. */
                while (UART_BUF_EMPTY(p_info->pRxBuf)) { /* true */
                    /* sleep */
                }
                
                do {
                    /* Rx buffer is not empty. */
                    
                    if (count < *pSize) {
                        p_char[count] = UART_BUF_READ(p_info->pRxBuf);
                        count += 1;
                    } else {
                        /* Reading from rx buffer was completed. */
                        break;
                    }
                } while (!UART_BUF_EMPTY(p_info->pRxBuf)); /* false */
                
                /* check suspend */
                if (p_info->RxSuspend == 1) {
                    /* Resume rx */
                    p_info->RxSuspend = 0;
                    uart_StartRx(p_info);
                }
                
            }
        } else { /* flags == UartDev_FLAG_NONBLOCKING */
        
            while (!UART_BUF_EMPTY(p_info->pRxBuf)) { /* false */
                /* Rx buffer is not empty. */
                
                if (count < *pSize) {
                    p_char[count] = UART_BUF_READ(p_info->pRxBuf);
                    count += 1;
                } else {
                    /* Reading from rx buffer was completed. */
                    break;
                }
            }
            
            /* Rx buffer was empty or reading from rx buffer was completed. */
            
            /* return size */
            *pSize = count;
            
            /* check suspend */
            if (p_info->RxSuspend == 1) {
                /* Resume rx */
                p_info->RxSuspend = 0;
                uart_StartRx(p_info);
            }
        }
        
        ret = SUCCESS;
    }
    
    return ret;
}


/*
 *  UartDev_BufFlush
 */
static int32_t  UartDev_BufFlush(uint32_t Ch)
{
    Uart_INFO *p_info;
    int32_t ret;
    
    p_info = &UartInfo[Ch];
    
    if (p_info->State != UART_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        uart_StopRx(p_info);
        
        /* flush buffer */
        UART_BUF_RESET(p_info->pRxBuf);
        UART_BUF_RESET(p_info->pTxBuf);
        
        uart_StartRx(p_info);
        
        ret = SUCCESS;
    }
    
    return ret;
}


/*
 *  uart_Init (Internal Function)
 */

#ifdef USE_UART_MALLOC
#else
static uint8_t f_uart_tx_buf[512];
static uint8_t f_uart_rx_buf[512];
#endif

static int32_t  uart_Init(Uart_INFO *pInfo)
{
    UartDev_CFG *p_cfg;
    int32_t ret;
    uint16_t reg_bgr;
    uint8_t reg_smr    = 0;
    uint8_t reg_escr   = 0;
#ifndef USE_CH03
    uint8_t reg_fcr0   = 0;
    uint8_t reg_fcr1   = 0;
    uint8_t reg_fbyte1 = 0;
    uint8_t reg_fbyte2 = 0;
#endif
    uint8_t reg_scr    = 0;
    
    p_cfg = &(pInfo->pIOB->Cfg);
    
    /* get memory for buffer */
#ifdef USE_UART_MALLOC
    pInfo->pRxBuf->pBuf = uart_malloc_buffer(p_cfg->BufSize);
    pInfo->pTxBuf->pBuf = uart_malloc_buffer(p_cfg->BufSize);
#else
    pInfo->pRxBuf->pBuf = f_uart_rx_buf;
    pInfo->pTxBuf->pBuf = f_uart_tx_buf;
#endif

    if ((pInfo->pRxBuf->pBuf != NULL) && (pInfo->pTxBuf->pBuf != NULL)) {
        
        /* initialize buffer info */
        UART_BUF_RESET(pInfo->pRxBuf);
        pInfo->pRxBuf->Mask = (uint32_t)(p_cfg->BufSize - 1);
        UART_BUF_RESET(pInfo->pTxBuf);
        pInfo->pTxBuf->Mask = (uint32_t)(p_cfg->BufSize - 1);
        
        
        /*
         *  Set value of SMR register
         */
        /* Mode = UART,  serial output enable */
        reg_smr |= (SMR_MD_UART | SMR_SOE);
        
        /* BitOrder */
        if (p_cfg->BitOrder != UartDev_BITORDER_LSB) {
            /* MSB first */
            reg_smr |= (SMR_BDS);
            
        } /* Do nothing if LSB first */
        
        /* StopBits */
        if (p_cfg->StopBits == UartDev_STOPBITS_2) {
            /* StopBits 2 */
            reg_smr |= (SMR_SBL);
        } /* Do nothing if StopBits 1 */
        
        
        /*
         *  Set value of BGR register
         */
        /* BaudRate : round off reg_bgr to the whole number */
        reg_bgr = (uint16_t)(((SysFrePCLK2 + ((uint32_t)p_cfg->BaudRate / 2)) / (uint32_t)p_cfg->BaudRate) - 1);
        
        /*
         *  Set value of ESCR register
         */
        /* DataBits */
        switch (p_cfg->DataBits) {
        case UartDev_DATABITS_6:
            reg_escr |= (ESCR_DATABITS_6);
            break;
        case UartDev_DATABITS_7:
            reg_escr |= (ESCR_DATABITS_7);
            break;
        default: /* UartDev_DATABITS_8 */
            reg_escr |= (ESCR_DATABITS_8);
        }
        
        /* Parity */
        switch (p_cfg->Parity) {
        case UartDev_PARITY_ODD:
            reg_escr |= (ESCR_PEN);
            break;
        case UartDev_PARITY_EVEN:
            reg_escr |= (ESCR_PEN | ESCR_P);
            break;
        default: /* UartDev_PARITY_NONE */
            /* Do nothing if Parity none */
            reg_escr |= 0; /* for MISRA-C */
        }
        
        /* Invert */
        if (UartDev_NRZ_INV == p_cfg->Inv) {
            reg_escr |= (ESCR_INV);
        }
        
        if (pInfo->Fifo > 0) { /* HW FIFO is used */
            /*
             *  Set value of FCR1 register
             */
            /* data lost enable, rx FIFO idle ebale */
            /* FCR1_FDRQ : don't care               */
#ifndef USE_CH03  
            reg_fcr1 |= (FCR1_FLSTE | FCR1_FRIIE | FCR1_FDRQ);
            /*
             *  Set value of FBYTE1/RBYTE2 registers
             */
            /* FIFO1 is tx */
            reg_fbyte1 = 0;
            /* FIFO2 is rx */
            reg_fbyte2 = (uint8_t)(pInfo->Fifo);
            
            /*
             *  Set value of FCR0 register
             */
            /* FIFO reset, FIFO enable */
            reg_fcr0 |= (FCR0_FCL2 | FCR0_FCL1 | FCR0_FE2 | FCR0_FE1);
#endif             
        } /* Do nothing if HW FIFO is not used */
        
        
        /*
         *  Set value of SCR register
         */
        /* rx enable, tx enable */
        reg_scr |= (SCR_RXE | SCR_TXE);
        
        
        /*
         *  Register writing
         */
        /* set mode */
        pInfo->pReg->SMR    = reg_smr;
        
        /* All disable before other registers are set. */
        pInfo->pReg->SCR    = 0;
        
        pInfo->pReg->BGR    = reg_bgr;
        pInfo->pReg->ESCR   = reg_escr;
        
        /* FIFO disable before other FIFO registers are set.*/
#ifndef USE_CH03
        pInfo->pReg->FCR0   = FCR1_FDRQ;
        
        pInfo->pReg->FCR1   = reg_fcr1;
        pInfo->pReg->FBYTE1 = reg_fbyte1;
        pInfo->pReg->FBYTE2 = reg_fbyte2;
        pInfo->pReg->FCR0   = reg_fcr0;
#endif        
        pInfo->pReg->SCR = reg_scr;
        
        ret = SUCCESS;
        
    } else {
        /* No memory for buffer */
        ret = ERROR;
    }

    return ret;
}

/*
 *  uart_UnInit (Internal Function)
 */
static void  uart_UnInit(uint32_t Ch, Uart_INFO *pInfo)
{
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fcr0;
#endif
    /* rx interrupt disable, tx interrupt disable, tx bus idle interrupt disable */
    /* rx disable, tx disable                                                    */
    reg_scr = pInfo->pReg->SCR & ~(SCR_RIE | SCR_TIE | SCR_TBIE | SCR_RXE | SCR_TXE);
    pInfo->pReg->SCR = reg_scr;

    /* tx FIFO disable, rx FIFO disable */
#ifndef USE_CH03
    reg_fcr0 = pInfo->pReg->FCR0 & ~(FCR0_FE2 | FCR0_FE1);
    pInfo->pReg->FCR0 = reg_fcr0;
#endif
    /* software rest */
    reg_scr = pInfo->pReg->SCR | (SCR_UPGL);
    pInfo->pReg->SCR = reg_scr;

    /* uninitialize Uart_INFO struct */
    pInfo->RxSuspend = 0;

    /* uninitialize tx/rx buffer */
    pInfo->pRxBuf->WIdx = 0;
    pInfo->pRxBuf->RIdx = 0;
    pInfo->pRxBuf->Mask = 0;
    if (pInfo->pRxBuf->pBuf != NULL) {
#ifdef USE_UART_MALLOC
        uart_free_buffer(pInfo->pRxBuf->pBuf);
#else
        ;
#endif
    }
    pInfo->pTxBuf->WIdx = 0;
    pInfo->pTxBuf->RIdx = 0;
    pInfo->pTxBuf->Mask = 0;
    if (pInfo->pTxBuf->pBuf != NULL) {
#ifdef USE_UART_MALLOC
        uart_free_buffer(pInfo->pTxBuf->pBuf);
#else
        ;
#endif
    }

    /* uninitialize FIFO Depth */
    switch (Ch) {
    case 4:
        pInfo->Fifo = UartDev_CH04_FIFO_DEPTH;
        break;
    case 5:
        pInfo->Fifo = UartDev_CH05_FIFO_DEPTH;
        break;
    case 6:
        pInfo->Fifo = UartDev_CH06_FIFO_DEPTH;
        break;
    case 7:
        pInfo->Fifo = UartDev_CH07_FIFO_DEPTH;
        break;
    default:
        pInfo->Fifo = 0;
    }
    
    /* uninitialize Config */
    pInfo->pIOB->Cfg.BaudRate = UART_BAUDRATE;
    pInfo->pIOB->Cfg.DataBits = UART_DATABITS;
    pInfo->pIOB->Cfg.StopBits = UART_STOPBITS;
    pInfo->pIOB->Cfg.Parity   = UART_PARITY;
    pInfo->pIOB->Cfg.BitOrder = UART_BITODER;
    pInfo->pIOB->Cfg.Reserved = 0;
    pInfo->pIOB->Cfg.BufSize  = UART_BUFSIZE;
    
    return;
}

/*
 *  uart_StartRx (Internal Function)
 */
static void  uart_StartRx(Uart_INFO *pInfo)
{
    uint8_t reg_scr;
    
    /* rx interrupt enable */
    reg_scr = pInfo->pReg->SCR | (SCR_RIE);
    pInfo->pReg->SCR = reg_scr;
    
    return;
}

/*
 *  uart_StopRx (Internal Function)
 */
static void  uart_StopRx(Uart_INFO *pInfo)
{
    uint8_t reg_scr;
    
    /* rx interrupt disable */
    reg_scr = pInfo->pReg->SCR & ~(SCR_RIE);
    pInfo->pReg->SCR = reg_scr;
    
    return;
}

/*
 *  uart_StartTx (Internal Function)
 */
static void  uart_StartTx(Uart_INFO *pInfo)
{
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif    
    if (pInfo->Fifo > 0) { /* HW FIFO is used */
        
        /* tx FIFO interrupt enable */
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1 | (FCR1_FTIE);
        pInfo->pReg->FCR1 = reg_fcr1;
#endif        
    } else { /* HW FIFO is not used */
        
        /* tx interrupt enalbe */
        reg_scr = pInfo->pReg->SCR | (SCR_TIE);
        pInfo->pReg->SCR = reg_scr;
        
    }
    
    return;
}

/*
 *  uart_StopTx (Internal Function)
 */
static void  uart_StopTx(Uart_INFO *pInfo)
{
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif    
    if (pInfo->Fifo > 0) { /* HW FIFO is used */
        
        /* tx FIFO interrupt disable */
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1 & ~(FCR1_FTIE);
        pInfo->pReg->FCR1 = reg_fcr1;
#endif   
    } else { /* HW FIFO is not used */
        
        /* tx interrupt disable */
        reg_scr = pInfo->pReg->SCR & ~(SCR_TIE);
        pInfo->pReg->SCR = reg_scr;
        
    }
    
    return;
}

/*
 *  uart_DataRx (Internal Function)
 */
static void  uart_DataRx(Uart_INFO *pInfo)
{
    uint16_t reg_data;
    uint8_t  reg_ssr;
    
    reg_ssr = pInfo->pReg->SSR;
    
    if ((reg_ssr & (SSR_RDRF)) != 0) {
        
        while (!UART_BUF_FULL(pInfo->pRxBuf)) { /* false */
            /* Rx buffer is not full */
            
            reg_ssr = pInfo->pReg->SSR;
            if ((reg_ssr & (SSR_RDRF)) != 0) {
                /* rx data from the register(or FIFO) to rx buffer */
                reg_data = pInfo->pReg->RDR;
                UART_BUF_WRITE(pInfo->pRxBuf, (uint8_t)reg_data);
            } else {
                /* No data */
                break;
            }
        }
        
        if (UART_BUF_FULL(pInfo->pRxBuf)) {
            /* Rx buffer is full */
            
            /* Suspend rx */
            pInfo->RxSuspend = 1;
            uart_StopRx(pInfo);
        }
        
    } else { /* overrun error, framing error, parity error */
        /* clear rx error interrupt */
        reg_ssr |= SSR_REC;
        pInfo->pReg->SSR = reg_ssr;
        
    }
    
    return;
}

/*
 *  uart_DataTx (Internal Function)
 */
static void  uart_DataTx(Uart_INFO *pInfo)
{
    uint32_t count = 0;
    uint8_t reg_ssr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    reg_ssr  = pInfo->pReg->SSR;
#ifndef USE_CH03
    reg_fcr1 = pInfo->pReg->FCR1;

    if (((reg_ssr & (SSR_TDRE)) != 0) || ((reg_fcr1 & FCR1_FDRQ) != 0)) {
#else
    if ((reg_ssr & (SSR_TDRE)) != 0) {
#endif
        if (pInfo->Fifo > 0) { /* HW FIFO is used */
            
            while (!UART_BUF_EMPTY(pInfo->pTxBuf)) { /* false */
                /* Tx buffer is not empty. */
                if (count < (uint32_t)pInfo->Fifo) {
                    /* tx data from tx buffer to tx FIFO */
                    pInfo->pReg->TDR = UART_BUF_READ(pInfo->pTxBuf);
                    count++;
                } else {
                    /* Tx FIFO is full. */
                    break;
                }
            }
            
            /* clear FIFO tx interrupt */
#ifndef USE_CH03
            reg_fcr1 = reg_fcr1 & ~(FCR1_FDRQ);
            pInfo->pReg->FCR1 = reg_fcr1;
#endif
        } else { /* HW FIFO is not used */
            
            if (!UART_BUF_EMPTY(pInfo->pTxBuf)) { /* false */
                /* tx data from tx buffer to the register */
                pInfo->pReg->TDR = UART_BUF_READ(pInfo->pTxBuf);
            }
        }
        
        if (UART_BUF_EMPTY(pInfo->pTxBuf)) {
            /* Tx buffer is empty. */
            uart_StopTx(pInfo);
        }
        
    } else {
        /* illegal interrupt */
    }
    
    return;
}

/*
 *  uart_IrqHandler (Interrupt Handling Funtion)
 */
void  uart_IrqHandler(uint32_t Ch)
{
    Uart_INFO *p_info;
    uint8_t reg_ssr;
    
    p_info = &UartInfo[Ch];
    
    reg_ssr = p_info->pReg->SSR;
    if ((reg_ssr & (SSR_RDRF | SSR_ORE | SSR_FRE | SSR_PE)) != 0) { /* true */
        /* rx interrupt */
        uart_DataRx(p_info);
        
    } else {
        /* tx interrupt */
        uart_DataTx(p_info);
        
    }
    
    return;
}


/*
 *  Channel Function for Ch0
 */
static int32_t  UartDev_Ch0_Init(void)
{
    return UartDev_Init(0);
}

static int32_t  UartDev_Ch0_UnInit(void)
{
    return UartDev_UnInit(0);
}

static int32_t  UartDev_Ch0_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(0, pData, pSize, flags);
}

static int32_t  UartDev_Ch0_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(0, pData, pSize, flags);
}

static int32_t  UartDev_Ch0_BufFlush(void)
{
    return UartDev_BufFlush(0);
}


/*
 *  Channel Function for Ch1
 */
static int32_t  UartDev_Ch1_Init(void)
{
    return UartDev_Init(1);
}

static int32_t  UartDev_Ch1_UnInit(void)
{
    return UartDev_UnInit(1);
}

static int32_t  UartDev_Ch1_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(1, pData, pSize, flags);
}

static int32_t  UartDev_Ch1_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(1, pData, pSize, flags);
}

static int32_t  UartDev_Ch1_BufFlush(void)
{
    return UartDev_BufFlush(1);
}


/*
 *  Channel Function for Ch2
 */
static int32_t  UartDev_Ch2_Init(void)
{
    return UartDev_Init(2);
}

static int32_t  UartDev_Ch2_UnInit(void)
{
    return UartDev_UnInit(2);
}

static int32_t  UartDev_Ch2_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(2, pData, pSize, flags);
}

static int32_t  UartDev_Ch2_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(2, pData, pSize, flags);
}

static int32_t  UartDev_Ch2_BufFlush(void)
{
    return UartDev_BufFlush(2);
}


/*
 *  Channel Function for Ch3
 */
static int32_t  UartDev_Ch3_Init(void)
{
    return UartDev_Init(3);
}

static int32_t  UartDev_Ch3_UnInit(void)
{
    return UartDev_UnInit(3);
}

static int32_t  UartDev_Ch3_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(3, pData, pSize, flags);
}

static int32_t  UartDev_Ch3_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(3, pData, pSize, flags);
}

static int32_t  UartDev_Ch3_BufFlush(void)
{
    return UartDev_BufFlush(3);
}


/*
 *  Channel Function for Ch4
 */
static int32_t  UartDev_Ch4_Init(void)
{
    return UartDev_Init(4);
}

static int32_t  UartDev_Ch4_UnInit(void)
{
    return UartDev_UnInit(4);
}

static int32_t  UartDev_Ch4_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(4, pData, pSize, flags);
}

static int32_t  UartDev_Ch4_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(4, pData, pSize, flags);
}

static int32_t  UartDev_Ch4_BufFlush(void)
{
    return UartDev_BufFlush(4);
}


/*
 *  Channel Function for Ch5
 */
static int32_t  UartDev_Ch5_Init(void)
{
    return UartDev_Init(5);
}

static int32_t  UartDev_Ch5_UnInit(void)
{
    return UartDev_UnInit(5);
}

static int32_t  UartDev_Ch5_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(5, pData, pSize, flags);
}

static int32_t  UartDev_Ch5_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(5, pData, pSize, flags);
}

static int32_t  UartDev_Ch5_BufFlush(void)
{
    return UartDev_BufFlush(5);
}


/*
 *  Channel Function for Ch6
 */
static int32_t  UartDev_Ch6_Init(void)
{
    return UartDev_Init(6);
}

static int32_t  UartDev_Ch6_UnInit(void)
{
    return UartDev_UnInit(6);
}

static int32_t  UartDev_Ch6_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(6, pData, pSize, flags);
}

static int32_t  UartDev_Ch6_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(6, pData, pSize, flags);
}

static int32_t  UartDev_Ch6_BufFlush(void)
{
    return UartDev_BufFlush(6);
}


/*
 *  Channel Function for Ch7
 */
static int32_t  UartDev_Ch7_Init(void)
{
    return UartDev_Init(7);
}

static int32_t  UartDev_Ch7_UnInit(void)
{
    return UartDev_UnInit(7);
}

static int32_t  UartDev_Ch7_BufTx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufTx(7, pData, pSize, flags);
}

static int32_t  UartDev_Ch7_BufRx(void *pData, int32_t *pSize, uint32_t flags)
{
    return UartDev_BufRx(7, pData, pSize, flags);
}

static int32_t  UartDev_Ch7_BufFlush(void)
{
    return UartDev_BufFlush(7);
}


