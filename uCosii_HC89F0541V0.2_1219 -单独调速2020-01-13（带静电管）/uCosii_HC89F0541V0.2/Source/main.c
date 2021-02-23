#define	ALLOCATE_EXTERN
#include "includes.h"

OS_STK Task1Stk[MaxStkSize+1];
OS_STK Task2Stk[MaxStkSize+1];
OS_STK Task3Stk[MaxStkSize+1];
//OS_STK Task4Stk[MaxStkSize+1];
//OS_STK Task5Stk[MaxStkSize+1];

void Task1(void *ppdata) reentrant;
void Task2(void *ppdata) reentrant;
void Task3(void *ppdata) reentrant;
//void Task4(void *ppdata) reentrant;
//void Task5(void *ppdata) reentrant;

//os_cfg.h设置最大的优先级OS_LOWEST_PRIO
//OS_MAX_TASKS任务数
void main(void)
{
	OSInit();//OS系统初始化
	InitHardware();//时钟&&管脚初始化

	OSTaskCreate(Task1,(void*)0,&Task1Stk[0],2);//wifi通信
	OSTaskCreate(Task2,(void*)0,&Task2Stk[0],1);//ADC
	OSTaskCreate(Task3,(void*)0,&Task3Stk[0],3);//轮子pwm
//	OSTaskCreate(Task4,(void*)0,&Task4Stk[0],3);//按键&项目
//	OSTaskCreate(Task5,(void*)0,&Task5Stk[0],1);//检测
	
	
	
	
	OSStart();
}




