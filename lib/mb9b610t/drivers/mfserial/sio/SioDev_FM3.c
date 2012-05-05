/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  SIO Driver
 */

#include "mcu.h"
#include "SioDev.h"
#include "SioDev_FM3.h"
#include "MfsDev_FM3.h"
#include "clock_def.h"

/*
 *  Channel Common Function 
 */
static int32_t  SioDev_Init(uint32_t Ch);
static int32_t  SioDev_UnInit(uint32_t Ch);
static int32_t  SioDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize);
static int32_t  SioDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch0
 */
static int32_t  SioDev_Ch0_Init(void);
static int32_t  SioDev_Ch0_UnInit(void);
static int32_t  SioDev_Ch0_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch0_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch1
 */
static int32_t  SioDev_Ch1_Init(void);
static int32_t  SioDev_Ch1_UnInit(void);
static int32_t  SioDev_Ch1_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch1_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch2
 */
static int32_t  SioDev_Ch2_Init(void);
static int32_t  SioDev_Ch2_UnInit(void);
static int32_t  SioDev_Ch2_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch2_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch3
 */
static int32_t  SioDev_Ch3_Init(void);
static int32_t  SioDev_Ch3_UnInit(void);
static int32_t  SioDev_Ch3_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch3_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch4
 */
static int32_t  SioDev_Ch4_Init(void);
static int32_t  SioDev_Ch4_UnInit(void);
static int32_t  SioDev_Ch4_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch4_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch5
 */
static int32_t  SioDev_Ch5_Init(void);
static int32_t  SioDev_Ch5_UnInit(void);
static int32_t  SioDev_Ch5_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch5_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch6
 */
static int32_t  SioDev_Ch6_Init(void);
static int32_t  SioDev_Ch6_UnInit(void);
static int32_t  SioDev_Ch6_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch6_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch7
 */
static int32_t  SioDev_Ch7_Init(void);
static int32_t  SioDev_Ch7_UnInit(void);
static int32_t  SioDev_Ch7_DataTx(void *pData, int32_t *pSize);
static int32_t  SioDev_Ch7_DataRx(void *pData, int32_t *pSize);


/*
 *  IOB Channel Information
 */
/* deviation from MISRA-C:2004 Rule 8.8 */
SioDev_IOB SioDev[] = {
  /* {{    BaudRate,     DataBits,     Mode,     SckPolarity,    BitOrder,Reserved},
                 Init,            UnInit,            DataTx,            DataRx, CallBackIrq}, */
/*0*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch0_Init, SioDev_Ch0_UnInit, SioDev_Ch0_DataTx, SioDev_Ch0_DataRx, NULL},

/*1*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch1_Init, SioDev_Ch1_UnInit, SioDev_Ch1_DataTx, SioDev_Ch1_DataRx, NULL},

/*2*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch2_Init, SioDev_Ch2_UnInit, SioDev_Ch2_DataTx, SioDev_Ch2_DataRx, NULL},

/*3*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch3_Init, SioDev_Ch3_UnInit, SioDev_Ch3_DataTx, SioDev_Ch3_DataRx, NULL},

/*4*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch4_Init, SioDev_Ch4_UnInit, SioDev_Ch4_DataTx, SioDev_Ch4_DataRx, NULL},

/*5*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch5_Init, SioDev_Ch5_UnInit, SioDev_Ch5_DataTx, SioDev_Ch5_DataRx, NULL},

/*6*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch6_Init, SioDev_Ch6_UnInit, SioDev_Ch6_DataTx, SioDev_Ch6_DataRx, NULL},

/*7*/{{SIO_BAUDRATE, SIO_DATABITS, SIO_MODE, SIO_SCKPOLARITY, SIO_BITODER, 0},
      SioDev_Ch7_Init, SioDev_Ch7_UnInit, SioDev_Ch7_DataTx, SioDev_Ch7_DataRx, NULL},
};


/*
 *  Data Information Struct
 */
typedef struct {
    uint32_t  Size;
    uint32_t  Count;
    uint32_t  IntState;
    uint8_t   *pData;
} Sio_DATA;


/*
 *  Driver Information Control Struct
 */
typedef struct {
    int32_t      State;      /* State            */
    int32_t      Fifo;       /* FIFO Depth       */
#ifdef USE_CH03
    FM3_MFS03_CSIO_TypeDef *pReg;    /* Register Struct  */
#else
    FM3_MFS47_CSIO_TypeDef *pReg;    /* Register Struct  */
#endif
    SioDev_IOB   *pIOB;      /* IOB Struct       */
    Sio_DATA     *pDataRx;   /* Data Info Struct */
    Sio_DATA     *pDataTx;   /* Data Info Struct */
    uint32_t     RxFlag;     /* Flag for Rx      */
    IRQn_Type    RxIRQn;     /* RX IRQ Number    */
    IRQn_Type    TxIRQn;     /* TX IRQ Number    */
} Sio_INFO;


/*
 *  DataRx Channel Information
 */
static Sio_DATA SioDataRx[] = {
   /*{Size,Count,IntState,pData}*/
/*0*/{   0,    0,       0, NULL},
/*1*/{   0,    0,       0, NULL},
/*2*/{   0,    0,       0, NULL},
/*3*/{   0,    0,       0, NULL},
/*4*/{   0,    0,       0, NULL},
/*5*/{   0,    0,       0, NULL},
/*6*/{   0,    0,       0, NULL},
/*7*/{   0,    0,       0, NULL},
};

/*
 *  DataTx Channel Information
 */
static Sio_DATA SioDataTx[] = {
   /*{Size,Count,IntState,pData}*/
/*0*/{   0,    0,       0, NULL},
/*1*/{   0,    0,       0, NULL},
/*2*/{   0,    0,       0, NULL},
/*3*/{   0,    0,       0, NULL},
/*4*/{   0,    0,       0, NULL},
/*5*/{   0,    0,       0, NULL},
/*6*/{   0,    0,       0, NULL},
/*7*/{   0,    0,       0, NULL},
};

/*
 *  Driver Chanel Information
 */
/* pReg : deviation from MISRA-C:2004 Rule 11.3 */
static Sio_INFO SioInfo[] = {
   /*{            State,                   Fifo,          pReg,       pIOB,       pDataRx,       pDataTx,RxFlag,  RxIRQn,      TxIRQn}*/
#ifdef USE_CH03
/*0*/{SIO_UNINITIALIZED,                      0, FM3_MFS0_CSIO, &SioDev[0], &SioDataRx[0], &SioDataTx[0], 0, MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{SIO_UNINITIALIZED,                      0, FM3_MFS1_CSIO, &SioDev[1], &SioDataRx[1], &SioDataTx[1], 0, MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{SIO_UNINITIALIZED,                      0, FM3_MFS2_CSIO, &SioDev[2], &SioDataRx[2], &SioDataTx[2], 0, MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{SIO_UNINITIALIZED,                      0, FM3_MFS3_CSIO, &SioDev[3], &SioDataRx[3], &SioDataTx[3], 0, MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{SIO_UNINITIALIZED, SioDev_CH04_FIFO_DEPTH,             0, &SioDev[4], &SioDataRx[4], &SioDataTx[4], 0, MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{SIO_UNINITIALIZED, SioDev_CH05_FIFO_DEPTH,             0, &SioDev[5], &SioDataRx[5], &SioDataTx[5], 0, MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{SIO_UNINITIALIZED, SioDev_CH06_FIFO_DEPTH,             0, &SioDev[6], &SioDataRx[6], &SioDataTx[6], 0, MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{SIO_UNINITIALIZED, SioDev_CH07_FIFO_DEPTH,             0, &SioDev[7], &SioDataRx[7], &SioDataTx[7], 0, MFS7RX_IRQn, MFS7TX_IRQn},
#else
/*0*/{SIO_UNINITIALIZED,                      0,             0, &SioDev[0], &SioDataRx[0], &SioDataTx[0], 0, MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{SIO_UNINITIALIZED,                      0,             0, &SioDev[1], &SioDataRx[1], &SioDataTx[1], 0, MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{SIO_UNINITIALIZED,                      0,             0, &SioDev[2], &SioDataRx[2], &SioDataTx[2], 0, MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{SIO_UNINITIALIZED,                      0,             0, &SioDev[3], &SioDataRx[3], &SioDataTx[3], 0, MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{SIO_UNINITIALIZED, SioDev_CH04_FIFO_DEPTH, FM3_MFS4_CSIO, &SioDev[4], &SioDataRx[4], &SioDataTx[4], 0, MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{SIO_UNINITIALIZED, SioDev_CH05_FIFO_DEPTH, FM3_MFS5_CSIO, &SioDev[5], &SioDataRx[5], &SioDataTx[5], 0, MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{SIO_UNINITIALIZED, SioDev_CH06_FIFO_DEPTH, FM3_MFS6_CSIO, &SioDev[6], &SioDataRx[6], &SioDataTx[6], 0, MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{SIO_UNINITIALIZED, SioDev_CH07_FIFO_DEPTH, FM3_MFS7_CSIO, &SioDev[7], &SioDataRx[7], &SioDataTx[7], 0, MFS7RX_IRQn, MFS7TX_IRQn},
#endif
};


/*
 *  RxFlag
 */
#define SIO_RESUME        1


/*
 *  Internal Function
 */
static int32_t  sio_Init(Sio_INFO *pInfo);
static void  sio_UnInit(uint32_t Ch, Sio_INFO *pInfo);

static void  sio_StartRx(Sio_INFO *pInfo);
static void  sio_SuspendRx(const Sio_INFO *pInfo);
static void  sio_ResumeRx(Sio_INFO *pInfo);
static void  sio_DataRx(Sio_INFO *pInfo);
static void  sio_StopRx(Sio_INFO *pInfo);

static void  sio_StartTx(Sio_INFO *pInfo);
static void  sio_DataTx(Sio_INFO *pInfo);
static void  sio_StopTx(Sio_INFO *pInfo);


/*
 *  SioDev_Init
 */
static int32_t  SioDev_Init(uint32_t Ch)
{
    Sio_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
        p_info = &SioInfo[Ch];
        
        if (Mfs_Open(Ch, SMR_MD_SIO) == ERROR) {
            ret = ERROR; /* open error */
            
        } else {
        /* When Mfs_Open is SUCCESS, p_info->State is SIO_UNINITIALIZED. */
        
            /* initialize hardware */
            ret = sio_Init(p_info);
            
            if (ret == SUCCESS) {
                /* enable NVIC */
                SIO_ENABLE_IRQ(p_info->RxIRQn);
                SIO_ENABLE_IRQ(p_info->TxIRQn);
                
                /* change state */
                p_info->State = SIO_INITIALIZED;
                
                if (p_info->pIOB->Cfg.Mode != SioDev_MODE_MASTER) { /* Slave */
                    sio_StartRx(p_info);
                }
                
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
 *  SioDev_UnInit
 */
static int32_t  SioDev_UnInit(uint32_t Ch)
{
    Sio_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
    
        p_info = &SioInfo[Ch];
        if (p_info->State != SIO_INITIALIZED) {
            ret = ERROR; /* not initialized */
            
        } else {
            sio_StopRx(p_info);
            
            /* change state */
            p_info->State = SIO_UNINITIALIZED;
            
            /* disable NVIC */
            SIO_DISABLE_IRQ(p_info->RxIRQn);
            SIO_DISABLE_IRQ(p_info->TxIRQn);
            
            /* uninitialize */
            sio_UnInit(Ch, p_info);
            
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
 *  SioDev_DataTx
 */
static int32_t  SioDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize)
{
    Sio_INFO *p_info;
    int32_t ret;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &SioInfo[Ch];
    
    if (p_info->State != SIO_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        /* initialized data information */
        p_info->pDataTx->Size     = (uint32_t)(*pSize);
        p_info->pDataTx->Count    = 0;
        p_info->pDataTx->IntState = 0;
        p_info->pDataTx->pData    = pData;
        
        sio_StartTx(p_info);
        
        /* wait until tx is completed or error occur */
        while ((p_info->pDataTx->Size != p_info->pDataTx->Count) &&
               (p_info->pDataTx->IntState == 0)) {
            /* sleep */
        }
        
        /* check Count */
        if (p_info->pDataTx->Count != 0) {
            /* return size */
            *pSize = (int32_t)(p_info->pDataTx->Count);
            ret = SUCCESS;
            
        } else { /* Count == 0 */
            *pSize = 0;
            ret = ERROR;
            
        }
        
    }
    
    return ret;
}

/*
 *  SioDev_DataRx
 */
static int32_t  SioDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize)
{
    Sio_INFO *p_info;
    int32_t ret;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &SioInfo[Ch];
    
    if (p_info->State != SIO_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        /* initialized data information */
        p_info->pDataRx->Size     = (uint32_t)(*pSize);
        p_info->pDataRx->Count    = 0;
        p_info->pDataRx->IntState = 0;
        p_info->pDataRx->pData    = pData;
        
        if (p_info->pIOB->Cfg.Mode == SioDev_MODE_MASTER) { /* Master */
            /* start rx */
            sio_StartRx(p_info);
            
        } else {  /* Slave */
            /* set RxFlag */
            p_info->RxFlag = SIO_RESUME;
            
            /* resume rx : pair of "suspend rx No.1" */
            sio_ResumeRx(p_info);
            
        }
        
        /* wait until rx is completed or error occur */
        while ((p_info->pDataRx->Size != p_info->pDataRx->Count) &&
               (p_info->pDataRx->IntState == 0)) {
            /* sleep */
        }
        
        /* initialized RxFlag */
        p_info->RxFlag = 0;
        
        if (p_info->pIOB->Cfg.Mode == SioDev_MODE_MASTER) { /* Master */
            /* start rx */
            sio_StopRx(p_info);
            
        } else {  /* Slave*/
            /* resume rx : pair of "suspend rx No.2" */
            sio_ResumeRx(p_info);
            
        }
        
        
        /* check Count */
        if (p_info->pDataRx->Count != 0) {
            /* return size */
            *pSize = (int32_t)(p_info->pDataRx->Count);
            ret = SUCCESS;
            
        } else { /* Count == 0 */
            *pSize = 0;
            ret = ERROR;
            
        }
    }
    
    return ret;
}


/*
 *  sio_Init (Internal Function)
 */
static int32_t  sio_Init(Sio_INFO *pInfo)
{
    SioDev_CFG *p_cfg;
    int32_t ret;
    uint16_t reg_bgr;
    uint8_t reg_smr    = 0;
    uint8_t reg_scr    = 0;
    uint8_t reg_escr   = 0;
#ifndef USE_CH03
    uint8_t reg_fcr0   = 0;
    uint8_t reg_fcr1   = 0;
    uint8_t reg_fbyte1 = 0;
    uint8_t reg_fbyte2 = 0;
#endif
    p_cfg = &(pInfo->pIOB->Cfg);
    
    /*
     *  Set value of SMR register
     */
    /* Mode = CSIO */
    reg_smr |= (SMR_MD_SIO);
    
    /* Serial Clock Output */
    if (p_cfg->Mode == SioDev_MODE_MASTER) {
        /* serial clock enable */
        reg_smr |= (SMR_SCKE);
    }
    
    /* SCLK Polarity */
    if (p_cfg->SckPolarity == SioDev_SCKPOLARITY_LOW) {
        /* Low */
        reg_smr |= (SMR_SCINV);
    } /* Do nothing if High */
    
    /* BitOrder */
    if (p_cfg->BitOrder != SioDev_BITORDER_LSB) {
        /* MSB first */
        reg_smr |= (SMR_BDS);
        
    } /* Do nothing if LSB first */
    
    
    /*
     *  Set value of BGR register
     */
    /* BaudRate : round off reg_bgr to the whole number */
    reg_bgr = (uint16_t)(((SysFrePCLK2 + ((uint32_t)p_cfg->BaudRate / 2)) / (uint32_t)p_cfg->BaudRate) - 1);
    
    
    /*
     *  Set value of SCR register
     */
    /* Tx enable */
    reg_scr |= (SCR_TXE);
    
    /* Master / Slave */
    if (p_cfg->Mode == SioDev_MODE_SLAVE) {
        /* Slave */
        reg_scr |= (SCR_MS);
    }
    
    /* SPI */
    if (p_cfg->Reserved == 1) {
        /* Slave */
        reg_scr |= (SCR_SPI);
    }
    
    
    /*
     *  Set value of ESCR register
     */
    /* DataBits */
    switch (p_cfg->DataBits) {
    case SioDev_DATABITS_6:
        reg_escr |= (ESCR_DATABITS_6);
        break;
    case SioDev_DATABITS_7:
        reg_escr |= (ESCR_DATABITS_7);
        break;
    default: /* SioDev_DATABITS_8 */
        reg_escr |= (ESCR_DATABITS_8);
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
     *  Register writing
     */
    pInfo->pReg->SMR    = reg_smr;
    pInfo->pReg->SCR    = reg_scr;
    
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
    ret = SUCCESS;
    
    return ret;
}


/*
 *  sio_UnInit (Internal Function)
 */
static void  sio_UnInit(uint32_t Ch, Sio_INFO *pInfo)
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
    
    /* uninitialize data information */
    pInfo->pDataRx->Size     = 0;
    pInfo->pDataRx->Count    = 0;
    pInfo->pDataRx->IntState = 0;
    pInfo->pDataRx->pData    = NULL;
    
    pInfo->pDataTx->Size     = 0;
    pInfo->pDataTx->Count    = 0;
    pInfo->pDataTx->IntState = 0;
    pInfo->pDataTx->pData    = NULL;
    
    /* uninitialize RxFlag */
    pInfo->RxFlag = 0;
    
    /* uninitialize FIFO Depth */
    switch (Ch) {
    case 4:
        pInfo->Fifo = SioDev_CH04_FIFO_DEPTH;
        break;
    case 5:
        pInfo->Fifo = SioDev_CH05_FIFO_DEPTH;
        break;
    case 6:
        pInfo->Fifo = SioDev_CH06_FIFO_DEPTH;
        break;
    case 7:
        pInfo->Fifo = SioDev_CH07_FIFO_DEPTH;
        break;
    default:
        pInfo->Fifo = 0;
    }
    
    /* uninitialize Config */
    pInfo->pIOB->Cfg.BaudRate    = SIO_BAUDRATE;
    pInfo->pIOB->Cfg.DataBits    = SIO_DATABITS;
    pInfo->pIOB->Cfg.Mode        = SIO_MODE;
    pInfo->pIOB->Cfg.SckPolarity = SIO_SCKPOLARITY;
    pInfo->pIOB->Cfg.BitOrder    = SIO_BITODER;
    pInfo->pIOB->Cfg.Reserved    = 0;
    
    /* uninitialize CallBackIrq */
    pInfo->pIOB->CallBackIrq = NULL;
    
    return;
}


/*
 *  sio_StartRx (Internal Function)
 */
static void  sio_StartRx(Sio_INFO *pInfo)
{
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fbyte1;
    uint8_t reg_fcr1;
#endif
    /* rx interrupt enable, rx enable */
    reg_scr = pInfo->pReg->SCR | (SCR_RIE | SCR_RXE);
    pInfo->pReg->SCR = reg_scr;
    
    /* start serial clock if Master */
    if (pInfo->pIOB->Cfg.Mode == SioDev_MODE_MASTER) {
        if (pInfo->Fifo > 0) { /* HW FIFO is used */
            /* set tx FIFO */
#ifndef USE_CH03
            if (pInfo->pDataRx->Size >= (uint32_t)pInfo->Fifo) {
                reg_fbyte1 = (uint8_t)(pInfo->Fifo);
            } else {
                reg_fbyte1 = (uint8_t)(pInfo->pDataRx->Size);
            }
            pInfo->pReg->FBYTE1 = reg_fbyte1;
            
            /* FDRQ = 0 */
            reg_fcr1 = pInfo->pReg->FCR1 & ~(FCR1_FDRQ);
            pInfo->pReg->FCR1 = reg_fcr1;
#endif            
        } else { /* HW FIFO is not used */
            
            /* write dummy data(0x00) */
            pInfo->pReg->TDR = 0x00;
            
        }
    }
    
    return;
}


/*
 *  sio_SuspendRx (Internal Function)
 */
static void  sio_SuspendRx(const Sio_INFO *pInfo)
{
    uint8_t reg_scr;
    
    /* rx interrupt disable */
    reg_scr = pInfo->pReg->SCR & ~(SCR_RIE);
    pInfo->pReg->SCR = reg_scr;
    
    if ((pInfo->pDataRx->Count == 0) && (pInfo->RxFlag == 0)) {
        /* call CallBackIrq */
        if (pInfo->pIOB->CallBackIrq != NULL) {
            pInfo->pIOB->CallBackIrq(SioDev_IRQSTATUS_RX);
        }
    }
    
    return;
}


/*
 *  sio_ResumeRx (Internal Function)
 */
static void  sio_ResumeRx(Sio_INFO *pInfo)
{
    uint8_t reg_ssr;
    uint8_t reg_scr;
    
    reg_ssr = pInfo->pReg->SSR;
    
    if ((reg_ssr & (SSR_RDRF)) != 0) {
        /* rx interrupt enable */
        reg_scr = pInfo->pReg->SCR | (SCR_RIE);
        pInfo->pReg->SCR = reg_scr;
        
    } else { /* No data */
        if (pInfo->RxFlag == 0) {
            pInfo->pDataRx->IntState = 1; /* 1 means error */
        }
    }
    
    return;
}


/*
 *  sio_DataRx (Internal Function)
 */
static void  sio_DataRx(Sio_INFO *pInfo)
{
    uint8_t  reg_ssr;
#ifndef USE_CH03
    uint8_t  reg_fbyte1;
    uint8_t  reg_fcr1;
#endif
    reg_ssr = pInfo->pReg->SSR;
    
    if ((reg_ssr & (SSR_RDRF)) != 0) {
        if (pInfo->pIOB->Cfg.Mode == SioDev_MODE_MASTER) { /* Master */
            
            if (pInfo->Fifo > 0) { /* HW FIFO is used */
                while (pInfo->pDataRx->Count < pInfo->pDataRx->Size) {
                    reg_ssr = pInfo->pReg->SSR;
                    if ((reg_ssr & (SSR_RDRF)) != 0) {
                        /* rx data from the FIFO */
                        pInfo->pDataRx->pData[pInfo->pDataRx->Count] = (uint8_t)pInfo->pReg->RDR;
                        pInfo->pDataRx->Count += 1;
                    } else {
                        /* No data */
                        break;
                    }
                }
                
                /* check last data */
                if (pInfo->pDataRx->Count == pInfo->pDataRx->Size) {
                    sio_StopRx(pInfo);
                    
                } else { /* prepare for next data */
                    /* set tx FIFO */
#ifndef USE_CH03
                    if ((pInfo->pDataRx->Size - pInfo->pDataRx->Count) >= (uint32_t)pInfo->Fifo) {
                        reg_fbyte1 = (uint8_t)(pInfo->Fifo);
                    } else {
                        reg_fbyte1 = (uint8_t)(pInfo->pDataRx->Size - pInfo->pDataRx->Count);
                    }
                    pInfo->pReg->FBYTE1 = reg_fbyte1;
                    
                    /* FDRQ = 0 */
                    reg_fcr1 = pInfo->pReg->FCR1 & ~(FCR1_FDRQ);
                    pInfo->pReg->FCR1 = reg_fcr1;
#endif
                }
                
            } else { /* HW FIFO is not used */
                /* rx data from the register */
                pInfo->pDataRx->pData[pInfo->pDataRx->Count] = (uint8_t)pInfo->pReg->RDR;
                pInfo->pDataRx->Count += 1;
                
                /* check last data */
                if (pInfo->pDataRx->Count == pInfo->pDataRx->Size) {
                    sio_StopRx(pInfo);
                    
                } else { /* prepare for next data */
                    /* write dummy data(0x00) */
                    pInfo->pReg->TDR = 0x00;
                    
                }
            }
            
        } else { /* Slave */
            
            if ((pInfo->pDataRx->Count == 0) && (pInfo->RxFlag == 0)) {
                /* suspend rx : No.1 */
                sio_SuspendRx(pInfo);
                
            } else {
                
                if (pInfo->Fifo > 0) { /* HW FIFO is used */
                    while (pInfo->pDataRx->Count < pInfo->pDataRx->Size) {
                        
                        reg_ssr = pInfo->pReg->SSR;
                        if ((reg_ssr & (SSR_RDRF)) != 0) {
                            /* rx data from the register(or FIFO) */
                            pInfo->pDataRx->pData[pInfo->pDataRx->Count] = (uint8_t)pInfo->pReg->RDR;
                            pInfo->pDataRx->Count += 1;
                            
                        } else {
                            /* end of data */
                            pInfo->pDataRx->IntState = 2; /* 2 means end of data */
                            break;
                        }
                    }
                } else {
                    if (pInfo->pDataRx->Count < pInfo->pDataRx->Size) {
                        reg_ssr = pInfo->pReg->SSR;
                        if ((reg_ssr & (SSR_RDRF)) != 0) {
                            /* rx data from the register(or FIFO) */
                            pInfo->pDataRx->pData[pInfo->pDataRx->Count] = (uint8_t)pInfo->pReg->RDR;
                            pInfo->pDataRx->Count += 1;
                            
                        } else {
                            /* end of data */
                            pInfo->pDataRx->IntState = 2; /* 2 means end of data */
                        }
                    }
                }
                
                if (pInfo->pDataRx->Count >= pInfo->pDataRx->Size) {
                    sio_SuspendRx(pInfo);
                }
            }
            
        }
        
    } else { /* overrun error */
        /* clear rx error interrupt */
        reg_ssr |= SSR_REC;
        pInfo->pReg->SSR = reg_ssr;
        
    }
    
    return;
}


/*
 *  sio_StopRx (Internal Function)
 */
static void  sio_StopRx(Sio_INFO *pInfo)
{
    uint8_t reg_scr;
    
    /* rx interrupt disable, rx disable */
    reg_scr = pInfo->pReg->SCR & ~(SCR_RIE | SCR_RXE);
    pInfo->pReg->SCR = reg_scr;
    
    return;
}


/*
 *  sio_StartTx (Internal Function)
 */
static void  sio_StartTx(Sio_INFO *pInfo)
{
    uint8_t reg_smr;
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif    
    /* serial output enable */
    reg_smr = pInfo->pReg->SMR | (SMR_SOE);
    pInfo->pReg->SMR = reg_smr;
    
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
 *  sio_DataTx (Internal Function)
 */
static void  sio_DataTx(Sio_INFO *pInfo)
{
    uint32_t i;
    uint32_t num;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    if (pInfo->Fifo > 0) { /* HW FIFO is used */
        num = pInfo->pDataTx->Size - pInfo->pDataTx->Count;
        if (num > (uint32_t)pInfo->Fifo) {
            num = (uint32_t)pInfo->Fifo;
        }
        
        for (i = 0; i < num; i++) {
            /* tx data to tx FIFO */
            pInfo->pReg->TDR = pInfo->pDataTx->pData[pInfo->pDataTx->Count];
            pInfo->pDataTx->Count += 1;
        }
        
        /* clear FIFO tx interrupt */
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1 & ~(FCR1_FDRQ);
        pInfo->pReg->FCR1 = reg_fcr1;
#endif
    } else { /* HW FIFO is not used */
        if (pInfo->pDataTx->Count < pInfo->pDataTx->Size) {
            /* tx data to register */
            pInfo->pReg->TDR = pInfo->pDataTx->pData[pInfo->pDataTx->Count];
            pInfo->pDataTx->Count += 1;
            
        }
        
    }
    
    if (pInfo->pDataTx->Count >= pInfo->pDataTx->Size) {
        /* tx is completed */
        sio_StopTx(pInfo);
    }
    
    return;
}


/*
 *  sio_StopTx (Internal Function)
 */
static void  sio_StopTx(Sio_INFO *pInfo)
{
    uint8_t reg_ssr;
    uint8_t reg_smr;
    uint8_t reg_scr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    /* wait until tx is completed */
    while(1) {
        reg_ssr = pInfo->pReg->SSR;
        if (((reg_ssr & (SSR_TDRE)) != 0) &&  ((reg_ssr & (SSR_TBI)) != 0)) {
            break;
        }
    }
    
    
    /* serial output disable */
    reg_smr = pInfo->pReg->SMR & ~(SMR_SOE);
    pInfo->pReg->SMR = reg_smr;
    
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
 *  sio_IrqHandler (Interrupt Handling Funtion)
 */
void  sio_IrqHandler(uint32_t Ch)
{
    Sio_INFO *p_info;
    uint8_t reg_scr;
    uint8_t reg_ssr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    p_info = &SioInfo[Ch];
    
    reg_scr  = p_info->pReg->SCR;
    reg_ssr  = p_info->pReg->SSR;
#ifndef USE_CH03
    reg_fcr1 = p_info->pReg->FCR1;
#endif
    if (((reg_scr & (SCR_RIE)) != 0) &&
        ((reg_ssr & (SSR_RDRF | SSR_ORE)) != 0)) {
        /* rx interrupt */
        sio_DataRx(p_info);
#ifndef USE_CH03        
    } else if ((((reg_scr & (SCR_TIE)) != 0) && ((reg_ssr & (SSR_TDRE)) != 0)) ||
               (((reg_fcr1 & (FCR1_FTIE)) != 0) && ((reg_fcr1 & (FCR1_FDRQ)) != 0))){
#else
    } else if (((reg_scr & (SCR_TIE)) != 0) && ((reg_ssr & (SSR_TDRE)) != 0)) {                 
#endif
        /* tx interrupt */
        sio_DataTx(p_info);
        
    } else {
        /* Do nothing */
    }
    
    return;
}


/*
 *  Channel Function for Ch0
 */
static int32_t  SioDev_Ch0_Init(void)
{
    return SioDev_Init(0);
}

static int32_t  SioDev_Ch0_UnInit(void)
{
    return SioDev_UnInit(0);
}

static int32_t  SioDev_Ch0_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(0, pData, pSize);
}

static int32_t  SioDev_Ch0_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(0, pData, pSize);
}


/*
 *  Channel Function for Ch1
 */
static int32_t  SioDev_Ch1_Init(void)
{
    return SioDev_Init(1);
}

static int32_t  SioDev_Ch1_UnInit(void)
{
    return SioDev_UnInit(1);
}

static int32_t  SioDev_Ch1_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(1, pData, pSize);
}

static int32_t  SioDev_Ch1_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(1, pData, pSize);
}


/*
 *  Channel Function for Ch2
 */
static int32_t  SioDev_Ch2_Init(void)
{
    return SioDev_Init(2);
}

static int32_t  SioDev_Ch2_UnInit(void)
{
    return SioDev_UnInit(2);
}

static int32_t  SioDev_Ch2_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(2, pData, pSize);
}

static int32_t  SioDev_Ch2_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(2, pData, pSize);
}


/*
 *  Channel Function for Ch3
 */
static int32_t  SioDev_Ch3_Init(void)
{
    return SioDev_Init(3);
}

static int32_t  SioDev_Ch3_UnInit(void)
{
    return SioDev_UnInit(3);
}

static int32_t  SioDev_Ch3_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(3, pData, pSize);
}

static int32_t  SioDev_Ch3_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(3, pData, pSize);
}


/*
 *  Channel Function for Ch4
 */
static int32_t  SioDev_Ch4_Init(void)
{
    return SioDev_Init(4);
}

static int32_t  SioDev_Ch4_UnInit(void)
{
    return SioDev_UnInit(4);
}

static int32_t  SioDev_Ch4_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(4, pData, pSize);
}

static int32_t  SioDev_Ch4_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(4, pData, pSize);
}


/*
 *  Channel Function for Ch5
 */
static int32_t  SioDev_Ch5_Init(void)
{
    return SioDev_Init(5);
}

static int32_t  SioDev_Ch5_UnInit(void)
{
    return SioDev_UnInit(5);
}

static int32_t  SioDev_Ch5_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(5, pData, pSize);
}

static int32_t  SioDev_Ch5_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(5, pData, pSize);
}


/*
 *  Channel Function for Ch6
 */
static int32_t  SioDev_Ch6_Init(void)
{
    return SioDev_Init(6);
}

static int32_t  SioDev_Ch6_UnInit(void)
{
    return SioDev_UnInit(6);
}

static int32_t  SioDev_Ch6_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(6, pData, pSize);
}

static int32_t  SioDev_Ch6_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(6, pData, pSize);
}


/*
 *  Channel Function for Ch7
 */
static int32_t  SioDev_Ch7_Init(void)
{
    return SioDev_Init(7);
}

static int32_t  SioDev_Ch7_UnInit(void)
{
    return SioDev_UnInit(7);
}

static int32_t  SioDev_Ch7_DataTx(void *pData, int32_t *pSize)
{
    return SioDev_DataTx(7, pData, pSize);
}

static int32_t  SioDev_Ch7_DataRx(void *pData, int32_t *pSize)
{
    return SioDev_DataRx(7, pData, pSize);
}

