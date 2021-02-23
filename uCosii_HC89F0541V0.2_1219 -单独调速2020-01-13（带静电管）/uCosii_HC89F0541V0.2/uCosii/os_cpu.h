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
  Changed by   : luyl Email�� luyl@uestc.edu.cn
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
*                                       ��������
*                                     (���������)
*********************************************************************************************************
*/


typedef unsigned char  BOOLEAN;       //ע�⣺��Ҫʹ��bit���壬��Ϊ�ڽṹ�����޷�ʹ��
typedef unsigned char  INT8U;         //�޷���8λ��
typedef signed   char  INT8S;         //�з���8λ��
typedef unsigned int   INT16U;        //�޷���16λ��
typedef signed   int   INT16S;        //�з���16λ��
typedef unsigned long  INT32U;        //�޷���32λ��
typedef signed   long  INT32S;        //�з���32λ��
typedef float          FP32;          //�����ȸ�����
typedef double         FP64;          //˫���ȸ�����

typedef unsigned char  OS_STK;        //ջ��Ԫ���Ϊ8����
typedef unsigned char  OS_CPU_SR;     /* Define size of CPU status register (PSW = 8 bits)  */

#define BYTE           INT8S          //������ǰ�汾����������
#define UBYTE          INT8U          //uC/OS-II���Բ�����Щ��������
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
#define  OS_ENTER_CRITICAL()  EA=0    //���ж�
#define  OS_EXIT_CRITICAL()   EA=1    //���ж�
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

#define  OS_STK_GROWTH    0           //MCU-51��ջ������������  1=���£�0=����

#define  OS_TASK_SW()     OSCtxSw()		//����ģʽ�²����ж�


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
void InitHardware(void) reentrant;    //��ʼ��Ӳ��ʱ���жϵȣ�ϵͳ��ʼ������

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