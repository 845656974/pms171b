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

//os_cfg.h�����������ȼ�OS_LOWEST_PRIO
//OS_MAX_TASKS������
void main(void)
{
	OSInit();//OSϵͳ��ʼ��
	InitHardware();//ʱ��&&�ܽų�ʼ��

	OSTaskCreate(Task1,(void*)0,&Task1Stk[0],2);//wifiͨ��
	OSTaskCreate(Task2,(void*)0,&Task2Stk[0],1);//ADC
	OSTaskCreate(Task3,(void*)0,&Task3Stk[0],3);//����pwm
//	OSTaskCreate(Task4,(void*)0,&Task4Stk[0],3);//����&��Ŀ
//	OSTaskCreate(Task5,(void*)0,&Task5Stk[0],1);//���
	
	
	
	
	OSStart();
}




