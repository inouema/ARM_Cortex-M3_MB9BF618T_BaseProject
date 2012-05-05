/*
 *  Copyright (C) 2010 FUJITSU SEMICONDUCTOR LIMITED
 *  SPI Driver
 */

#include "mcu.h"
#include "SpiDev.h"
#include "SpiDev_FM3.h"
#include "clock_def.h"

/*
 *  Channel Common Function 
 */
static int32_t  SpiDev_Init(uint32_t Ch);
static int32_t  SpiDev_UnInit(uint32_t Ch);
static int32_t  SpiDev_BufTxRx(uint32_t Ch, void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_SetBaudrate(uint32_t Ch, int32_t Baudrate);
static int32_t  SpiDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize);
static int32_t  SpiDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch0
 */
static int32_t  SpiDev_Ch0_Init(void);
static int32_t  SpiDev_Ch0_UnInit(void);
static int32_t  SpiDev_Ch0_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch0_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch0_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch0_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch1
 */
static int32_t  SpiDev_Ch1_Init(void);
static int32_t  SpiDev_Ch1_UnInit(void);
static int32_t  SpiDev_Ch1_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch1_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch1_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch1_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch2
 */
static int32_t  SpiDev_Ch2_Init(void);
static int32_t  SpiDev_Ch2_UnInit(void);
static int32_t  SpiDev_Ch2_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch2_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch2_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch2_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch3
 */
static int32_t  SpiDev_Ch3_Init(void);
static int32_t  SpiDev_Ch3_UnInit(void);
static int32_t  SpiDev_Ch3_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch3_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch3_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch3_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch4
 */
static int32_t  SpiDev_Ch4_Init(void);
static int32_t  SpiDev_Ch4_UnInit(void);
static int32_t  SpiDev_Ch4_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch4_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch4_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch4_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch5
 */
static int32_t  SpiDev_Ch5_Init(void);
static int32_t  SpiDev_Ch5_UnInit(void);
static int32_t  SpiDev_Ch5_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch5_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch5_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch5_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch6
 */
static int32_t  SpiDev_Ch6_Init(void);
static int32_t  SpiDev_Ch6_UnInit(void);
static int32_t  SpiDev_Ch6_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch6_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch6_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch6_DataRx(void *pData, int32_t *pSize);

/*
 *  Channel Function for Ch7
 */
static int32_t  SpiDev_Ch7_Init(void);
static int32_t  SpiDev_Ch7_UnInit(void);
static int32_t  SpiDev_Ch7_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size);
static int32_t  SpiDev_Ch7_SetBaudrate(int32_t Baudrate);
static int32_t  SpiDev_Ch7_DataTx(void *pData, int32_t *pSize);
static int32_t  SpiDev_Ch7_DataRx(void *pData, int32_t *pSize);


/*
 *  IOB Channel Information
 */
SpiDev_IOB SpiDev[] = {
  /* {{    BaudRate,     Polarity,Phase,SlaveSelect,     Mode,    BitOrder, Reserved},
                 Init,            UnInit,            BufTxRx,            SetBaudrate,            DataTx,            DataRx}*/
/*0*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch0_Init, SpiDev_Ch0_UnInit, SpiDev_Ch0_BufTxRx, SpiDev_Ch0_SetBaudrate, SpiDev_Ch0_DataTx, SpiDev_Ch0_DataRx},

/*1*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch1_Init, SpiDev_Ch1_UnInit, SpiDev_Ch1_BufTxRx, SpiDev_Ch1_SetBaudrate, SpiDev_Ch1_DataTx, SpiDev_Ch1_DataRx},

/*2*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch2_Init, SpiDev_Ch2_UnInit, SpiDev_Ch2_BufTxRx, SpiDev_Ch2_SetBaudrate, SpiDev_Ch2_DataTx, SpiDev_Ch2_DataRx},

/*3*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch3_Init, SpiDev_Ch3_UnInit, SpiDev_Ch3_BufTxRx, SpiDev_Ch3_SetBaudrate, SpiDev_Ch3_DataTx, SpiDev_Ch3_DataRx},

/*4*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch4_Init, SpiDev_Ch4_UnInit, SpiDev_Ch4_BufTxRx, SpiDev_Ch4_SetBaudrate, SpiDev_Ch4_DataTx, SpiDev_Ch4_DataRx},

/*5*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch5_Init, SpiDev_Ch5_UnInit, SpiDev_Ch5_BufTxRx, SpiDev_Ch5_SetBaudrate, SpiDev_Ch5_DataTx, SpiDev_Ch5_DataRx},

/*6*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch6_Init, SpiDev_Ch6_UnInit, SpiDev_Ch6_BufTxRx, SpiDev_Ch6_SetBaudrate, SpiDev_Ch6_DataTx, SpiDev_Ch6_DataRx},

/*7*/{{SPI_BAUDRATE, SPI_POLARITY,    0,          0, SPI_MODE, SPI_BITODER, 0},
      SpiDev_Ch7_Init, SpiDev_Ch7_UnInit, SpiDev_Ch7_BufTxRx, SpiDev_Ch7_SetBaudrate, SpiDev_Ch7_DataTx, SpiDev_Ch7_DataRx},
};


/*
 *  Driver Information Control Struct
 */
typedef struct {
    int32_t      State;      /* State          */
    SpiDev_IOB   *pIOB;      /* IOB Struct     */
    SioDev_IOB   *pSioIOB;   /* Sio IOB Struct */
} Spi_INFO;


/*
 *  Driver Channel Information
 */
static Spi_INFO SpiInfo[] = {
   /*{            State,       pIOB,    pSioIOB}*/
/*0*/{SPI_UNINITIALIZED, &SpiDev[0], &SioDev[0]},
/*1*/{SPI_UNINITIALIZED, &SpiDev[1], &SioDev[1]},
/*2*/{SPI_UNINITIALIZED, &SpiDev[2], &SioDev[2]},
/*3*/{SPI_UNINITIALIZED, &SpiDev[3], &SioDev[3]},
/*4*/{SPI_UNINITIALIZED, &SpiDev[4], &SioDev[4]},
/*5*/{SPI_UNINITIALIZED, &SpiDev[5], &SioDev[5]},
/*6*/{SPI_UNINITIALIZED, &SpiDev[6], &SioDev[6]},
/*7*/{SPI_UNINITIALIZED, &SpiDev[7], &SioDev[7]},
};


/*
 *  Internal Function
 */
static void  spi_SetBaudrate(uint32_t Ch, int32_t Baudrate);


/*
 *  SpiDev_Init
 */
static int32_t  SpiDev_Init(uint32_t Ch)
{
    Spi_INFO *p_info;
    int32_t ret;
    
    p_info = &SpiInfo[Ch];
    
    p_info->pSioIOB->Cfg.BaudRate    = p_info->pIOB->Cfg.Baudrate;
    p_info->pSioIOB->Cfg.DataBits    = SioDev_DATABITS_8;
    p_info->pSioIOB->Cfg.Mode        = p_info->pIOB->Cfg.Mode;
    p_info->pSioIOB->Cfg.SckPolarity = p_info->pIOB->Cfg.Polarity;
    p_info->pSioIOB->Cfg.BitOrder    = p_info->pIOB->Cfg.BitOrder;
    p_info->pSioIOB->Cfg.Reserved    = 1; /* 1 means SPI Mode */
    
    ret = p_info->pSioIOB->Init();
    
    if (ret == SUCCESS) {
        /* change state */
        p_info->State = SPI_INITIALIZED;
    }
    
    return ret;
}


/*
 *  SpiDev_UnInit
 */
static int32_t  SpiDev_UnInit(uint32_t Ch)
{
    Spi_INFO *p_info;
    int32_t ret;
    
    p_info = &SpiInfo[Ch];
    
    ret = p_info->pSioIOB->UnInit();
    if (ret == SUCCESS) {
        /* uninitialize Config */
        p_info->pIOB->Cfg.Baudrate = SPI_BAUDRATE;
        p_info->pIOB->Cfg.Mode     = SPI_MODE;
        p_info->pIOB->Cfg.Polarity = SPI_POLARITY;
        p_info->pIOB->Cfg.BitOrder = SPI_BITODER;
        
        /* change state */
        p_info->State = SPI_UNINITIALIZED;
    }
    
    return ret;
}


/*
 *  SpiDev_BufTxRx
 */
static int32_t  SpiDev_BufTxRx(uint32_t Ch, void *pDataTx, void *pDataRx, uint32_t Size)
{
    Spi_INFO *p_info;
    int32_t ret;
    int32_t err;
    int32_t len;
    uint32_t count;
    uint8_t *p_tx;
    uint8_t *p_rx;
    
    /* check State in SIO Driver */
    
    /* check parameter */
    if (Size == 0) {
        return ERROR; /* deviation from MISRA-C:2004 Rule 14.7 */
    }
    
    p_info = &SpiInfo[Ch];
    
    ret   = SUCCESS;
    count = 0;
    len   = 1;
    p_tx  = (uint8_t*)pDataTx;
    p_rx  = (uint8_t*)pDataRx;
    
    while (count < Size) {
        
        /* tx */
        err = p_info->pSioIOB->DataTx(&(p_tx[count]), &len);
        if ((err == SUCCESS) && (len == 1)) {
            
            /* rx */
            err = p_info->pSioIOB->DataRx(&(p_rx[count]), &len);
            if ((err == SUCCESS) && (len == 1)) {
                count++;
                
            } else {
                /* rx error */
                ret = ERROR;
            }
        } else {
            /* tx error */
            ret = ERROR;
        }
        
        if (ret == ERROR) {
            break;
        }
    }
    
    return ret;
}


/*
 *  SpiDev_SetBaudrate
 */
static int32_t  SpiDev_SetBaudrate(uint32_t Ch, int32_t Baudrate)
{
    Spi_INFO *p_info;
    int32_t ret;
    
    p_info = &SpiInfo[Ch];
    
    if (p_info->State != SPI_INITIALIZED) {
        ret = ERROR; /* not initialized */
        
    } else {
        spi_SetBaudrate(Ch, Baudrate);
        ret = SUCCESS;
    }
    
    return ret;
}


/*
 *  SpiDev_DataTx
 */
static int32_t  SpiDev_DataTx(uint32_t Ch, void *pData, int32_t *pSize)
{
    Spi_INFO *p_info;
    int32_t ret;
    
    /* check State in SIO Driver */
    
    p_info = &SpiInfo[Ch];
    
    ret = p_info->pSioIOB->DataTx(pData, pSize);
    
    return ret;
}


/*
 *  SpiDev_DataRx
 */
static int32_t  SpiDev_DataRx(uint32_t Ch, void *pData, int32_t *pSize)
{
    Spi_INFO *p_info;
    int32_t ret;
    
    /* check State in SIO Driver */
    
    p_info = &SpiInfo[Ch];
    
    ret = p_info->pSioIOB->DataRx(pData, pSize);
    
    return ret;
}


/*
 *  spi_SetBaudrate (Internal Function)
 */
static void  spi_SetBaudrate(uint32_t Ch, int32_t Baudrate)
{
    /* p_reg : deviation from MISRA-C:2004 Rule 11.3 */
#ifdef USE_CH03  
    FM3_MFS03_CSIO_TypeDef *p_reg[] = {FM3_MFS0_CSIO, FM3_MFS1_CSIO, FM3_MFS2_CSIO, FM3_MFS3_CSIO, 0, 0, 0, 0}; 
#else
    FM3_MFS47_CSIO_TypeDef *p_reg[] = {0, 0, 0, 0, FM3_MFS4_CSIO, FM3_MFS5_CSIO, FM3_MFS6_CSIO, FM3_MFS7_CSIO}; 
#endif
    uint16_t reg_bgr;
    
    /*
     *  SIO's BGR register
     */
    /* BaudRate : round off reg_bgr to the whole number */
    reg_bgr = (uint16_t)(((SysFrePCLK2 + (uint32_t)Baudrate) / (uint32_t)Baudrate) - 1);
    p_reg[Ch]->BGR = reg_bgr;
    
    return;
}


/*
 *  Channel Function for Ch0
 */
static int32_t  SpiDev_Ch0_Init(void)
{
    return SpiDev_Init(0);
}

static int32_t  SpiDev_Ch0_UnInit(void)
{
    return SpiDev_UnInit(0);
}

static int32_t  SpiDev_Ch0_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(0, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch0_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(0, Baudrate);
}

static int32_t  SpiDev_Ch0_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(0, pData, pSize);
}

static int32_t  SpiDev_Ch0_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(0, pData, pSize);
}


/*
 *  Channel Function for Ch1
 */
static int32_t  SpiDev_Ch1_Init(void)
{
    return SpiDev_Init(1);
}

static int32_t  SpiDev_Ch1_UnInit(void)
{
    return SpiDev_UnInit(1);
}

static int32_t  SpiDev_Ch1_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(1, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch1_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(1, Baudrate);
}

static int32_t  SpiDev_Ch1_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(1, pData, pSize);
}

static int32_t  SpiDev_Ch1_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(1, pData, pSize);
}


/*
 *  Channel Function for Ch2
 */
static int32_t  SpiDev_Ch2_Init(void)
{
    return SpiDev_Init(2);
}

static int32_t  SpiDev_Ch2_UnInit(void)
{
    return SpiDev_UnInit(2);
}

static int32_t  SpiDev_Ch2_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(2, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch2_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(2, Baudrate);
}

static int32_t  SpiDev_Ch2_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(2, pData, pSize);
}

static int32_t  SpiDev_Ch2_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(2, pData, pSize);
}


/*
 *  Channel Function for Ch3
 */
static int32_t  SpiDev_Ch3_Init(void)
{
    return SpiDev_Init(3);
}

static int32_t  SpiDev_Ch3_UnInit(void)
{
    return SpiDev_UnInit(3);
}

static int32_t  SpiDev_Ch3_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(3, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch3_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(3, Baudrate);
}

static int32_t  SpiDev_Ch3_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(3, pData, pSize);
}

static int32_t  SpiDev_Ch3_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(3, pData, pSize);
}


/*
 *  Channel Function for Ch4
 */
static int32_t  SpiDev_Ch4_Init(void)
{
    return SpiDev_Init(4);
}

static int32_t  SpiDev_Ch4_UnInit(void)
{
    return SpiDev_UnInit(4);
}

static int32_t  SpiDev_Ch4_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(4, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch4_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(4, Baudrate);
}

static int32_t  SpiDev_Ch4_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(4, pData, pSize);
}

static int32_t  SpiDev_Ch4_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(4, pData, pSize);
}


/*
 *  Channel Function for Ch5
 */
static int32_t  SpiDev_Ch5_Init(void)
{
    return SpiDev_Init(5);
}

static int32_t  SpiDev_Ch5_UnInit(void)
{
    return SpiDev_UnInit(5);
}

static int32_t  SpiDev_Ch5_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(5, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch5_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(5, Baudrate);
}

static int32_t  SpiDev_Ch5_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(5, pData, pSize);
}

static int32_t  SpiDev_Ch5_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(5, pData, pSize);
}


/*
 *  Channel Function for Ch6
 */
static int32_t  SpiDev_Ch6_Init(void)
{
    return SpiDev_Init(6);
}

static int32_t  SpiDev_Ch6_UnInit(void)
{
    return SpiDev_UnInit(6);
}

static int32_t  SpiDev_Ch6_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(6, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch6_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(6, Baudrate);
}

static int32_t  SpiDev_Ch6_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(6, pData, pSize);
}

static int32_t  SpiDev_Ch6_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(6, pData, pSize);
}


/*
 *  Channel Function for Ch7
 */
static int32_t  SpiDev_Ch7_Init(void)
{
    return SpiDev_Init(7);
}

static int32_t  SpiDev_Ch7_UnInit(void)
{
    return SpiDev_UnInit(7);
}

static int32_t  SpiDev_Ch7_BufTxRx(void *pDataTx, void *pDataRx, uint32_t Size)
{
    return SpiDev_BufTxRx(7, pDataTx, pDataRx, Size);
}

static int32_t  SpiDev_Ch7_SetBaudrate(int32_t Baudrate)
{
    return SpiDev_SetBaudrate(7, Baudrate);
}

static int32_t  SpiDev_Ch7_DataTx(void *pData, int32_t *pSize)
{
    return SpiDev_DataTx(7, pData, pSize);
}

static int32_t  SpiDev_Ch7_DataRx(void *pData, int32_t *pSize)
{
    return SpiDev_DataRx(7, pData, pSize);
}

