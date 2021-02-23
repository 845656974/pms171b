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
*			   在console下没有中断调用，应此也没有中断返回地址，vc编译后的保护模式下的段地址没有变的
*				应此也不用压栈，简单起见，我也没有把浮点寄存器压栈
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
void Delay_2us(unsigned int fui_i);		//延时函数
OS_STK *OSTaskStkInit (void (*task)(void *pd) reentrant, void *ppdata, OS_STK *ptos, INT16U opt) reentrant
{    
    OS_STK *stk;

    ppdata = ppdata;
    opt    = opt;                               //opt没被用到，保留此语句防止告警产生    
    stk    = ptos;                              //用户堆栈最低有效地址
    *stk++ = 15;                                //用户堆栈长度
    *stk++ = (INT16U)task & 0xFF;               //任务地址低8位
    *stk++ = (INT16U)task >> 8;                 //任务地址高8位    
    *stk++ = 0x0A;                              //ACC
    *stk++ = 0x0B;                              //B
    *stk++ = 0x00;                              //DPH
    *stk++ = 0x00;                              //DPL
    *stk++ = 0x00;                              //PSW
    *stk++ = 0x00;                              //R0
    
	//R3、R2、R1用于传递任务参数ppdata，其中R3代表存储器类型，R2为高字节偏移，R1为低字节位移。
	//通过分析KEIL汇编，了解到任务的void *ppdata参数恰好是用R3、R2、R1传递，不是通过虚拟堆栈。
    *stk++ = (INT16U)ppdata & 0xFF;             //R1
    *stk++ = (INT16U)ppdata >> 8;               //R2
    *stk++ = 0x01;                              //R3  因为我用的全是XDATA，所以存储器类型固定为1，见C51.PDF第178页说明。

    *stk++ = 0x04;                              //R4
    *stk++ = 0x05;                              //R5
    *stk++ = 0x06;                              //R6
    *stk++ = 0x07;                              //R7
                                                //不用保存SP，任务切换时根据用户堆栈长度计算得出。    
    *stk++ = (INT16U) (ptos+MaxStkSize) >> 8;   //?C_XBP 仿真堆栈指针高8位
    *stk++ = (INT16U) (ptos+MaxStkSize) & 0xFF; //?C_XBP 仿真堆栈指针低8位
        
    return ((void *)ptos);
}
/*
   使用C语言的中断处理函数有助与提高程序的移植性。建议中断程序不要太长，如果长则使用信号量来与任务同步，
   在外部任务中实现大量的处理。
   中断处理例程都放在下面。
*/

void UserTickTimer(void)
{
    TH0=0xB8;    		//普通51定时器方式1，必须在发生中断时，重新赋值并再次启动计时
    TL0=0;            	//Tick=50次/秒(即0.02秒/次)，晶振11.0592M
    TR0=1;
}

/* 
  uCOS-II系统时钟中断处理程序
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
  CLKSWR = 0x51;						//选择内部高频RC为系统时钟，内部高频RC 2分频，Fosc=16MHz
	CLKDIV = 0x01;						//Fosc 1分频得到Fcpu，Fcpu=16MHz
	
//  CLKSWR = 0x50;						//选择内部高频RC为系统时钟，内部高频RC 2分频，Fosc=32MHz
//	CLKDIV = 0x01;						//Fosc 1分频得到Fcpu，Fcpu=32MHz
	
	
		TMOD &= 0xF0;
		TMOD |= 0x01;   //定时器0：模式1(16位定时器)，仅受TR0控制；定时器1：波特率发生器
    TH0  = 0xB8;   //b8定义Tick=50次/秒(即0.02秒/次),TH,TL值与CPU的频率有关(11.0592M)!!!!!gai!!!!!!
    TL0  = 0x00;   //00  OS_CPU_C.C中定时器中断响应也要设置，OS_CFG.H中OS_TICKS_PER_SEC也有关系
    ET0  = 1;    //允许T0中断(在第一个任务开始执行时才开时钟中断,否则万一中断系统进入不可知状态)
    TR0  = 1;
		TCON1 |= 0x01;						//T0时钟1分频

	
	/*************************************PWM初始化****************************************/
    P2M3 = 0xC2;                        //P23设置为推挽输出
    P2M2 = 0xC2;                        //P22设置为推挽输出
    P1M1 = 0xC2;                        //P11设置为推挽输出
    P1M2 = 0xC2;                        //P12设置为推挽输出	
	
		PWM0_MAP = 0x23;					//PWM0通道映射P23口
		PWM01_MAP = 0x22;					//PWM01通道映射P22口
    PWM0C = 0x01;						//PWM0高有效，PWM01高有效，时钟8分频 
		
		
		
		PWM1_MAP = 0x11;					//PWM0通道映射P11口
		PWM11_MAP = 0x12;					//PWM01通道映射P12口
    PWM1C = 0x01;						//PWM1高有效，PWM11高有效，时钟8分频 
		
		
		
	/*************************************风机****************************************/
	P1M5 = 0xC2;                        //P15设置为推挽输出
//	P1_5 = 0;

		PWM2_MAP = 0x15;					//PWM2通道映射P15口
	  PWM2C = 0x01;						//PWM2高有效，PWM15高有效，时钟8分频 


    PWMM |= 0x77;						//PWM0和PWM1工作于互补模式					
	//独立模式下，PWM0和PWM01共用一个周期寄存器
	//PWM0的占空比调节使用			PWM0组的占空比寄存器
	//PWM01的占空比调节使用			PWM0组的死区寄存器

	//周期计算 	= 0x03ff / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
	//			= 0x03ff / (16000000 / 8)			
	// 			= 1023   /2000000
	//			= 511.5us		   		约1.955kHz
//  	OSSemCreate(PCF8563Sem, 1);
//	task2();

//	P2M3 = 0xC0;                        //P26设置为推挽输出
//	P2_3 = 0;
	
	
	
	/*************************************触摸按键****************************************/
		P1M3 = 0x54;                        //P05设置为非施密特数字带下拉输入
		P1M4 = 0x54;                        //P34设置为非施密特数字带下拉输入
	
	/*************************************碰撞****************************************/
		P1M0 = 0x50;                        //P10设置为非施密特数字带下拉输入

	/*************************************wifi****************************************/
		P0M7 = 0xC2;                        //P07设置为推挽输出,wifi
		P0_7 = 1;														//wifi使能
	/*************************************地检****************************************/
		P2_1 = 1;														//地检使能
		P2M1 = 0xC2;                        //P21设置为推挽输出
		
//		P1M3 = 0xC2;                        //P13设置为推挽输出
//		P0M4 = 0x68;                        //P05设置为带SMT上拉输入
//		PITS1 |= 0x11;						//INT4下降沿
//		PINTE0 |= 0x10;						//使能INT4
//		IE1 |= 0x80;							//打开INT2-7中断
		
//		/**********************************INT1配置初始化**************************************/
//		P0M4 = 0x69;			        	//P04设置为带SMT上拉输入
//		PITS1 |= 0x01;						//INT4下降沿
//		IE1 |= 0x80;							//打开INT4中断
		

//	
	/*************************************蜂鸣器****************************************/
		P3M2 = 0xC2;                        //P32设置为推挽输出
	
	/*************************************LED灯****************************************/
		P3M0 = 0xC2;                        //P32设置为推挽输出,绿LED
		P3M1 = 0xC2;                        //P32设置为推挽输出,红LED
		P3_0 = 1;
		P3_1 = 1;


	/*************************************边刷****************************************/
	P3M3 = 0xC2;                        //P15设置为推挽输出		
	
	
	
	/*****************5V供电使能**************/
	P0M6 = 0xC2;                        //P06设置为推挽输出
	P0_6 = 1;														//5V供电使能
	


/*****************充电**************/
//	P2M0 = 0xC2;
	P2M0 = 0x60;
//	P2_0 = 1;														//充电使能
	P3M4 = 0x60;                        //P14设置为非施密特数字带上拉输入


	/*********************ADC*************************/
	P0M2 = 0x00;                        //P05设置为模拟输入,右地检58
	P0M3 = 0x00;                        //P03设置为模拟输入,前地检
	P0M4 = 0x00;                        //P04设置为模拟输入,左地检
		
	P0M1 = 0x02;				        //P01设置为模拟输入,左轮ADC
	P0M0 = 0x02;				        //P00设置为模拟输入,右轮ADC
	P2M7 = 0x02;				        //P27设置为模拟输入,风机ADC
	P2M5 = 0x02;				        //P25设置为模拟输入,边刷ADC
	P2M6 = 0x02;                //P26设置为模拟输入,电源ADC

	ADCC0 = 0x81;						//打开ADC转换电源
	
	Delay_2us(20);						//延时20us，确保ADC系统稳定
	ADCC2 = 0x4B;						//转换结果12位数据，数据右对齐，ADC时钟16分频
//  SCANCON = 0x1A;                     //多通道扫描模式，扫描两个通道
  SCCH0 = 0x10;                     	//第一个扫描AN0，第二个扫描AN1
	
	
  SCCH1 = 0xDF;                     	//第三个扫描AN15，第四个扫描AN13
	
	SCCH2 = 0x4E;												//第五个扫描AN14，第六个扫描AN4
	SCCH3 = 0x23;												//第七个扫描AN3，第八个扫描是AN2
	
	SCANCON = 0x1F;                     //多通道扫描模式，扫描八个通道
		
		
		
  IE1 |= 0x40;                        //打开ADC中断
	ADCC0 &=~ 0x20;						//清除ADC中断标志位
  ADCC0 |= 0x40;
	
	
	
	
	
	
	
	
	
	/*****************定时1读取右轮编码器**************/

//	P3M5 = 0x62;                        //P35设置为带上拉输入
//	T1_MAP = 0x35;						//T0映射为P02
//	TCON1 |= 0x10;						//T1时钟1分频
//	TMOD |= 0x40;						//设置T1为计数器模式
//	TH1 = 0xFF;
//	TL1 = 0xFF;							//1脉冲
//	IE |= 0x08;							//打开T1中断
//	TCON |= 0x40;						//启动T1
	
	P3M5 = 0x69;			        	//P35设置为带SMT上拉输入
	PITS4 |= 0x04;						//INT17下降沿
	PINTE2 |= 0x02;						//使能INT17
	IE2 |= 0x01;							//打开INT17中断
		

	/****************中断读取左轮编码器*****************************/
	P2M4 = 0x6B;			        	//P24设置为施密特上拉输入
  P2INTE |= 0x10;						//使能P24下降沿中断
	IE2 |= 0x02;						//打开P2口中断
	EA = 1;								//打开总中断
	


	/***********************************WDT配置初始化(看门狗)**************************************/
	WDTC = 0x17;						//允许WDT复位，允许掉电/空闲模式下运行，1024分频

	//WDT使用的是RC44K时钟，WDT_CLOCK_1024是1024分频，初值为0xFF
	//定时时间 	= 1024 * 0xFF / 44000
	//			= 5.93s

	WDTCCR = 0xFF;						//写入00时，将关闭WDT功能（但不关闭内部低频RC），
										//即相当于禁止WDT。写入非0数据时，将启动WDT。

/************************************************************************************/ 


/**********************************UART配置初始化**************************************/
	P1M6 = 0xC2;				        //P16设置为推挽输出
	P1M7 = 0x62;				        //P17设置为上拉输出
	TXD_MAP = 0x16;						//TXD映射P16
	RXD_MAP = 0x17;						//RXD映射P17		
	T4CON = 0x06;						//T4工作模式：UART1波特率发生器
	//波特率计算
	//波特率 = 1/16 * (T4时钟源频率 / 定时器4预分频比) / (65536 - 0xFF30)
	//       = 1/16 * ((32000000 / 1) / 208)
	//		 = 9615.38(误差0.16%)

	//波特率9600
	//反推初值 = (65536 - ((T4时钟源频率 / 定时器4预分频比) * (1 / 16)) / 波特率)
	//		   = (65536 - (32000000 * (1 / 16) / 9600))
	//		   = (65536 - 208.33)
	//         = FF30
    
	TH4 = 0xFF;
	TL4 = 0x98;							//波特率9600,TL4 = 0x98;F7是115200	
	SCON2 = 0x02;						//8位UART，波特率可变
	SCON = 0x10;						//允许串行接收
	IE |= 0x10;							//使能串口中断
	EA = 1;								//使能总中断
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
	//printf("CPU利用率=%d\n",OSCPUUsage);
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
* Description: This function is called every tick.打印定时信息
				
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