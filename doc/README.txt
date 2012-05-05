Defined in system_mb9bf61x.h
====================================
External Main CLK : 4MHz
External Sub  CLK : 32kHz

動作クロックはMainCLK 4MHzをPLLで逓倍した値

PLLCLK = CLKMO * PLLN / PLLK
       = 4MHz  * 36 / 1 = 144MHz

UARTはAPB2バスに接続されている
APB2バスのクロックは

HCLK/2 = 144MHz/2 = 72MHz

