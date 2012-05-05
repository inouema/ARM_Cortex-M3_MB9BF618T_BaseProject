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


/* Includes ------------------------------------------------------------------*/

#include "mcu.h"
#include "UartDev.h"
//#include "debug.h"
//#include "clock_def.h"


/* Variables -----------------------------------------------------------------*/
extern UartDev_IOB UartDev[];
static UartDev_IOB *uart;
static f_rx_buf[1];


/* Defines -------------------------------------------------------------------*/
//#define UART_WAIT_COUNT  (SysFreHCLK / 20)

/* Internal function prototypes ----------------------------------------------*/
static void InitPort(void);


/* Internal Functions --------------------------------------------------------*/
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

	/*
	 * SIN3_2 Pin58 Port P48
	 * SOT3_2 Pin59 Port P49
	 */
	FM3_GPIO->ADE    = 0x00000000;                    /* 0:外部端子をアナログ入力として使用せずデジタル入出力として使用する.*/
	FM3_GPIO->PFR4   = FM3_GPIO->PFR4   | 0x0300;     /* P48,P49を周辺機能端子として使用する (bit8,bit9) */
	FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x03C00000; /* MFS ch3 SOT3B/SIN3Sに SOT3_2端子(bit25:24), SIN3_2端子(bit23:22)を使用する.*/


}


/* Functions -----------------------------------------------------------------*/
/**
 * @brief   Send 1 character
 * @details
 * @param
 * @retval
 */
void UartPutc(uint8_t data)
{
	int32_t size = 1;
	uart->BufTx(&data, &size, UartDev_FLAG_BLOCKING);
}

/**
 * @brief   Receive 1 character
 * @details
 * @param
 * @retval
 */
uint8_t UartGetc(void)
{
	int32_t size = 1;
	uart->BufRx(&f_rx_buf[0], &size, UartDev_FLAG_NONBLOCKING);

	return f_rx_buf[0];
}


/**
 * @brief   Initialize Uart module
 * @details
 * @param
 * @retval
 */
void UartInit(void)
{
    int32_t ret = SUCCESS;

    /* For Using UART */
    InitPort();

    /* Initialized */
    uart = &UartDev[UART_CH];

    // When UART Driver is initialized in retarget_io.c, 
    // the following are skipped.
    uart->Cfg.BitOrder = UartDev_BITORDER_LSB;
    uart->Cfg.BufSize  = 128;  //1024;
    ret = uart->Init();

}



/* End Of File ---------------------------------------------------------------*/

