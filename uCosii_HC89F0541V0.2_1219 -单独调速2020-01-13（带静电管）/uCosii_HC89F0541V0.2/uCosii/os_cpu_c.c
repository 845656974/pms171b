/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                         (c) Copyright 1992-2001, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*
*                                       80x86/80x88 Specific code
*                                          PROTECTED MEMORY MODEL
*
*                                          vc++6.0
*
* File         : OS_CPU_C.C
* By           : Jean J. Labrosse
*********************************************************************************************************
*/
#ifndef __INCLUDES__
#include "source\includes.h"
#endif
/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*			   ��console��û���жϵ��ã�Ӧ��Ҳû���жϷ��ص�ַ��vc�����ı���ģʽ�µĶε�ַû�б��
*				Ӧ��Ҳ����ѹջ�����������Ҳû�аѸ���Ĵ���ѹջ
*				
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable 
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/
void Delay_2us(unsigned int fui_i);		//��ʱ����
OS_STK *OSTaskStkInit (void (*task)(void *pd) reentrant, void *ppdata, OS_STK *ptos, INT16U opt) reentrant
{    
    OS_STK *stk;

    ppdata = ppdata;
    opt    = opt;                               //optû���õ�������������ֹ�澯����    
    stk    = ptos;                              //�û���ջ�����Ч��ַ
    *stk++ = 15;                                //�û���ջ����
    *stk++ = (INT16U)task & 0xFF;               //�����ַ��8λ
    *stk++ = (INT16U)task >> 8;                 //�����ַ��8λ    
    *stk++ = 0x0A;                              //ACC
    *stk++ = 0x0B;                              //B
    *stk++ = 0x00;                              //DPH
    *stk++ = 0x00;                              //DPL
    *stk++ = 0x00;                              //PSW
    *stk++ = 0x00;                              //R0
    
	//R3��R2��R1���ڴ����������ppdata������R3����洢�����ͣ�R2Ϊ���ֽ�ƫ�ƣ�R1Ϊ���ֽ�λ�ơ�
	//ͨ������KEIL��࣬�˽⵽�����void *ppdata����ǡ������R3��R2��R1���ݣ�����ͨ�������ջ��
    *stk++ = (INT16U)ppdata & 0xFF;             //R1
    *stk++ = (INT16U)ppdata >> 8;               //R2
    *stk++ = 0x01;                              //R3  ��Ϊ���õ�ȫ��XDATA�����Դ洢�����͹̶�Ϊ1����C51.PDF��178ҳ˵����

    *stk++ = 0x04;                              //R4
    *stk++ = 0x05;                              //R5
    *stk++ = 0x06;                              //R6
    *stk++ = 0x07;                              //R7
                                                //���ñ���SP�������л�ʱ�����û���ջ���ȼ���ó���    
    *stk++ = (INT16U) (ptos+MaxStkSize) >> 8;   //?C_XBP �����ջָ���8λ
    *stk++ = (INT16U) (ptos+MaxStkSize) & 0xFF; //?C_XBP �����ջָ���8λ
        
    return ((void *)ptos);
}
/*
   ʹ��C���Ե��жϴ�������������߳������ֲ�ԡ������жϳ���Ҫ̫�����������ʹ���ź�����������ͬ����
   ���ⲿ������ʵ�ִ����Ĵ���
   �жϴ������̶��������档
*/

void UserTickTimer(void)
{
    TH0=0xB8;    		//��ͨ51��ʱ����ʽ1�������ڷ����ж�ʱ�����¸�ֵ���ٴ�������ʱ
    TL0=0;            	//Tick=50��/��(��0.02��/��)������11.0592M
    TR0=1;
}

/* 
  uCOS-IIϵͳʱ���жϴ������
*/
void OSTickISRuser(void) interrupt 1
{
    OSIntEnter();					// Must be called first at every hardware interrupt entry point 
    UserTickTimer();				// User functions can be called here.
   	OSTimeTick();					// Must be called during tick isr 
   	OSIntExit();					// Must be called finally at every hardware interupt exit point 
}

void InitHardware(void) reentrant
{   
  CLKSWR = 0x51;						//ѡ���ڲ���ƵRCΪϵͳʱ�ӣ��ڲ���ƵRC 2��Ƶ��Fosc=16MHz
	CLKDIV = 0x01;						//Fosc 1��Ƶ�õ�Fcpu��Fcpu=16MHz
	
//  CLKSWR = 0x50;						//ѡ���ڲ���ƵRCΪϵͳʱ�ӣ��ڲ���ƵRC 2��Ƶ��Fosc=32MHz
//	CLKDIV = 0x01;						//Fosc 1��Ƶ�õ�Fcpu��Fcpu=32MHz
	
	
		TMOD &= 0xF0;
		TMOD |= 0x01;   //��ʱ��0��ģʽ1(16λ��ʱ��)������TR0���ƣ���ʱ��1�������ʷ�����
    TH0  = 0xB8;   //b8����Tick=50��/��(��0.02��/��),TH,TLֵ��CPU��Ƶ���й�(11.0592M)!!!!!gai!!!!!!
    TL0  = 0x00;   //00  OS_CPU_C.C�ж�ʱ���ж���ӦҲҪ���ã�OS_CFG.H��OS_TICKS_PER_SECҲ�й�ϵ
    ET0  = 1;    //����T0�ж�(�ڵ�һ������ʼִ��ʱ�ſ�ʱ���ж�,������һ�ж�ϵͳ���벻��֪״̬)
    TR0  = 1;
		TCON1 |= 0x01;						//T0ʱ��1��Ƶ

	
	/*************************************PWM��ʼ��****************************************/
    P2M3 = 0xC2;                        //P23����Ϊ�������
    P2M2 = 0xC2;                        //P22����Ϊ�������
    P1M1 = 0xC2;                        //P11����Ϊ�������
    P1M2 = 0xC2;                        //P12����Ϊ�������	
	
		PWM0_MAP = 0x23;					//PWM0ͨ��ӳ��P23��
		PWM01_MAP = 0x22;					//PWM01ͨ��ӳ��P22��
    PWM0C = 0x01;						//PWM0����Ч��PWM01����Ч��ʱ��8��Ƶ 
		
		
		
		PWM1_MAP = 0x11;					//PWM0ͨ��ӳ��P11��
		PWM11_MAP = 0x12;					//PWM01ͨ��ӳ��P12��
    PWM1C = 0x01;						//PWM1����Ч��PWM11����Ч��ʱ��8��Ƶ 
		
		
		
	/*************************************���****************************************/
	P1M5 = 0xC2;                        //P15����Ϊ�������
//	P1_5 = 0;

		PWM2_MAP = 0x15;					//PWM2ͨ��ӳ��P15��
	  PWM2C = 0x01;						//PWM2����Ч��PWM15����Ч��ʱ��8��Ƶ 


    PWMM |= 0x77;						//PWM0��PWM1�����ڻ���ģʽ					
	//����ģʽ�£�PWM0��PWM01����һ�����ڼĴ���
	//PWM0��ռ�ձȵ���ʹ��			PWM0���ռ�ձȼĴ���
	//PWM01��ռ�ձȵ���ʹ��			PWM0��������Ĵ���

	//���ڼ��� 	= 0x03ff / (Fosc / PWM��Ƶϵ��)		��Fosc��ϵͳʱ�����õĲ��֣�
	//			= 0x03ff / (16000000 / 8)			
	// 			= 1023   /2000000
	//			= 511.5us		   		Լ1.955kHz
//  	OSSemCreate(PCF8563Sem, 1);
//	task2();

//	P2M3 = 0xC0;                        //P26����Ϊ�������
//	P2_3 = 0;
	
	
	
	/*************************************��������****************************************/
		P1M3 = 0x54;                        //P05����Ϊ��ʩ�������ִ���������
		P1M4 = 0x54;                        //P34����Ϊ��ʩ�������ִ���������
	
	/*************************************��ײ****************************************/
		P1M0 = 0x50;                        //P10����Ϊ��ʩ�������ִ���������

	/*************************************wifi****************************************/
		P0M7 = 0xC2;                        //P07����Ϊ�������,wifi
		P0_7 = 1;														//wifiʹ��
	/*************************************�ؼ�****************************************/
		P2_1 = 1;														//�ؼ�ʹ��
		P2M1 = 0xC2;                        //P21����Ϊ�������
		
//		P1M3 = 0xC2;                        //P13����Ϊ�������
//		P0M4 = 0x68;                        //P05����Ϊ��SMT��������
//		PITS1 |= 0x11;						//INT4�½���
//		PINTE0 |= 0x10;						//ʹ��INT4
//		IE1 |= 0x80;							//��INT2-7�ж�
		
//		/**********************************INT1���ó�ʼ��**************************************/
//		P0M4 = 0x69;			        	//P04����Ϊ��SMT��������
//		PITS1 |= 0x01;						//INT4�½���
//		IE1 |= 0x80;							//��INT4�ж�
		

//	
	/*************************************������****************************************/
		P3M2 = 0xC2;                        //P32����Ϊ�������
	
	/*************************************LED��****************************************/
		P3M0 = 0xC2;                        //P32����Ϊ�������,��LED
		P3M1 = 0xC2;                        //P32����Ϊ�������,��LED
		P3_0 = 1;
		P3_1 = 1;


	/*************************************��ˢ****************************************/
	P3M3 = 0xC2;                        //P15����Ϊ�������		
	
	
	
	/*****************5V����ʹ��**************/
	P0M6 = 0xC2;                        //P06����Ϊ�������
	P0_6 = 1;														//5V����ʹ��
	


/*****************���**************/
//	P2M0 = 0xC2;
	P2M0 = 0x60;
//	P2_0 = 1;														//���ʹ��
	P3M4 = 0x60;                        //P14����Ϊ��ʩ�������ִ���������


	/*********************ADC*************************/
	P0M2 = 0x00;                        //P05����Ϊģ������,�ҵؼ�58
	P0M3 = 0x00;                        //P03����Ϊģ������,ǰ�ؼ�
	P0M4 = 0x00;                        //P04����Ϊģ������,��ؼ�
		
	P0M1 = 0x02;				        //P01����Ϊģ������,����ADC
	P0M0 = 0x02;				        //P00����Ϊģ������,����ADC
	P2M7 = 0x02;				        //P27����Ϊģ������,���ADC
	P2M5 = 0x02;				        //P25����Ϊģ������,��ˢADC
	P2M6 = 0x02;                //P26����Ϊģ������,��ԴADC

	ADCC0 = 0x81;						//��ADCת����Դ
	
	Delay_2us(20);						//��ʱ20us��ȷ��ADCϵͳ�ȶ�
	ADCC2 = 0x4B;						//ת�����12λ���ݣ������Ҷ��룬ADCʱ��16��Ƶ
//  SCANCON = 0x1A;                     //��ͨ��ɨ��ģʽ��ɨ������ͨ��
  SCCH0 = 0x10;                     	//��һ��ɨ��AN0���ڶ���ɨ��AN1
	
	
  SCCH1 = 0xDF;                     	//������ɨ��AN15�����ĸ�ɨ��AN13
	
	SCCH2 = 0x4E;												//�����ɨ��AN14��������ɨ��AN4
	SCCH3 = 0x23;												//���߸�ɨ��AN3���ڰ˸�ɨ����AN2
	
	SCANCON = 0x1F;                     //��ͨ��ɨ��ģʽ��ɨ��˸�ͨ��
		
		
		
  IE1 |= 0x40;                        //��ADC�ж�
	ADCC0 &=~ 0x20;						//���ADC�жϱ�־λ
  ADCC0 |= 0x40;
	
	
	
	
	
	
	
	
	
	/*****************��ʱ1��ȡ���ֱ�����**************/

//	P3M5 = 0x62;                        //P35����Ϊ����������
//	T1_MAP = 0x35;						//T0ӳ��ΪP02
//	TCON1 |= 0x10;						//T1ʱ��1��Ƶ
//	TMOD |= 0x40;						//����T1Ϊ������ģʽ
//	TH1 = 0xFF;
//	TL1 = 0xFF;							//1����
//	IE |= 0x08;							//��T1�ж�
//	TCON |= 0x40;						//����T1
	
	P3M5 = 0x69;			        	//P35����Ϊ��SMT��������
	PITS4 |= 0x04;						//INT17�½���
	PINTE2 |= 0x02;						//ʹ��INT17
	IE2 |= 0x01;							//��INT17�ж�
		

	/****************�ж϶�ȡ���ֱ�����*****************************/
	P2M4 = 0x6B;			        	//P24����Ϊʩ������������
  P2INTE |= 0x10;						//ʹ��P24�½����ж�
	IE2 |= 0x02;						//��P2���ж�
	EA = 1;								//�����ж�
	


	/***********************************WDT���ó�ʼ��(���Ź�)**************************************/
	WDTC = 0x17;						//����WDT��λ���������/����ģʽ�����У�1024��Ƶ

	//WDTʹ�õ���RC44Kʱ�ӣ�WDT_CLOCK_1024��1024��Ƶ����ֵΪ0xFF
	//��ʱʱ�� 	= 1024 * 0xFF / 44000
	//			= 5.93s

	WDTCCR = 0xFF;						//д��00ʱ�����ر�WDT���ܣ������ر��ڲ���ƵRC����
										//���൱�ڽ�ֹWDT��д���0����ʱ��������WDT��

/************************************************************************************/ 


/**********************************UART���ó�ʼ��**************************************/
	P1M6 = 0xC2;				        //P16����Ϊ�������
	P1M7 = 0x62;				        //P17����Ϊ�������
	TXD_MAP = 0x16;						//TXDӳ��P16
	RXD_MAP = 0x17;						//RXDӳ��P17		
	T4CON = 0x06;						//T4����ģʽ��UART1�����ʷ�����
	//�����ʼ���
	//������ = 1/16 * (T4ʱ��ԴƵ�� / ��ʱ��4Ԥ��Ƶ��) / (65536 - 0xFF30)
	//       = 1/16 * ((32000000 / 1) / 208)
	//		 = 9615.38(���0.16%)

	//������9600
	//���Ƴ�ֵ = (65536 - ((T4ʱ��ԴƵ�� / ��ʱ��4Ԥ��Ƶ��) * (1 / 16)) / ������)
	//		   = (65536 - (32000000 * (1 / 16) / 9600))
	//		   = (65536 - 208.33)
	//         = FF30
    
	TH4 = 0xFF;
	TL4 = 0x98;							//������9600,TL4 = 0x98;F7��115200	
	SCON2 = 0x02;						//8λUART�������ʿɱ�
	SCON = 0x10;						//�����н���
	IE |= 0x10;							//ʹ�ܴ����ж�
	EA = 1;								//ʹ�����ж�
/************************************************************************************/    	



   	


}

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void) reentrant
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void) reentrant
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb) reentrant
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_TASK_DEL_EN > 0
void OSTaskDelHook (OS_TCB *ptcb) reentrant
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif
/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void) reentrant
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_TASK_STAT_EN > 0
void OSTaskStatHook (void) reentrant
{
	//printf("CPU������=%d\n",OSCPUUsage);
}
#endif

#if OS_TASK_RETURN_EN >0
void   OSTaskReturnHook(OS_TCB          *ptcb) reentrant
{
	ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

#if OS_DEBUG_EN > 0u
void          OSDebugInit             (void) reentrant
{
}
#endif
/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb) reentrant
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.��ӡ��ʱ��Ϣ
				
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void) reentrant
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none 
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void) reentrant
{
	//Sleep(1);		
}
#endif
#endif


void Delay_2us(unsigned int fui_i)
{
	while(fui_i--);	
}