#include "includes.h"
#include "HC89F0541.h"
//unsigned char guc_AdcFlag  = 0;         //ADC转换完成标志位




void Task5(void *ppdata) reentrant
{
		ppdata = ppdata;
	while(1)
	{


		
OSTimeDly(50);
	
		
	}}













//void ADC_Rpt() interrupt ADC_VECTOR//电流转换
//{
//		guc_AdcFlag = 1;                    //ADC转换结束
//		IE1 &=~ 0x40;                       //关闭ADC中断
//		ADCC0 &=~ 0x20;						//清除ADC中断标志位ADCC0 &=~ 0x20;	

//    
//} 

