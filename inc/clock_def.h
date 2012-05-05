/*
 *  Copyright (C) 2012 FUJITSU SEMICONDUCTOR LIMITED
 *  Clock Define
 */
#ifndef _CLOCK_DEF_H_
#define _CLOCK_DEF_H_

/* 
 *  144MHz : Master Clock
 */
# if 1
#define SystemFrequency 144000000
#define SysFreHCLK      144000000 /* HCLK = MasterClock / 1 */
#define SysFrePCLK0     72000000  /* PCLK0 = HCLK / 2 */
#define SysFrePCLK1     72000000  /* PCLK1 = HCLK / 2 */
#define SysFrePCLK2     72000000  /* PCLK2 = HCLK / 2 */
#define SysFreTPIU      0 /* TPIUCLK : Disable */
#endif

#endif /* _CLOCK_DEF_H_ */
