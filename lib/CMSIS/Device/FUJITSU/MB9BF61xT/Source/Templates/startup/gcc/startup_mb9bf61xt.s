/********************************************************************************/
/*!
	@file			startup_mb9bf61xt.s
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2012.02.02
	@brief          MB9BF61xT's Startup ASM Code									@n
					Based On STM32F107VCT6's Startup

    @section HISTORY
		2012.02.02	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

	.syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */  
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss


/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section	.boot,"ax",%progbits
	.weak	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:	

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4

LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss
 
/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call the application's entry point.*/
  bl  main
  bx  lr    

.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section	.text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/    
 	.section .isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word   _estack		                /* Top of Stack                 					*/
    .word   Reset_Handler               /* Reset Handler                					*/
    .long   NMI_Handler                 /* NMI Handler                  					*/
    .long   HardFault_Handler           /* Hard Fault Handler           					*/
    .long   MemManage_Handler           /* MPU Fault Handler            					*/
    .long   BusFault_Handler            /* Bus Fault Handler            					*/
    .long   UsageFault_Handler          /* Usage Fault Handler          					*/
    .long   0xEFFEFA5B      		    /* Valid User Code              					*/
    .long   0                           /* Reserved                     					*/
    .long   0                           /* Reserved                     					*/
    .long   0                           /* Reserved                     					*/
    .long   SVC_Handler                 /* SVCall Handler               					*/
    .long   DebugMon_Handler            /* Debug Monitor Handler        					*/
    .long   0                           /* Reserved                     					*/
    .long   PendSV_Handler              /* PendSV Handler               					*/
    .long   SysTick_Handler             /* SysTick Handler              					*/

    /* External Interrupts */
    .word	CSV_Handler            		/*  Clock Super Visor                            	*/
    .word	SWDT_Handler            	/*  Software Watchdog Timer							*/
    .word	LVD_Handler               	/*  Low Voltage Detector               				*/
    .word	MFT_WG_IRQHandler           /*  Wave Form Generator / DTIF              	 	*/
    .word	INT0_7_Handler            	/*  External Interrupt Request ch.0 to ch.7         */
    .word	INT8_31_Handler            	/*  External Interrupt Request ch.8 to ch.31        */
    .word	DT_Handler               	/*  Dual Timer / Quad Decoder              		 	*/
    .word	MFS0RX_IRQHandler           /*  MultiFunction Serial ch.0               		*/
    .word	MFS0TX_IRQHandler         	/*	MultiFunction Serial ch.0               		*/
    .word	MFS1RX_IRQHandler         	/*	MultiFunction Serial ch.1               		*/
    .word	MFS1TX_IRQHandler         	/*	MultiFunction Serial ch.1               		*/
    .word	MFS2RX_IRQHandler         	/*	MultiFunction Serial ch.2               		*/
    .word	MFS2TX_IRQHandler         	/*	MultiFunction Serial ch.2               		*/
    .word	MFS3RX_IRQHandler         	/*	MultiFunction Serial ch.3               		*/
    .word	MFS3TX_IRQHandler        	/*	MultiFunction Serial ch.3               		*/
    .word	MFS4RX_IRQHandler        	/*	MultiFunction Serial ch.4               		*/
    .word	MFS4TX_IRQHandler         	/*	MultiFunction Serial ch.4               		*/
    .word	MFS5RX_IRQHandler         	/*	MultiFunction Serial ch.5               		*/
    .word	MFS5TX_IRQHandler         	/*	MultiFunction Serial ch.5               		*/
    .word	MFS6RX_IRQHandler        	/*	MultiFunction Serial ch.6               		*/
    .word	MFS6TX_IRQHandler        	/*	MultiFunction Serial ch.6               		*/
    .word	MFS7RX_IRQHandler         	/*	MultiFunction Serial ch.7               		*/
    .word	MFS7TX_IRQHandler         	/*	MultiFunction Serial ch.7               		*/
    .word	PPG_Handler               	/*	PPG               								*/
    .word	TIM_IRQHandler            	/*	OSC / PLL / Watch Counter               		*/
    .word	ADC0_IRQHandler           	/*	ADC0               								*/
    .word	ADC1_IRQHandler           	/*	ADC1               								*/
    .word	ADC2_IRQHandler           	/*	ADC2               								*/
    .word	MFT_FRT_IRQHandler       	/*	Free-run Timer              			 		*/
    .word	MFT_IPC_IRQHandler        	/*	Input Capture               					*/
    .word	MFT_OPC_IRQHandler        	/*	Output Compare               					*/
    .word	BT0_7_IRQHandler            /*	Base Timer ch.0 to ch.7               			*/
    .word	ETHER_MAC0_IRQHandler       /*	Ethernet MAC ch.0								*/
    .word	ETHER_MAC1_IRQHandler       /*	Ethernet MAC ch.1								*/
    .word	USB0F_Handler              	/*	USB0 Function									*/
    .word	USB0_Handler               	/*	USB0 Function / USB0 HOST          				*/
    .word	USB1F_Handler              	/*	USB1 Function                    				*/
    .word	USB1_Handler               	/*	USB1 Function / USB1 HOST                    	*/
    .word	DMAC0_Handler             	/*	DMAC ch.0                                       */
    .word	DMAC1_Handler             	/*	DMAC ch.1                                       */
    .word	DMAC2_Handler             	/*	DMAC ch.2                                       */
    .word	DMAC3_Handler             	/*	DMAC ch.3                                       */
    .word	DMAC4_Handler             	/*	DMAC ch.4                                       */
    .word	DMAC5_Handler             	/*	DMAC ch.5                                       */
    .word	DMAC6_Handler             	/*	DMAC ch.6                                       */
    .word	DMAC7_Handler             	/*	DMAC ch.7                                       */
    .word	BT8_15_IRQHandler            /*	Base Timer ch.8 to ch.15               			*/
    .word	DummyHandler              	/*	Reserved                                      	*/


/* Exception Handlers */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler


/* IRQ Handlers */

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

    IRQ     CSV_Handler
    IRQ     SWDT_Handler
    IRQ     LVD_Handler
    IRQ     MFT_WG_IRQHandler
    IRQ     INT0_7_Handler
    IRQ     INT8_31_Handler
    IRQ     DT_Handler
    IRQ     MFS0RX_IRQHandler
    IRQ     MFS0TX_IRQHandler
    IRQ     MFS1RX_IRQHandler
    IRQ     MFS1TX_IRQHandler
    IRQ     MFS2RX_IRQHandler
    IRQ     MFS2TX_IRQHandler
    IRQ		MFS3RX_IRQHandler
    IRQ     MFS3TX_IRQHandler
    IRQ     MFS4RX_IRQHandler
    IRQ     MFS4TX_IRQHandler
    IRQ     MFS5RX_IRQHandler
    IRQ     MFS5TX_IRQHandler
    IRQ     MFS6RX_IRQHandler
    IRQ     MFS6TX_IRQHandler
    IRQ     MFS7RX_IRQHandler
    IRQ     MFS7TX_IRQHandler
    IRQ     PPG_Handler
    IRQ     TIM_IRQHandler
    IRQ     ADC0_IRQHandler
    IRQ     ADC1_IRQHandler
    IRQ     ADC2_IRQHandler
    IRQ     MFT_FRT_IRQHandler
    IRQ     MFT_IPC_IRQHandler
    IRQ     MFT_OPC_IRQHandler
    IRQ     BT0_7_IRQHandler
    IRQ     DMAC0_Handler
    IRQ		DMAC1_Handler
    IRQ		DMAC2_Handler
	IRQ		DMAC3_Handler
	IRQ		DMAC4_Handler
	IRQ		DMAC5_Handler
	IRQ		DMAC6_Handler
	IRQ		DMAC7_Handler
	IRQ		ETHER_MAC0_IRQHandler
	IRQ		ETHER_MAC1_IRQHandler
	IRQ		USB0F_Handler
	IRQ	  	USB0_Handler
	IRQ	  	USB1F_Handler
	IRQ	  	USB1_Handler
	IRQ	 	BT8_15_IRQHandler
	IRQ	  	DummyHandler

.end
