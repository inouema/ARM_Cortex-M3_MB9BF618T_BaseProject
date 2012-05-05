/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  Sample UART
 *  
 *  This program is the sample program communicated with the terminal software
 *  of PC. 
 */

/*=============================================================================
   [NOTE] Connected chart
-------------------------------------------------------------------------------
     PC
   +-----------------------+
   | Baudrate : 115200 bps |
   | DataBit  : 8 bit      |
   | Parity   : None       | <-----> MFS Ch4@MB9Bxxx/MB9Axxx Board
   | StopBit  : 1 bit      |
   | FlowCtl  : None       |
   +-----------------------+

=============================================================================*/

#include "mcu.h"
#include "UartDev.h"
#include "debug.h"
#include "clock_def.h"

/*
 *  Extern IO Block
 */
extern UartDev_IOB UartDev[];

/*
 *  Use channel (0 to 7)
 *  (When user wants to use channel 0 to 3, please define "USE_CH03" in compile option.)
 */
#ifdef USE_CH03
#define UART_CH           0
#else
#define UART_CH           4
#endif

/*
 *  Wait count
 */
#define UART_WAIT_COUNT  (SysFreHCLK / 20)

/*
 *  Internal Function
 */
static void InitPort(void);

/*
 *  sample_uart
 */
void sample_uart(void)
{
    UartDev_IOB *uart;
    int32_t size;
    int32_t i;
    int32_t ret = SUCCESS;
    int32_t flag_NG;
    
    uint8_t tx_buf_press_key[] = "Press key(q or Q:End) ---->";
    uint8_t tx_buf_end[]       = "End\r\n";
    uint8_t tx_buf_crlf[]      = "\r\n";
    uint8_t rx_buf[10];
    
    InitPort();
    
    /* Initialized */
    uart = &UartDev[UART_CH];
    
    // When UART Driver is initialized in retarget_io.c, 
    // the following are skipped.
    uart->Cfg.BitOrder = UartDev_BITORDER_LSB;
    uart->Cfg.BufSize  = 128;  //1024;
    ret = uart->Init();
    
#ifdef PRINTF_DEBUG  
    PRINTF("\n=====================================================\n");
#endif    
    if (ret == SUCCESS) {
#ifdef PRINTF_DEBUG 
        PRINTF("UART Driver Sample Program \n");
        PRINTF("[Note] Please connect PC and Ch%d of EvaBoard. \n", UART_CH);
#endif 
        flag_NG = 0;
#ifdef PRINTF_DEBUG         
        PRINTF("\n  Cfg Information \n");
        PRINTF("    Baudrate = %d \n", uart->Cfg.BaudRate);
        PRINTF("    DataBits = %d \n", uart->Cfg.DataBits);
        PRINTF("    StopBits = %d \n", uart->Cfg.StopBits);
#endif
        if (uart->Cfg.Parity == UartDev_PARITY_NONE) {
#ifdef PRINTF_DEBUG 
            PRINTF("    Parity   = NONE \n");
#endif
        } else if (uart->Cfg.Parity == UartDev_PARITY_ODD) {
#ifdef PRINTF_DEBUG 
            PRINTF("    Parity   = ODD \n");
#endif
        } else {
#ifdef PRINTF_DEBUG 
            PRINTF("    Polarity = EVEN \n");
#endif
        }
        if (uart->Cfg.BitOrder == UartDev_BITORDER_MSB) {
#ifdef PRINTF_DEBUG 
            PRINTF("    BitOrder = MSB \n");
#endif
        } else {
#ifdef PRINTF_DEBUG 
            PRINTF("    BitOrder = LSB \n");
#endif
        }
#ifdef PRINTF_DEBUG 
        PRINTF("    BufSize  = %d \n", uart->Cfg.BufSize);
#endif        
        /* 
         * Write
         */
        size = sizeof(tx_buf_press_key);
        uart->BufTx(tx_buf_press_key, &size, UartDev_FLAG_BLOCKING);
        
        while (1) {
            /*
             * Read
             */
            for (i = 0; i < UART_WAIT_COUNT; i++) {
                size = 1;
                uart->BufRx(rx_buf, &size, UartDev_FLAG_NONBLOCKING);
                
                if (size == 1) {
                    flag_NG = 0;
                    break;
                } else {
                    flag_NG = 1;
                }
            }
            
            if (flag_NG == 0) {
                size = 1;
                uart->BufTx(rx_buf, &size, UartDev_FLAG_BLOCKING);
                size = sizeof(tx_buf_crlf);
                uart->BufTx(tx_buf_crlf, &size, UartDev_FLAG_BLOCKING);
                if (rx_buf[0] == 'q' || rx_buf[0] == 'Q') {
                    size = sizeof(tx_buf_end);
                    uart->BufTx(tx_buf_end, &size, UartDev_FLAG_BLOCKING);
                    break;
                }
#ifdef PRINTF_DEBUG             
                PRINTF("\n\n[SUCCESS] Read/Write \n");
#endif
                size = sizeof(tx_buf_press_key);
                uart->BufTx(tx_buf_press_key, &size, UartDev_FLAG_BLOCKING);
            } else {
#ifdef PRINTF_DEBUG             
                PRINTF("\n\n[ERROR] Read/Write \n");
#endif
                break;
            }
        }
    }
#ifdef PRINTF_DEBUG     
    PRINTF("=====================================================\n");
#endif
    /* Uninitialized */
    /*
    uart->UnInit();
    */
}

/*
 *  InitPort
 */
static void InitPort(void)
{
    /*
     * Set I/O Port
     * // Port Number
     * SIN0_0:P21, SIN1_0:P56, SIN2_0:P72, SIN3_0:P75, SIN4_0:PD2, SIN5_0:P60, SIN6_0:P53, SIN7_0:P59
     * SOT0_0:P22, SOT1_0:P57, SOT2_0:P73, SOT3_0:P76, SOT4_0:PD1, SOT5_0:P61, SOT6_0:P54, SOT7_0:P5A
     * 
     * // Pin Number(LQFP176)
     * SIN0_0:126, SIN1_0:019, SIN2_0:067, SIN3_0:070, SIN4_0:165, SIN5_0:169, SIN6_0:016, SIN7_0:022
     * SOT0_0:125, SOT1_0:020, SOT2_0:068, SOT3_0:071, SOT4_0:164, SOT5_0:168, SOT6_0:017, SOT7_0:023
     */
    /*
    FM3_GPIO->ADE = 0x00000000;
    FM3_GPIO->PFR2 = FM3_GPIO->PFR2 | 0x0006;
    FM3_GPIO->PFR5 = FM3_GPIO->PFR5 | 0x06D8;
    FM3_GPIO->PFR6 = FM3_GPIO->PFR6 | 0x0003;
    FM3_GPIO->PFR7 = FM3_GPIO->PFR7 | 0x006C;
    FM3_GPIO->PFRD = FM3_GPIO->PFRD | 0x0006;
    
    FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x05145140;
    FM3_GPIO->EPFR08 = FM3_GPIO->EPFR08 | 0x05145140;
    */
#ifdef USE_CH03
    /* Set Uart Ch0 Port */
    FM3_GPIO->ADE = 0x00000000;
    FM3_GPIO->PFR2 = FM3_GPIO->PFR2 | 0x0006;
    FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x00000140;
#else
    /* Set Uart Ch4 Port */
    FM3_GPIO->PFRD = FM3_GPIO->PFRD | 0x0006;
    FM3_GPIO->EPFR08 = FM3_GPIO->EPFR08 | 0x00000140;
#endif
}
