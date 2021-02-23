/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                           All Rights Reserved
*
*                                           MASTER INCLUDE FILE
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

#ifndef __INCLUDES__
#define __INCLUDES__

#include    "HC89F0541.h"
#include    "uCosii\os_cpu.h"
#include    "uCosii\os_cfg.h"
#include    "uCosii\ucos_ii.h"




#define buzzer P3_2		//������		1��0��
#define G_led	 P3_0		//��LED			0��1��
#define R_led	 P3_1		//��LED			0��1��
#define fan	   P1_5 	//���
#define Brush  P3_3		//��ˢ
#define Wifi	P0_7 		//wifiʹ��
#define	floor	P2_1		//�ؼ�ʹ��
#define mapan P0_6		//����
sbit L1_touch = P1^3; //����		1����0̧��
sbit R1_touch = P1^4; //�Ҵ���		1����0̧��
sbit crash = P1^0;		//��ײ			1��ײ


sbit power = P3^4;//��������	0��� 1δ���
sbit power_enabled =P2^0; //����� 1Ϊ����


#endif
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