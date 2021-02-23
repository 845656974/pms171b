/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-1999, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
*                                          VC++6.0
*
* File         : OS_CPU.H
* By           : Jean J. Labrosse
  Changed by   : luyl Email： luyl@uestc.edu.cn
*********************************************************************************************************
*/

/*
*********************************************************************************************************
* 
author:1949KJY
time:20191608
qq:2692377302
https://item.taobao.com/item.htm?spm=a1z10.1-c.w4004-18323443223.14.456657b1N7KwE0&id=580551386625
*                                     
*********************************************************************************************************
*/
#ifndef __OS_CPU_H
#define __OS_CPU_H

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                       数据类型
*                                     (编译器相关)
*********************************************************************************************************
*/


typedef unsigned char  BOOLEAN;       //注意：不要使用bit定义，因为在结构体里无法使用
typedef unsigned char  INT8U;         //无符号8位数
typedef signed   char  INT8S;         //有符号8位数
typedef unsigned int   INT16U;        //无符号16位数
typedef signed   int   INT16S;        //有符号16位数
typedef unsigned long  INT32U;        //无符号32位数
typedef signed   long  INT32S;        //有符号32位数
typedef float          FP32;          //单精度浮点数
typedef double         FP64;          //双精度浮点数

typedef unsigned char  OS_STK;        //栈单元宽度为8比特
typedef unsigned char  OS_CPU_SR;     /* Define size of CPU status register (PSW = 8 bits)  */

#define BYTE           INT8S          //兼容以前版本的数据类型
#define UBYTE          INT8U          //uC/OS-II可以不用这些数据类型
#define WORD           INT16S
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

/* 
*********************************************************************************************************
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD    1

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  EA=0    //关中断
#define  OS_EXIT_CRITICAL()   EA=1    //开中断
#endif

#if      OS_CRITICAL_METHOD == 2
/* As an undocumented keyword of keil c. __asm is supported in Keil C v6.20.
. No other means to define assemble language code in a macro, I have to use it here. If your compiler does not support __asm, use method 1 or 3 then. */
/* A2 AF MOV C, EA*/
/* C2 AF CLR EA   */
/* C0 D0 PUSH PSW */
#define  OS_ENTER_CRITICAL()    __asm DB 0A2H, 0AFH, 0C2H, 0AFH, 0C0H, 0D0H

/* D0 D0 POP PSW   */
/* 92 AF MOV EA, C */
#define  OS_EXIT_CRITICAL()     __asm DB 0D0H, 0D0H, 092H, 0AFH
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = EA, EA=0)   /* Disable interrupts  */
#define  OS_EXIT_CRITICAL()   (EA=cpu_sr)    		/* Enable  interrupts  */
#endif

/*
*********************************************************************************************************
*                           Intel 80x86 (Real-Mode, Large Model) Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH    0           //MCU-51堆栈从下往上增长  1=向下，0=向上

#define  OS_TASK_SW()     OSCtxSw()		//保护模式下不能中断


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/



#define OS_ISR_PROTO_EXT	1
void OSCtxSw(void);
void InitHardware(void) reentrant;    //初始化硬件时钟中断等，系统初始化工作

#endif //_OS_CPU_H

/*
*********************************************************************************************************
* 
author:1949KJY
time:20191608
qq:2692377302
https://item.taobao.com/item.htm?spm=a1z10.1-c.w4004-18323443223.14.456657b1N7KwE0&id=580551386625
*                                     
*********************************************************************************************************
*/