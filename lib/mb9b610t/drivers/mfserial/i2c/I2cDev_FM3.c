/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  I2C Driver
 */

#include "mcu.h"
#include "I2cDev.h"
#include "I2cDev_FM3.h"
#include "MfsDev_FM3.h"
#include "clock_def.h"

/*
 *  Channel Common Function 
 */
static int32_t  I2cDev_Init(uint32_t Ch);
static int32_t  I2cDev_UnInit(uint32_t Ch);
static int32_t  I2cDev_DataTxRx(uint32_t Ch, void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize);
static int32_t  I2cDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch0
 */
static int32_t  I2cDev_Ch0_Init(void);
static int32_t  I2cDev_Ch0_UnInit(void);
static int32_t  I2cDev_Ch0_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch0_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch0_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch1
 */
static int32_t  I2cDev_Ch1_Init(void);
static int32_t  I2cDev_Ch1_UnInit(void);
static int32_t  I2cDev_Ch1_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch1_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch1_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch2
 */
static int32_t  I2cDev_Ch2_Init(void);
static int32_t  I2cDev_Ch2_UnInit(void);
static int32_t  I2cDev_Ch2_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch2_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch2_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch3
 */
static int32_t  I2cDev_Ch3_Init(void);
static int32_t  I2cDev_Ch3_UnInit(void);
static int32_t  I2cDev_Ch3_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch3_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch3_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch4
 */
static int32_t  I2cDev_Ch4_Init(void);
static int32_t  I2cDev_Ch4_UnInit(void);
static int32_t  I2cDev_Ch4_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch4_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch4_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch5
 */
static int32_t  I2cDev_Ch5_Init(void);
static int32_t  I2cDev_Ch5_UnInit(void);
static int32_t  I2cDev_Ch5_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch5_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch5_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch6
 */
static int32_t  I2cDev_Ch6_Init(void);
static int32_t  I2cDev_Ch6_UnInit(void);
static int32_t  I2cDev_Ch6_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch6_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch6_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch7
 */
static int32_t  I2cDev_Ch7_Init(void);
static int32_t  I2cDev_Ch7_UnInit(void);
static int32_t  I2cDev_Ch7_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  I2cDev_Ch7_DataTx(void *pData, int32_t *pSize);
static int32_t  I2cDev_Ch7_DataRx(void *pData, int32_t *pSize);


/*
 *  IOB Channel Information
 */
/* deviation from MISRA-C:2004 Rule 8.8 */
I2cDev_IOB I2cDev[] = {
  /* {{    BaudRate,     SlaveAddr,     AddrMode,    Mode,Reserved},
                 Init,            UnInit,            DataTxRx,            DataTx,            DataRx, CallBackIrq}, */
/*0*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch0_Init, I2cDev_Ch0_UnInit, I2cDev_Ch0_DataTxRx, I2cDev_Ch0_DataTx, I2cDev_Ch0_DataRx, NULL},

/*1*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch1_Init, I2cDev_Ch1_UnInit, I2cDev_Ch1_DataTxRx, I2cDev_Ch1_DataTx, I2cDev_Ch1_DataRx, NULL},

/*2*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch2_Init, I2cDev_Ch2_UnInit, I2cDev_Ch2_DataTxRx, I2cDev_Ch2_DataTx, I2cDev_Ch2_DataRx, NULL},

/*3*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch3_Init, I2cDev_Ch3_UnInit, I2cDev_Ch3_DataTxRx, I2cDev_Ch3_DataTx, I2cDev_Ch3_DataRx, NULL},

/*4*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch4_Init, I2cDev_Ch4_UnInit, I2cDev_Ch4_DataTxRx, I2cDev_Ch4_DataTx, I2cDev_Ch4_DataRx, NULL},

/*5*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch5_Init, I2cDev_Ch5_UnInit, I2cDev_Ch5_DataTxRx, I2cDev_Ch5_DataTx, I2cDev_Ch5_DataRx, NULL},

/*6*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch6_Init, I2cDev_Ch6_UnInit, I2cDev_Ch6_DataTxRx, I2cDev_Ch6_DataTx, I2cDev_Ch6_DataRx, NULL},

/*7*/{{I2C_BAUDRATE, I2C_SLAVEADDR, I2C_ADDRMODE, I2C_MODE, 0},
      I2cDev_Ch7_Init, I2cDev_Ch7_UnInit, I2cDev_Ch7_DataTxRx, I2cDev_Ch7_DataTx, I2cDev_Ch7_DataRx, NULL},
};


/*
 *  Data Information Struct
 */
typedef struct {
    uint32_t  Size;
    uint32_t  Count;
    uint32_t  IntState;
    uint32_t  TxRxFlag;
    uint8_t   *pData;
} I2c_DATA;

/*
 *  Driver Information Control Struct
 */
typedef struct {
    int32_t      State;      /* State            */
    int32_t      Fifo;       /* FIFO Depth       */
#ifdef USE_CH03
    FM3_MFS03_I2C_TypeDef *pReg;     /* Register Struct  */
#else
    FM3_MFS47_I2C_TypeDef *pReg;     /* Register Struct  */
#endif
    I2cDev_IOB   *pIOB;      /* IOB Struct       */
    I2c_DATA     *pDataInfo; /* Data Info Struct */
    IRQn_Type    RxIRQn;     /* RX IRQ Number    */
    IRQn_Type    TxIRQn;     /* TX IRQ Number    */
} I2c_INFO;


/*
 *  Data Channel Information
 */
static I2c_DATA I2cData[] = {
   /*{Size,Count,IntState,TxRxFlag,pData}*/
/*0*/{   0,    0,       0,       0, NULL},
/*1*/{   0,    0,       0,       0, NULL},
/*2*/{   0,    0,       0,       0, NULL},
/*3*/{   0,    0,       0,       0, NULL},
/*4*/{   0,    0,       0,       0, NULL},
/*5*/{   0,    0,       0,       0, NULL},
/*6*/{   0,    0,       0,       0, NULL},
/*7*/{   0,    0,       0,       0, NULL},
};


/*
 *  Driver Channel Information
 */
/* pReg : deviation from MISRA-C:2004 Rule 11.3 */
static I2c_INFO I2cInfo[] = {
   /*{            State,                   Fifo,         pReg,       pIOB,   pDataInfo,      RxIRQn,      TxIRQn}*/
#ifdef USE_CH03
/*0*/{I2C_UNINITIALIZED,                      0, FM3_MFS0_I2C, &I2cDev[0], &I2cData[0], MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{I2C_UNINITIALIZED,                      0, FM3_MFS1_I2C, &I2cDev[1], &I2cData[1], MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{I2C_UNINITIALIZED,                      0, FM3_MFS2_I2C, &I2cDev[2], &I2cData[2], MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{I2C_UNINITIALIZED,                      0, FM3_MFS3_I2C, &I2cDev[3], &I2cData[3], MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{I2C_UNINITIALIZED, I2cDev_CH04_FIFO_DEPTH,            0, &I2cDev[4], &I2cData[4], MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{I2C_UNINITIALIZED, I2cDev_CH05_FIFO_DEPTH,            0, &I2cDev[5], &I2cData[5], MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{I2C_UNINITIALIZED, I2cDev_CH06_FIFO_DEPTH,            0, &I2cDev[6], &I2cData[6], MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{I2C_UNINITIALIZED, I2cDev_CH07_FIFO_DEPTH,            0, &I2cDev[7], &I2cData[7], MFS7RX_IRQn, MFS7TX_IRQn},
#else
/*0*/{I2C_UNINITIALIZED,                      0,            0, &I2cDev[0], &I2cData[0], MFS0RX_IRQn, MFS0TX_IRQn},
/*1*/{I2C_UNINITIALIZED,                      0,            0, &I2cDev[1], &I2cData[1], MFS1RX_IRQn, MFS1TX_IRQn},
/*2*/{I2C_UNINITIALIZED,                      0,            0, &I2cDev[2], &I2cData[2], MFS2RX_IRQn, MFS2TX_IRQn},
/*3*/{I2C_UNINITIALIZED,                      0,            0, &I2cDev[3], &I2cData[3], MFS3RX_IRQn, MFS3TX_IRQn},
/*4*/{I2C_UNINITIALIZED, I2cDev_CH04_FIFO_DEPTH, FM3_MFS4_I2C, &I2cDev[4], &I2cData[4], MFS4RX_IRQn, MFS4TX_IRQn},
/*5*/{I2C_UNINITIALIZED, I2cDev_CH05_FIFO_DEPTH, FM3_MFS5_I2C, &I2cDev[5], &I2cData[5], MFS5RX_IRQn, MFS5TX_IRQn},
/*6*/{I2C_UNINITIALIZED, I2cDev_CH06_FIFO_DEPTH, FM3_MFS6_I2C, &I2cDev[6], &I2cData[6], MFS6RX_IRQn, MFS6TX_IRQn},
/*7*/{I2C_UNINITIALIZED, I2cDev_CH07_FIFO_DEPTH, FM3_MFS7_I2C, &I2cDev[7], &I2cData[7], MFS7RX_IRQn, MFS7TX_IRQn},
#endif
};


/*
 *  TxRxFlag
 */
#define I2C_RX          1
#define I2C_RX_RESTART  2
#define I2C_TX          3
#define I2C_TX_NOSTOP   4

/*
 *  IntState
 */
#define I2C_END_DATA    1
#define I2C_NACK        2
#define I2C_STOP_COND   3
#define I2C_ERROR       0x0000FFFF

/*
 *  Internal Function
 */
static int32_t  i2c_Init(I2c_INFO *pInfo);
static void  i2c_UnInit(uint32_t Ch, I2c_INFO *pInfo);
static void  i2c_MasterStart(I2c_INFO *pInfo);
static void  i2c_MasterDataTx(I2c_INFO *pInfo);
static void  i2c_MasterDataRx(I2c_INFO *pInfo);
static void  i2c_SlavePreStart(const I2c_INFO *pInfo);
static void  i2c_SlaveStart(I2c_INFO *pInfo);
static void  i2c_SlaveDataTx(I2c_INFO *pInfo);
static void  i2c_SlaveDataRx(I2c_INFO *pInfo);


/*
 *  I2cDev_Init
 */
static int32_t  I2cDev_Init(uint32_t Ch)
{
    I2c_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
        p_info = &I2cInfo[Ch];
        
        if (Mfs_Open(Ch, SMR_MD_I2C) == ERROR) {
            ret = ERROR; /* open error */
            
        } else {
        /* When Mfs_Open is SUCCESS, p_info->State is I2C_UNINITIALIZED. */
            
            /* initialize hardware */
            ret = i2c_Init(p_info);
            
            if (ret == SUCCESS) {
                /* enable NVIC */
                I2C_ENABLE_IRQ(p_info->RxIRQn);
                I2C_ENABLE_IRQ(p_info->TxIRQn);
                
                /* change state */
                p_info->State = I2C_INITIALIZED;
                
                ret = SUCCESS;
            } else {
                Mfs_Close(Ch);
                ret = ERROR; /* i2c_Init error */
            }
        }
        
        Mfs_UnLock(Ch);
    } else {
        ret = ERROR; /* Mfs_Lock error */
    }
    
    return ret;
}


/*
 *  I2cDev_UnInit
 */
static int32_t  I2cDev_UnInit(uint32_t Ch)
{
    I2c_INFO *p_info;
    int32_t ret;
    
    if (Mfs_Lock(Ch) != ERROR) {
    
        p_info = &I2cInfo[Ch];
        if (p_info->State != I2C_INITIALIZED) {
            ret = ERROR; /* not initialized */
            
        } else {
            /* change state */
            p_info->State = I2C_UNINITIALIZED;
            
            /* disable NVIC */
            I2C_DISABLE_IRQ(p_info->RxIRQn);
            I2C_DISABLE_IRQ(p_info->TxIRQn);
            
            /* uninitialize */
            i2c_UnInit(Ch, p_info);
            
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
 *  I2cDev_DataTxRx
 */
static int32_t  I2cDev_DataTxRx(uint32_t Ch, void *pDataTx, void *pDataRx, uint32_t Size)
{
    I2c_INFO *p_info;
    int32_t ret;
    
    /* check NULL pointer */
    if ((pDataTx == NULL) || (pDataRx == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (Size == 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &I2cInfo[Ch];
    
    if (p_info->State != I2C_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else if (p_info->pIOB->Cfg.Mode != I2cDev_MODE_MASTER) {
        ret = ERROR; /* called by the slave mode */
        
    } else {
        /*
         *  Tx sequence
         */
        /* initialized data information */
        p_info->pDataInfo->Size     = Size;
        p_info->pDataInfo->Count    = 0;
        p_info->pDataInfo->IntState = 0;
        p_info->pDataInfo->TxRxFlag = I2C_TX_NOSTOP; /* No Stop Condition */
        p_info->pDataInfo->pData    = pDataTx;
        
        i2c_MasterStart(p_info);
        
        /* wait until tx is completed or error occur */
        while (p_info->pDataInfo->IntState == 0) {
            /* sleep */
        }
        
        if (p_info->pDataInfo->Count != 0) {
            /*
             *  Rx sequence
             */
            /* initialized data information */
            p_info->pDataInfo->Size     = p_info->pDataInfo->Count; /* Tx Count */
            p_info->pDataInfo->Count    = 0;
            p_info->pDataInfo->IntState = 0;
            p_info->pDataInfo->TxRxFlag = I2C_RX_RESTART;
            p_info->pDataInfo->pData    = pDataRx;
            
            i2c_MasterStart(p_info);
            
            /* wait until rx is completed or error occur */
            while (p_info->pDataInfo->IntState == 0) {
                /* sleep */
            }
            
            /* check Count */
            if (p_info->pDataInfo->Size == p_info->pDataInfo->Count) {
                ret = SUCCESS;
                
            } else {
                ret = ERROR;
                
            }
            
        } else {
            ret = ERROR; /* Tx Count == 0 */
            
        }
        
        /* uninitialized data information */
        p_info->pDataInfo->TxRxFlag = 0;
        
    }
    
    return ret;
}

/*
 *  I2cDev_DataTx
 */
static int32_t  I2cDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize)
{
    I2c_INFO *p_info;
    int32_t ret;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &I2cInfo[Ch];
    
    if (p_info->State != I2C_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        /* initialized data information */
        p_info->pDataInfo->Size     = (uint32_t)(*pSize);
        p_info->pDataInfo->Count    = 0;
        p_info->pDataInfo->IntState = 0;
        p_info->pDataInfo->TxRxFlag = I2C_TX;
        p_info->pDataInfo->pData    = pData;
        
        /* check Mode */
        if (p_info->pIOB->Cfg.Mode == I2cDev_MODE_MASTER) {
            /* Master mode */
            i2c_MasterStart(p_info);
            
        } else {
            /* Slave mode */
            i2c_SlaveStart(p_info);
            
        }
        
        /* wait until tx is completed or error occur */
        while (p_info->pDataInfo->IntState == 0) {
            /* sleep */
        }
        
        /* uninitialized data information */
        p_info->pDataInfo->TxRxFlag = 0;
        
        /* check Count */
        if (p_info->pDataInfo->Count != 0) {
            /* return size */
            *pSize = (int32_t)(p_info->pDataInfo->Count);
            ret = SUCCESS;
            
        } else { /* Count == 0 */
            *pSize = 0;
            ret = ERROR;
            
        }
        
    }
    
    return ret;
}


/*
 *  I2cDev_DataRx
 */
static int32_t  I2cDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize)
{
    I2c_INFO *p_info;
    int32_t ret;
    
    /* check NULL pointer */
    if ((pData == NULL) || (pSize == NULL)) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    /* check parameter */
    if (*pSize <= 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &I2cInfo[Ch];
    
    if (p_info->State != I2C_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        /* initialized data information */
        p_info->pDataInfo->Size     = (uint32_t)(*pSize);
        p_info->pDataInfo->Count    = 0;
        p_info->pDataInfo->IntState = 0;
        p_info->pDataInfo->TxRxFlag = I2C_RX;
        p_info->pDataInfo->pData    = pData;
        
        /* check Mode */
        if (p_info->pIOB->Cfg.Mode == I2cDev_MODE_MASTER) { /* Master mode */
            i2c_MasterStart(p_info);
            
        } else { /* Slave mode */
            i2c_SlaveStart(p_info);
            
        }
        
        /* wait until rx is completed or error occur */
        while (p_info->pDataInfo->IntState == 0) {
            /* sleep */
        }
        
        /* uninitialized data information */
        p_info->pDataInfo->TxRxFlag = 0;
        
        /* check Count */
        if (p_info->pDataInfo->Count != 0) {
            /* return size */
            *pSize = (int32_t)(p_info->pDataInfo->Count);
            ret = SUCCESS;
            
        } else { /* Count == 0 */
            *pSize = 0;
            ret = ERROR;
            
        }
    }
    
    return ret;
}



/*
 *  i2c_Init (Internal Function)
 */
static int32_t  i2c_Init(I2c_INFO *pInfo)
{
    I2cDev_CFG *p_cfg;
    int32_t ret;
    uint16_t reg_bgr;
    uint8_t reg_smr    = 0;
    uint8_t reg_ibcr   = 0;
    uint8_t reg_isba   = 0;
    uint8_t reg_ismk   = 0;
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
    /* Mode = I2C */
    reg_smr |= (SMR_MD_I2C);
    
    /*
     *  Set value of BGR register
     */
    /* BaudRate : round off reg_bgr to the whole number */
    reg_bgr = (uint16_t)(((SysFrePCLK2 + ((uint32_t)p_cfg->BaudRate / 2)) / (uint32_t)p_cfg->BaudRate) - 1);
    
    /*
     *  Set value of IBCR register
     */
    /* ACK enable */
    reg_ibcr |= (IBCR_ACKE);
    
    if (p_cfg->Mode == I2cDev_MODE_SLAVE) {
        /* slave mode */
        /* stop condition interrupt enable, interrup enable */
        reg_ibcr |= (IBCR_CNDE | IBCR_INTE);
    }
    
    /*
     *  Set value of ISBA register
     */
    if (p_cfg->Mode == I2cDev_MODE_SLAVE) {
        /* slave mode */
        /* set slave address */
        reg_isba |= (uint8_t)p_cfg->SlaveAddr;
        
        /* slave address detection enable */
        reg_isba |= ISBA_SAEN;
    }
    
    if (pInfo->Fifo > 0) { /* HW FIFO is used */
        /*
         *  Set value of FCR1 register
         */
        /* FCR1_FDRQ : don't care */
#ifndef USE_CH03
        reg_fcr1 |= (FCR1_FDRQ);
        
        /*
         *  Set value of FBYTE1/RBYTE2 registers
         */
        /* FIFO1 is tx */
        reg_fbyte1 = 0; 
        /* FIFO2 is rx */
        reg_fbyte2 = 0; 
        
        /*
         *  Set value of FCR0 register
         */
        /* FIFO reset, FIFO enable */
        reg_fcr0 |= (FCR0_FCL2 | FCR0_FCL1 | FCR0_FE2 | FCR0_FE1);
#endif        
    } /* Do nothing if HW FIFO is not used */
    
    /*
     *  Set value of ISMK register
     */
    /* i2c enable, bit comparing enable */
    reg_ismk |= (ISMK_EN | 0x7F) ;
    
    
    /*
     *  Register writing
     */
    /* set mode */
    pInfo->pReg->SMR    = reg_smr;
    
    /* i2c disable before other registers are set. */
    pInfo->pReg->ISMK   = 0;
    
    pInfo->pReg->BGR    = reg_bgr;
    pInfo->pReg->IBCR   = reg_ibcr;
    pInfo->pReg->ISBA   = reg_isba;
    
    /* FIFO disable before other FIFO registers are set.*/
#ifndef USE_CH03
    pInfo->pReg->FCR0   = FCR1_FDRQ;
    
    pInfo->pReg->FCR1   = reg_fcr1;
    pInfo->pReg->FBYTE1 = reg_fbyte1;
    pInfo->pReg->FBYTE2 = reg_fbyte2;
    pInfo->pReg->FCR0   = reg_fcr0;
#endif    
    pInfo->pReg->ISMK   = reg_ismk;
    
    ret = SUCCESS;
    
    return ret;
}

/*
 *  i2c_UnInit (Internal Function)
 */
static void  i2c_UnInit(uint32_t Ch, I2c_INFO *pInfo)
{
    uint8_t reg_ismk;
    
    /* disable i2c */
    reg_ismk = pInfo->pReg->ISMK & ~(ISMK_EN);
    pInfo->pReg->ISMK = reg_ismk;
    
    
    /* uninitialize data information */
    pInfo->pDataInfo->Size     = 0;
    pInfo->pDataInfo->Count    = 0;
    pInfo->pDataInfo->IntState = 0;
    pInfo->pDataInfo->TxRxFlag = 0;
    pInfo->pDataInfo->pData    = NULL;
    
    /* uninitialize FIFO Depth */
    switch (Ch) {
    case 4:
        pInfo->Fifo = I2cDev_CH04_FIFO_DEPTH;
        break;
    case 5:
        pInfo->Fifo = I2cDev_CH05_FIFO_DEPTH;
        break;
    case 6:
        pInfo->Fifo = I2cDev_CH06_FIFO_DEPTH;
        break;
    case 7:
        pInfo->Fifo = I2cDev_CH07_FIFO_DEPTH;
        break;
    default:
        pInfo->Fifo = 0;
    }
    
    /* uninitialize Config */
    pInfo->pIOB->Cfg.BaudRate  = I2C_BAUDRATE;
    pInfo->pIOB->Cfg.SlaveAddr = I2C_SLAVEADDR;
    pInfo->pIOB->Cfg.AddrMode  = I2C_ADDRMODE;
    pInfo->pIOB->Cfg.Mode      = I2C_MODE;
    pInfo->pIOB->Cfg.Reserved  = 0;
    
    /* uninitialize CallBackIrq */
    pInfo->pIOB->CallBackIrq = NULL;
    
    return;
}


/*
 *  i2c_MasterStart (Internal Function)
 */
static void  i2c_MasterStart(I2c_INFO *pInfo)
{
    uint32_t i;
    uint32_t count;
    uint8_t slave_addr;
    uint8_t reg_ibcr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    uint8_t reg_smr;
    
    
    if ((pInfo->pDataInfo->TxRxFlag == I2C_RX) || 
        (pInfo->pDataInfo->TxRxFlag == I2C_RX_RESTART)) {
        
        if (pInfo->Fifo > 0) { /* HW FIFO is used */
#ifndef USE_CH03
            /* count is number of dummy data */
            if ((uint32_t)(pInfo->Fifo - 1) >= pInfo->pDataInfo->Size) {
                count = pInfo->pDataInfo->Size;
            } else {
                /* -1 is for SlaveAddr */
                count = (uint32_t)(pInfo->Fifo - 1);
            }
            
            /* threshold value of Rx FIFO */
            pInfo->pReg->FBYTE2 = (uint8_t)count;
#endif     
        } else { /* HW FIFO is not used */
            count = 0;
        }
        
        /* rx */
        /* write slave address,  bit0 = 1 (rx) */
        slave_addr = (((uint8_t)pInfo->pIOB->Cfg.SlaveAddr << 1) | 0x01);
        pInfo->pReg->TDR = slave_addr;
        
        /* write dummy data (0x00) for HW FIFO */
        for (i = 0; i < count; i++) {
            pInfo->pReg->TDR = 0x00; 
        }
        
        /* ACK, wait select, interrup enable */
        reg_ibcr = pInfo->pReg->IBCR | (IBCR_ACKE | IBCR_WSEL | IBCR_INTE);
        reg_ibcr = reg_ibcr & ~(IBCR_INT | IBCR_SCC);
        
        if (pInfo->pDataInfo->TxRxFlag == I2C_RX) {
            /* set master mode */
            reg_ibcr = reg_ibcr | (IBCR_MSS);
            
        } else { /* I2C_RX_RESTART */
            /* restart */
            reg_ibcr = reg_ibcr | (IBCR_SCC);
        }
        
        pInfo->pReg->IBCR = reg_ibcr;
        
    } else { /* I2C_TX or I2C_TX_NOSTOP */
        /* tx */
        /* write slave address,  bit0 = 0 (tx) */
        slave_addr = (((uint8_t)pInfo->pIOB->Cfg.SlaveAddr << 1) & ~(0x01U));
        pInfo->pReg->TDR = slave_addr;
        
        /* set master mode, ACK enable, interrupt enable */
        reg_ibcr = pInfo->pReg->IBCR | (IBCR_MSS | IBCR_ACKE | IBCR_INTE);
        reg_ibcr = reg_ibcr & ~(IBCR_SCC | IBCR_WSEL);
        pInfo->pReg->IBCR = reg_ibcr;
        
        if (pInfo->Fifo > 0) { /* HW FIFO is used */
            /* tx FIFO interrupt enable : interruption occur */
#ifndef USE_CH03
            reg_fcr1 = pInfo->pReg->FCR1 | (FCR1_FTIE);
            pInfo->pReg->FCR1 = reg_fcr1;
#endif
        } else { /* HW FIFO is not used */
            /* tx interrupt enable : interruption occur */
            reg_smr = pInfo->pReg->SMR | (SMR_TIE);
            pInfo->pReg->SMR = reg_smr;
            
        }
        
    }
    
    return;
}


/*
 *  i2c_MasterDataTx (Internal Function)
 */
static void  i2c_MasterDataTx(I2c_INFO *pInfo)
{
    uint32_t num;
    uint32_t i;
    uint8_t reg_ibsr;
    uint8_t reg_smr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    uint8_t reg_ibcr;
    uint8_t reg_ssr;
    
    reg_ibsr = pInfo->pReg->IBSR;
    reg_ibcr = pInfo->pReg->IBCR;
    
    if ((reg_ibsr & IBSR_SPC) != 0) {    /* stop condition */
        /* clear stop condition interrupt */
        reg_ibsr = reg_ibsr & ~(IBSR_SPC);
        pInfo->pReg->IBSR = reg_ibsr;
        
        /* stop condition interrupt disable */
        reg_ibcr = reg_ibcr & ~(IBCR_CNDE | IBCR_SCC);
        pInfo->pReg->IBCR = reg_ibcr;
        
        pInfo->pDataInfo->IntState = I2C_STOP_COND; /* stop condition */
        
    } else if (((pInfo->pDataInfo->Size == pInfo->pDataInfo->Count) && ((reg_ibcr & IBCR_INT) != 0)) ||
        ((reg_ibsr & IBSR_RACK) != 0)) { /* end of data or received NACK */
        
        if (pInfo->pDataInfo->TxRxFlag == I2C_TX) {
            /* stop condition, interrupt disable, clear interrupt */
            reg_ibcr = reg_ibcr & ~(IBCR_MSS | IBCR_SCC| IBCR_INTE | IBCR_INT);
            /* stop condition interrupt enable */
            reg_ibcr = reg_ibcr | (IBCR_CNDE);
            
        } else { /* I2C_TX_NOSTOP */
            /* interrupt disable */
            reg_ibcr = reg_ibcr & ~(IBCR_SCC | IBCR_INTE);
            
            pInfo->pDataInfo->IntState = I2C_END_DATA; /* end of data */
            
        }
        
        pInfo->pReg->IBCR = reg_ibcr;
        
    } else { /* tx data */
        reg_ssr  = pInfo->pReg->SSR;
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1;
        
        if (((reg_ssr & (SSR_TDRE)) != 0) || ((reg_fcr1 & FCR1_FDRQ) != 0)) {
#else
        if ((reg_ssr & (SSR_TDRE)) != 0) { 
#endif
            if (pInfo->Fifo > 0) { /* HW FIFO is used */
                num = pInfo->pDataInfo->Size - pInfo->pDataInfo->Count;
                if (num > (uint32_t)pInfo->Fifo) {
                    num = (uint32_t)pInfo->Fifo;
                }
                
                for (i = 0; i < num; i++) {
                    /* tx data to tx FIFO */
                    pInfo->pReg->TDR = pInfo->pDataInfo->pData[pInfo->pDataInfo->Count];
                    pInfo->pDataInfo->Count += 1;
                }
                
                /* clear FIFO tx interrupt */
#ifndef USE_CH03
                reg_fcr1 = reg_fcr1 & ~(FCR1_FDRQ);
                
                if (pInfo->pDataInfo->Size == pInfo->pDataInfo->Count) {
                    /* tx FIFO interrupt disable */
                    reg_fcr1 = reg_fcr1 & ~(FCR1_FTIE);
                }
                
                pInfo->pReg->FCR1 = reg_fcr1;
#endif                
            } else { /* HW FIFO is not used */
                
                /* tx data to register */
                pInfo->pReg->TDR = pInfo->pDataInfo->pData[pInfo->pDataInfo->Count];
                pInfo->pDataInfo->Count += 1;
                
                if (pInfo->pDataInfo->Size == pInfo->pDataInfo->Count) {
                    /* tx interrupt disable */
                    reg_smr = pInfo->pReg->SMR & ~(SMR_TIE);
                    pInfo->pReg->SMR = reg_smr;
                }
            }
        }
        
        /* clear interrupt */
        reg_ibcr = reg_ibcr & ~(IBCR_SCC | IBCR_INT);
        pInfo->pReg->IBCR = reg_ibcr;
        
    }
    
    return;
}


/*
 *  i2c_MasterDataRx (Internal Function)
 */
static void  i2c_MasterDataRx(I2c_INFO *pInfo)
{
#ifndef USE_CH03
    uint32_t count;
    uint32_t i;
#endif
    uint8_t reg_ibsr;
    uint8_t reg_ssr;
    uint8_t reg_ibcr;
    uint8_t reg_ismk;
    
    reg_ibsr = pInfo->pReg->IBSR;
    reg_ibcr = pInfo->pReg->IBCR;
    reg_ssr  = pInfo->pReg->SSR;
    
    if ((reg_ibsr & IBSR_SPC) != 0) {    /* stop condition */
        /* clear stop condition interrupt */
        reg_ibsr = reg_ibsr & ~(IBSR_SPC);
        pInfo->pReg->IBSR = reg_ibsr;
        
        /* stop condition interrupt disable, interrupt disable */
        reg_ibcr = reg_ibcr & ~(IBCR_CNDE | IBCR_INTE);
        
        /* clear IBSR:RACK */
        reg_ismk = pInfo->pReg->ISMK & ~(ISMK_EN);
        pInfo->pReg->ISMK = reg_ismk;
        /* restart */
        pInfo->pReg->ISMK = reg_ismk | (ISMK_EN);
        
        pInfo->pDataInfo->IntState = I2C_STOP_COND; /* stop condition */
        
    } else if (((reg_ssr & (SSR_RDRF)) != 0) && ((reg_ibsr & (IBSR_FBT)) == 0 )) { /* RDRF == 1 &&  FBT == 0 */
        
        while (pInfo->pDataInfo->Count < pInfo->pDataInfo->Size) {
            
            reg_ssr = pInfo->pReg->SSR;
            if ((reg_ssr & (SSR_RDRF)) != 0) {
                /* rx data from the register(or FIFO) */
                pInfo->pDataInfo->pData[pInfo->pDataInfo->Count] = pInfo->pReg->RDR;
                pInfo->pDataInfo->Count += 1;
                
            } else {
                /* No data */
                break;
            }
        }
        
        if (pInfo->pDataInfo->Count == pInfo->pDataInfo->Size) { /* rx completed */
            /* stop condition, NACK */
            reg_ibcr = reg_ibcr & ~(IBCR_MSS | IBCR_ACKE);
            
            /* stop condition interrupt enable */
            reg_ibcr = reg_ibcr | (IBCR_CNDE);
            
            if ((reg_ibsr & IBSR_RSC) != 0) {
                /* clear restart condiont */
                reg_ibsr = reg_ibsr & ~(IBSR_RSC);
                pInfo->pReg->IBSR = reg_ibsr;
            }
            
        } else {     /* prepare for next data */
            
            if (pInfo->Fifo > 0) { /* HW FIFO is used */
                /* count is number of dummy data */
#ifndef USE_CH03
                if ((uint32_t)pInfo->Fifo >= (pInfo->pDataInfo->Size - pInfo->pDataInfo->Count)) {
                    count = pInfo->pDataInfo->Size - pInfo->pDataInfo->Count;
                } else {
                    count = (uint32_t)pInfo->Fifo;
                }
                
                if (count != 0) {
                    pInfo->pReg->FBYTE2 = (uint8_t)count;
                    
                    /* write dummy data (0x00) */
                    for (i = 0; i < count; i++) {
                        pInfo->pReg->TDR = 0x00; 
                    }
                }
#endif
            }
            
        }
        
    } else if ((reg_ssr & (SSR_ORE)) != 0) {/* overrun error */
        /* clear rx error interrupt */
        reg_ssr |= SSR_REC;
        pInfo->pReg->SSR = reg_ssr;
        
    } else {
        /* Do nothing */
    }
    
    /* clear interrupt */
    reg_ibcr = reg_ibcr & ~(IBCR_SCC | IBCR_INT);
    pInfo->pReg->IBCR = reg_ibcr;
    
    return;
}

/*
 *  i2c_SlavePreStart (Internal Function)
 */
static void  i2c_SlavePreStart(const I2c_INFO *pInfo)
{
    int32_t status;
    uint8_t reg_ibsr;
    uint8_t reg_smr;
    

    
    uint8_t reg_ibcr;
    
    /* clear interrupt */
    reg_ibcr = pInfo->pReg->IBCR & ~(IBCR_SCC | IBCR_INT);
    pInfo->pReg->IBCR = reg_ibcr;
    

    
    reg_ibsr = pInfo->pReg->IBSR;
    
    /* check direction */
    if ((reg_ibsr & (IBSR_TRX)) != 0) { /* tx */
        status = I2cDev_IRQSTATUS_SLAVE_TX;
        
        /* tx interrupt disable */
        reg_smr = pInfo->pReg->SMR & ~(SMR_TIE);
        pInfo->pReg->SMR = reg_smr;
        
    } else { /* rx */
        status = I2cDev_IRQSTATUS_SLAVE_RX;
        
        /* rx interrupt disable */
        reg_smr = pInfo->pReg->SMR & ~(SMR_RIE);
        pInfo->pReg->SMR = reg_smr;
        
    }
    
    /* call CallBackIrq */
    if (pInfo->pIOB->CallBackIrq != NULL) {
        pInfo->pIOB->CallBackIrq(status);
    }
    
    return;
}

/*
 *  i2c_SlaveStart (Internal Function)
 */
static void  i2c_SlaveStart(I2c_INFO *pInfo)
{
    uint8_t reg_ibcr;
    uint8_t reg_ibsr;
    uint8_t reg_smr;
    
    reg_ibcr = pInfo->pReg->IBCR & (IBCR_MSS | IBCR_ACT);
    
    if (reg_ibcr == IBCR_ACT) { /* MSS == 0, ACT == 1 */
        /* Slave is active */
    

    
        /* set ACK */
        reg_ibcr = pInfo->pReg->IBCR | (IBCR_ACKE);
        reg_ibcr = reg_ibcr & ~(IBCR_SCC);
        pInfo->pReg->IBCR = reg_ibcr;
    

    

        reg_ibsr = pInfo->pReg->IBSR;
        
        /* check direction */
        if ((reg_ibsr & (IBSR_TRX)) != 0) { /* tx */
            /* tx interrupt enable */
            reg_smr = pInfo->pReg->SMR | (SMR_TIE);
            pInfo->pReg->SMR = reg_smr;
            
        } else {
            /* tx interrupt enable */
            reg_smr = pInfo->pReg->SMR | (SMR_RIE);
            pInfo->pReg->SMR = reg_smr;
            
        }
        
    } else {
        /* Slave is not active */
        pInfo->pDataInfo->IntState = I2C_ERROR; /*  error */
        
    }
    
    return;
}

/*
 *  i2c_SlaveDataTx (Internal Function)
 */
static void  i2c_SlaveDataTx(I2c_INFO *pInfo)
{
    uint8_t reg_ibsr;
    uint8_t reg_smr;
#ifndef USE_CH03
    uint8_t reg_fcr1;
#endif
    uint8_t reg_ibcr;
    uint8_t reg_ssr;
    
    reg_ibsr = pInfo->pReg->IBSR;
    
    if ((pInfo->pDataInfo->Size == pInfo->pDataInfo->Count)  || /* end of data  or   */
        ((reg_ibsr & IBSR_RACK) != 0) ||                        /* received NACK  or */
        ((reg_ibsr & IBSR_SPC) != 0)) {                          /* stop condition    */
        
        /* tx interrupt disable */
        reg_smr = pInfo->pReg->SMR & ~(SMR_TIE);
        pInfo->pReg->SMR = reg_smr;
        
        /* tx FIFO interrupt disable */
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1 & ~(FCR1_FTIE);
        pInfo->pReg->FCR1 = reg_fcr1;
#endif
        if ((reg_ibsr & IBSR_RACK) != 0) {
            pInfo->pDataInfo->IntState = I2C_NACK; /* NACK */
            
        } else if ((reg_ibsr & IBSR_SPC) != 0) {
            /* clear SPC interrupt */
            reg_ibsr = pInfo->pReg->IBSR & ~(IBSR_SPC);
            pInfo->pReg->IBSR = reg_ibsr;
            
            pInfo->pDataInfo->IntState = I2C_STOP_COND; /* stop condition */
            
        } else {
            pInfo->pDataInfo->IntState = I2C_END_DATA; /* end of data */
            
        }
        

        
        /* clear interrupt */
        reg_ibcr = pInfo->pReg->IBCR & ~(IBCR_SCC | IBCR_INT);
        pInfo->pReg->IBCR = reg_ibcr;
        

        
        /* clear RSC interrupt */
        reg_ibsr = pInfo->pReg->IBSR & ~(IBSR_RSC);
        pInfo->pReg->IBSR = reg_ibsr;
        

        
    } else { /* tx data */
        reg_ssr  = pInfo->pReg->SSR;
#ifndef USE_CH03
        reg_fcr1 = pInfo->pReg->FCR1;
        
        if (((reg_ssr & (SSR_TDRE)) != 0) || ((reg_fcr1 & FCR1_FDRQ) != 0)) {
#else
          if ((reg_ssr & (SSR_TDRE)) != 0) {
#endif
            if (pInfo->Fifo > 0) { /* HW FIFO is used */
#ifndef USE_CH03                
                while (pInfo->pDataInfo->Count < pInfo->pDataInfo->Size) {
                    
                    if (pInfo->pDataInfo->Count < (uint32_t)pInfo->Fifo) {
                        /* tx data to tx FIFO */
                        pInfo->pReg->TDR = pInfo->pDataInfo->pData[pInfo->pDataInfo->Count];
                        pInfo->pDataInfo->Count += 1;
                    } else {
                        /* Tx FIFO is full. */
                        break;
                    }
                }
                
                /* clear FIFO tx interrupt */
                reg_fcr1 = reg_fcr1 & ~(FCR1_FDRQ);
                pInfo->pReg->FCR1 = reg_fcr1;
#endif                
            } else { /* HW FIFO is not used */
                
                /* tx data to register */
                pInfo->pReg->TDR = pInfo->pDataInfo->pData[pInfo->pDataInfo->Count];
                pInfo->pDataInfo->Count += 1;
                
            }
            
        }
        
        /* clear interrupt */
        reg_ibcr = pInfo->pReg->IBCR & ~(IBCR_SCC | IBCR_INT);
        pInfo->pReg->IBCR = reg_ibcr;
        
    }
    
    return;
}

/*
 *  i2c_SlaveDataRx (Internal Function)
 */
static void  i2c_SlaveDataRx(I2c_INFO *pInfo)
{
    uint8_t reg_ibsr;
    uint8_t reg_ssr;
    uint8_t reg_ibcr;
    
    reg_ibsr = pInfo->pReg->IBSR;
    
    if ((reg_ibsr & IBSR_SPC) != 0) { /* stop condition */
        /* clear SPC interrupt */
        reg_ibsr = pInfo->pReg->IBSR & ~(IBSR_SPC);
        pInfo->pReg->IBSR = reg_ibsr;
        
        pInfo->pDataInfo->IntState = I2C_STOP_COND; /* stop condition */
        
    } else if ((reg_ibsr & IBSR_RACK) != 0) {
        

        
        /* clear interrupt */
        reg_ibcr = pInfo->pReg->IBCR & ~(IBCR_SCC | IBCR_INT);
        pInfo->pReg->IBCR = reg_ibcr;
        

        
        pInfo->pDataInfo->IntState = I2C_NACK; /* NACK */
        
    } else { /* rx data */
        reg_ssr  = pInfo->pReg->SSR;
        reg_ibsr = pInfo->pReg->IBSR;
        reg_ibcr = pInfo->pReg->IBCR;
        
        if (((reg_ssr & (SSR_RDRF)) != 0) && ((reg_ibsr & (IBSR_FBT)) == 0 )) { /* RDRF == 1 &&  FBT == 0 */
            
            while (pInfo->pDataInfo->Count < pInfo->pDataInfo->Size) {
                
                reg_ssr = pInfo->pReg->SSR;
                if ((reg_ssr & (SSR_RDRF)) != 0) {
                    /* rx data from the register(or FIFO) */
                    pInfo->pDataInfo->pData[pInfo->pDataInfo->Count] = pInfo->pReg->RDR;
                    pInfo->pDataInfo->Count += 1;
                    
                } else {
                    /* No data */
                    break;
                }
            }
            
            if (pInfo->pDataInfo->Count == pInfo->pDataInfo->Size) { /* rx completed */
                /* set value of IBCR : send NACK */
                reg_ibcr = reg_ibcr & ~(IBCR_ACKE);
            } else {
                /* set value of IBCR : send ACK */
                reg_ibcr = reg_ibcr | (IBCR_ACKE);
            }
            
        } else {/* overrun error */
            /* clear rx error interrupt */
            reg_ssr |= SSR_REC;
            pInfo->pReg->SSR = reg_ssr;
            
        }
        
        /* set value of IBCR : clear interrupt */
        reg_ibcr = reg_ibcr & ~(IBCR_SCC | IBCR_INT);
        /* register writing : IBCR */
        pInfo->pReg->IBCR = reg_ibcr;
    }
    
    return;
}

/*
 *  i2c_IrqHandler (Interrupt Handling Funtion)
 */
void  i2c_IrqHandler(uint32_t Ch)
{
    I2c_INFO *p_info;
    
    p_info = &I2cInfo[Ch];
    
    /* check Mode */
    if (p_info->pIOB->Cfg.Mode == I2cDev_MODE_MASTER) { /* Master mode */
        
        if ((p_info->pDataInfo->TxRxFlag == I2C_RX) ||
            (p_info->pDataInfo->TxRxFlag == I2C_RX_RESTART)) { 
            /* rx */
            i2c_MasterDataRx(p_info);
            
        } else { /* I2C_TX or I2C_TX_NOSTOP */
            /* tx */
            i2c_MasterDataTx(p_info);
            
        }
        
    } else { /* Slave mode */
        
        if (p_info->pDataInfo->TxRxFlag == I2C_RX) {
            /* rx */
            i2c_SlaveDataRx(p_info);
            
        } else if (p_info->pDataInfo->TxRxFlag == I2C_TX) {
            /* tx */
            i2c_SlaveDataTx(p_info);
            
        } else {
            /* Before rx and tx */
            i2c_SlavePreStart(p_info);
            
        }
        
    }
    
    return;
}



/*
 *  Channel Function for Ch0
 */
static int32_t  I2cDev_Ch0_Init(void)
{
    return I2cDev_Init(0);
}

static int32_t  I2cDev_Ch0_UnInit(void)
{
    return I2cDev_UnInit(0);
}

static int32_t  I2cDev_Ch0_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(0, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch0_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(0, pData, pSize);
}

static int32_t  I2cDev_Ch0_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(0, pData, pSize);
}


/*
 *  Channel Function for Ch1
 */
static int32_t  I2cDev_Ch1_Init(void)
{
    return I2cDev_Init(1);
}

static int32_t  I2cDev_Ch1_UnInit(void)
{
    return I2cDev_UnInit(1);
}

static int32_t  I2cDev_Ch1_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(1, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch1_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(1, pData, pSize);
}

static int32_t  I2cDev_Ch1_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(1, pData, pSize);
}


/*
 *  Channel Function for Ch2
 */
static int32_t  I2cDev_Ch2_Init(void)
{
    return I2cDev_Init(2);
}

static int32_t  I2cDev_Ch2_UnInit(void)
{
    return I2cDev_UnInit(2);
}

static int32_t  I2cDev_Ch2_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(2, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch2_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(2, pData, pSize);
}

static int32_t  I2cDev_Ch2_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(2, pData, pSize);
}


/*
 *  Channel Function for Ch3
 */
static int32_t  I2cDev_Ch3_Init(void)
{
    return I2cDev_Init(3);
}

static int32_t  I2cDev_Ch3_UnInit(void)
{
    return I2cDev_UnInit(3);
}

static int32_t  I2cDev_Ch3_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(3, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch3_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(3, pData, pSize);
}

static int32_t  I2cDev_Ch3_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(3, pData, pSize);
}


/*
 *  Channel Function for Ch4
 */
static int32_t  I2cDev_Ch4_Init(void)
{
    return I2cDev_Init(4);
}

static int32_t  I2cDev_Ch4_UnInit(void)
{
    return I2cDev_UnInit(4);
}

static int32_t  I2cDev_Ch4_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(4, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch4_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(4, pData, pSize);
}

static int32_t  I2cDev_Ch4_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(4, pData, pSize);
}


/*
 *  Channel Function for Ch5
 */
static int32_t  I2cDev_Ch5_Init(void)
{
    return I2cDev_Init(5);
}

static int32_t  I2cDev_Ch5_UnInit(void)
{
    return I2cDev_UnInit(5);
}

static int32_t  I2cDev_Ch5_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(5, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch5_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(5, pData, pSize);
}

static int32_t  I2cDev_Ch5_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(5, pData, pSize);
}


/*
 *  Channel Function for Ch6
 */
static int32_t  I2cDev_Ch6_Init(void)
{
    return I2cDev_Init(6);
}

static int32_t  I2cDev_Ch6_UnInit(void)
{
    return I2cDev_UnInit(6);
}

static int32_t  I2cDev_Ch6_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(6, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch6_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(6, pData, pSize);
}

static int32_t  I2cDev_Ch6_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(6, pData, pSize);
}


/*
 *  Channel Function for Ch7
 */
static int32_t  I2cDev_Ch7_Init(void)
{
    return I2cDev_Init(7);
}

static int32_t  I2cDev_Ch7_UnInit(void)
{
    return I2cDev_UnInit(7);
}

static int32_t  I2cDev_Ch7_DataTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return I2cDev_DataTxRx(7, pDataTx, pDataRx, Size);
}

static int32_t  I2cDev_Ch7_DataTx(void *pData, int32_t *pSize)
{
    return I2cDev_DataTx(7, pData, pSize);
}

static int32_t  I2cDev_Ch7_DataRx(void *pData, int32_t *pSize)
{
    return I2cDev_DataRx(7, pData, pSize);
}

