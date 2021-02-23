#include "includes.h"
#include "Source\task3.h"

INT8U run_stop_flag =0;//90°直角停止标志，在中断内触发
INT8U run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
INT8U run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启

INT8U  num_L=0;				//左轮码盘值
INT8U num_L1 = 0;			//左轮码盘值
int	num_L2=0;					//左轮码盘值
int sum_L=0;						//累计偏差
int count_L_1=0;				//编码器本次偏差
int count_L_2=0;				//编码器上次偏差
int count_sum_L = 0;		//系统偏差，用于pid的d
int count_sum1_L = 0;		//系统偏差，当前偏差
int count_sum2_L = 0;		//系统偏差，上一次偏差
int count_sum3_L = 0;		//系统偏差，上上次偏差
int	pwm_L=0;			//PWM值0-2048

INT8U  num_R=0;				//右轮码盘值
INT8U  num_R1=0;				//右轮码盘值
int	num_R2=0;				//右轮码盘值
int sum_R=0;						//累计偏差
int count_R_1=0;				//编码器本次偏差
int count_R_2=0;				//编码器上次偏差
int count_sum_R = 0;		//系统偏差，用于pid的d
int count_sum1_R = 0;		//系统偏差，当前偏差
int count_sum2_R = 0;		//系统偏差，上一次偏差
int count_sum3_R = 0;		//系统偏差，上上次偏差
int	pwm_R=0;			//PWM值0-2048



int f3_crash1= 0;//后退次数
int 	f3_count = 0;//步数计数器
bool	f3_flag0 = 1;//按键启动标志，一般用于按键按下执行第一次操作
INT8U 	f3_flag1 = 0;//记录标志，一般用于第一次复位
INT8U f3_flag3 = 0;//旋转完毕计数器,6次跳出绕物，进行120度旋转
int		f3_flag4 = 0;//重点清扫模式计数器
INT8U f3_flag5 = 0;//120°次数

bool fan_open = 0;//风机开启1,关闭0
INT32U f3_fengji = 0;

int f3_flag = -1;//步骤运行，共100多步骤

//f2
extern INT8U f2_mode ;//模式0停止 1弓形 2回充 3充电 4快闪 5慢闪 6异常 7方向按键 10定点 11待机
extern INT8U TK_ZJ;//自检模式

extern INT8U	BUZZER_flag ;//蜂鸣器
extern INT8U	BUZZER_num ;//蜂鸣器计数器，用于响应时间
extern bool sensor_error ;//传感器状态，地检墙检碰撞



void Task3(void *ppdata) reentrant
{
	initialize_pwm();//初始化PWM

	ppdata = ppdata;
		OSTimeDly(1000);
	while(1){
/***********************弓形、定点、快慢闪初始化*******************************/		
f3_run_first();//	弓形、定点、快慢闪初始化
/***********************停止运行*******************************/	
f3_stop();			//	停止运行		
	
/***********************弓形*******************************/			
f3_run();		
		
/***********************自检轮子测试*******************************/					
f3_run_ZJ();//	自检轮子测试			
/***********************WIFI方向按键*******************************/					
f3_run_WIFI();//	自检轮子测试	
	
	
		OSTimeDly(15);
		
		
		
	}  }


	
void f3_run(void)			//	正常
{
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = 0xFF;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位
//			
//			SBUF = 0xFE;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位

//			SBUF = 0x01;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位
	
//			SBUF = num_L1;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位

	
if(f3_flag==0)//步骤0 直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(10);
	}		
	f3_count++;//步数计数器
	if(sensor_error==0)//碰撞和地检都正常
	{
			advance_L(1,20,0);
			advance_R(1,20,0);
	}
	else//碰撞
	{
		f3_flag1 = 0;//记录标志，一般用于步骤第一次复位
		f3_flag = 1;//进入步骤1
	}
	if(f3_count >600)
	{
	f3_flag1 =0;//计数器
	f3_flag =2;//跳过步骤1后退，直接君如旋转
	}
}	

if(f3_flag==1)//步骤1 碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(50);
	}

	back(12,20);	//后退20次，进入步骤12	
}
if(f3_flag==2)//步骤2，右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =1;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =3;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==3)//步骤3，右转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,15,0);
		advance_R(1,15,0);
	}
	else//碰撞
	{
		f3_flag = 40;//进入步骤40
		f3_flag3 =0;//旋转完毕计数器,6次跳出绕物，进行120度旋转
		f3_flag1 =0;//计数器
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=4;
	}	
}
if(f3_flag==4)//步骤4，右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =1;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =5;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==5)//步骤5，右转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;
	if(sensor_error==0)
	{
		advance_R(1,20,0);
		advance_L(1,20,0);
	}
	else
	{
		f3_flag = 6;
		f3_flag1 = 0;
	}	
	if(f3_count >600)
	{
	f3_flag1 =0;//计数器
	f3_flag =7;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==6)//步骤6，碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(50);
	}
	
	back(17,20);	//后退10次，进入步骤17	
}
if(f3_flag==7)//步骤7，碰撞后退进行左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =1;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
//	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);	
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =8;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==8)//步骤8，左转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_R(1,15,0);
		advance_L(1,15,0);
	}
	else//碰撞
	{
		f3_flag = 20;//进入步骤40
		f3_flag3 =0;//旋转完毕计数器
		f3_flag1 =0;//计数器
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=9;
	}	
}
if(f3_flag==9)//步骤9，左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =1;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =0;//跳过步骤1后退，直接君如旋转
	}
}

if(f3_flag==12)//步骤2，右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =2;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =3;//跳过步骤1后退，直接君如旋转
	}	
}

if(f3_flag==17)//步骤2，左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =2;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(0,15,0);
		advance_R(1,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =8;//跳过步骤1后退，直接君如旋转
	}
	
}	









if(f3_flag==20)//步骤20，绕物，碰撞后后退
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count=15;
		OSTimeDly(50);
	}
	f3_count--;//后退计数器
	advance_L(0,15,0);
	advance_R(0,15,0);
	if(sensor_error)//碰撞
	{
		f3_count=15;	
	}
	if(f3_count==0)//后退完毕，执行21
	{
		f3_flag1=0;
		f3_flag=21;
	}	
}	
if(f3_flag==21)//步骤21，右边绕物
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count=0;
		OSTimeDly(50);
	}
	f3_count++;		//旋转切换计数器,切换左旋转到右或者右到左
	if(f3_count<50)
	{
		advance_L(1,2,1);	
		advance_R(1,20,1);
	}
	else
	{
		f3_flag =22;
		f3_flag1= 0;//不在进入重置	
	}
	if(sensor_error)//碰撞
	{
		f3_flag = 22;//进入步骤40
		f3_flag1 =0;//计数器
	}
}	
if(f3_flag==22)//步骤42，右边绕物
{
	if(f3_flag1==0)//重置PWM
	{
		f3_flag3++;//旋转完毕计数器,6次跳出绕物，进行120度旋转
		f3_count=0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;
	if(f3_count<500)
	{
		advance_L(1,20,1);	
		advance_R(1,4,1);
	}
	else
	{
		f3_flag =23;
		f3_flag1= 0;//不在进入重置
	}
	if(sensor_error)//碰撞
	{
		f3_flag = 20;//进入步骤20
		f3_flag1 =0;//计数器
	}
	if(f3_flag3>=6)//绕完物
	{
		f3_flag3 = 0;
		f3_flag  =24;//步骤24
		f3_flag1 = 0;
		f3_flag5 = 0;//120°次数
	}
}

if(f3_flag==23)//步骤43，直行
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);	
	}
	else
	{
		f3_flag = 20;//进入步骤40，后退
		f3_flag1 =0;//计数器
	}
	if(f3_count>200)
	{
		f3_flag = 0;//回到50
		f3_flag1 = 0;//计数器
	}	
}
if(f3_flag==24)//绕柱玩准备走,120°
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if((sensor_error))
	{
		f3_flag = 26;
		f3_flag1=0;
	}
	else
	{
		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	if(f3_count>66)
	{
		f3_flag = 25;
		f3_flag1 = 0;//计数器
	}			
}
if(f3_flag==25)///120°直行
{
	if(f3_flag1==0)//重置PWM
	{
		f3_flag5++;//120°次数
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag = 26;
		f3_flag1=0;	
	}
	if(f3_count>520)
	{
		f3_flag = 24;
		f3_flag1=0;
	}
	if(f3_flag5==3)			
	{
		f3_flag5 = 0;//120°次数
		f3_flag = 0;
		f3_flag1=0;
	
	}
}
if(f3_flag==26)//步骤46，碰撞后退
	{
		if(f3_flag1==0)//重置PWM
		{
			reset_pwm();//重置PWM
			f3_flag1= 1;//不在进入重置
			f3_count = 0;
			OSTimeDly(50);
		}
		back(24,20);//后退20，进入44
	}









if(f3_flag==40)//步骤40，绕物，碰撞后后退
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count=15;
		OSTimeDly(50);
	}
	f3_count--;//后退计数器
	advance_L(0,15,0);
	advance_R(0,15,0);
	if(sensor_error)//碰撞
	{
		f3_count=15;	
	}
	if(f3_count==0)//后退完毕，执行41
	{
		f3_flag1=0;
		f3_flag=41;
	}	
}
if(f3_flag==41)//步骤41，左边绕物，后退完执行前进
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count=0;
		OSTimeDly(50);
	}
	f3_count++;		//旋转切换计数器,切换左旋转到右或者右到左
	if(f3_count<50)
	{
		advance_L(1,20,1);
		advance_R(1,2,1);	
	}
	else
	{
		f3_flag =42;
		f3_flag1= 0;//不在进入重置
	}	
	if(sensor_error)//碰撞
	{
		f3_flag = 40;//进入步骤40
		f3_flag1 =0;//计数器
	}
}



if(f3_flag==42)//步骤42，左边绕物
{
	if(f3_flag1==0)//重置PWM
	{
		f3_flag3++;//旋转完毕计数器,6次跳出绕物，进行120度旋转
		f3_count=0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;
	if(f3_count<500)
	{
		advance_L(1,4,1);
		advance_R(1,20,1);	
	}
	else
	{
		f3_flag =43;
		f3_flag1= 0;//不在进入重置
	}	
	if(sensor_error)//碰撞
	{
		f3_flag = 40;//进入步骤40
		f3_flag1 =0;//计数器
	}
		if(f3_flag3>=6)//绕完物
	{
		f3_flag3 = 0;
		f3_flag  =44;//步骤44
		f3_flag1 = 0;
		f3_flag5 = 0;//120°次数
	}

}	

if(f3_flag==43)//步骤43，直行
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);	
	}
	else
	{
		f3_flag = 40;//进入步骤40，后退
		f3_flag1 =0;//计数器
	}
	if(f3_count>200)
	{
		f3_flag = 50;//回到50
		f3_flag1 = 0;//计数器
	}	
}


if(f3_flag==44)//绕柱玩准备走,120°
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
		f3_count++;//步数计数器
	if(sensor_error)
	{
		f3_flag = 46;
		f3_flag1=0;
	}
	else
	{
		advance_L(1,20,0);
		advance_R(0,20,0);
	}
	if(f3_count>66)
	{
		f3_flag = 45;
		f3_flag1 = 0;//计数器
	}			
}

if(f3_flag==45)///120°执行完毕，直行
{
	if(f3_flag1==0)//重置PWM
	{
		f3_flag5++;//120°次数
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag = 46;
		f3_flag1=0;	
	}
	if(f3_count>520)
	{
		f3_flag = 44;
		f3_flag1=0;
	}
	if(f3_flag5==3)			
	{
		f3_flag5 = 0;//120°次数
		f3_flag = 50;
		f3_flag1=0;
	
	}
}
if(f3_flag==46)//步骤46，碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		f3_count = 0;
		OSTimeDly(50);
	}
	back(44,20);//后退20，进入44
}


	
	
	
if(f3_flag==50)//步骤50 直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(50);
	}		
	f3_count++;//步数计数器
	if(sensor_error==0)
	{

			advance_R(1,20,0);
			advance_L(1,20,0);
	}
	else//碰撞
	{
		f3_flag1 = 0;//记录标志，一般用于步骤第一次复位
		f3_flag = 51;//进入步骤51
	}
	if(f3_count >600)
	{
	f3_flag1 =0;//计数器
	f3_flag =52;//跳过步骤1后退，直接君如旋转
	}
}
	
	
if(f3_flag==51)//步骤51 碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(50);
	}
	
	back(62,20);	//后退20次，进入步骤2	
}
	

if(f3_flag==52)//步骤52，左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =1;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =53;//跳过步骤1后退，直接君如旋转
	}
	
}
if(f3_flag==53)//步骤3，左转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_R(1,15,0);
		advance_L(1,15,0);
	}
	else//碰撞
	{
		f3_flag = 20;//进入步骤20
		f3_flag3 =0;//旋转完毕计数器
		f3_flag1 =0;//计数器
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=54;
	}	
}
if(f3_flag==54)//步骤54，左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =1;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(0,15,0);
		advance_R(1,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =55;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==55)//步骤5，左转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag = 56;
		f3_flag1 = 0;
	}	
	if(f3_count >600)
	{
	f3_flag1 =0;//计数器
	f3_flag =67;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==56)//步骤6，碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;//步数计数器，用于走的长度
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
		OSTimeDly(50);
	}
	
	back(67,20);	//后退10次，进入步骤57
}
if(f3_flag==57)//步骤7，碰撞后退进行右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =1;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =58;//跳过步骤1后退，直接君如旋转
	}
}
if(f3_flag==58)//步骤8，右转后直行
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(1,15,0);
		advance_R(1,15,0);
	}
	else//碰撞
	{
		f3_flag = 40;//进入步骤40
		f3_flag3 =0;//旋转完毕计数器
		f3_flag1 =0;//计数器
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=59;
	}	
}
if(f3_flag==59)//步骤9，右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =1;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =50;//跳过步骤1后退，直接君如旋转
	}
}
		
	
	

if(f3_flag==62)//步骤2，左转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =2;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =53;//跳过步骤1后退，直接君如旋转
	}
}
	
if(f3_flag==67)//步骤2，右转
{
	if(f3_flag1==0)//重置PWM
	{
		run_stop_flag = 0;//90°直角停止标志，在中断内触发
		run_stop_flag_R =2;//90°直角开始标志，在开始直角中触发开启
		run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
		OSTimeDly(50);
	}
	f3_count ++;//步数计数器
	if(sensor_error==0&&run_stop_flag ==0)//碰撞和地检都正常,且中断内的90°计数器没有到1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//碰撞和地检异常||90°完成||计数器溢出75
	{
	f3_flag1 =0;//计数器
	f3_flag =58;//跳过步骤1后退，直接君如旋转
	}
	
}	



/****************************************************************	
*
*重点清扫
*
*
*****************************************************************/			
if(f3_flag==70)//步骤2，右转
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count ++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag1 =0;//计数器
		f3_flag =71;//跳过步骤1后退，直接进入旋转	
	
	}
	if(f3_count >100)
	{
	f3_flag1 =0;//计数器
	f3_flag =72;//跳过步骤1后退，直接进入旋转
	}

}

if(f3_flag==71)//半圈旋转碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 20;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count --;//步数计数器
	if(sensor_error)//处于碰撞
	{
		f3_count=10;
	}
	if(f3_count >0)//后退
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	if(f3_count==0)//碰撞解除和后退指令执行完毕，进入步骤70
	{
	f3_flag1 =0;//计数器
	f3_flag =70;//跳过步骤1后退，直接进入旋转	
	}	
}
if(f3_flag==72)//左旋转重点扫地
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag4=0;//旋转模式计数器
		f3_flag1= 1;//不在进入重置
	}
	
	if(sensor_error)//碰撞
	{
		f3_flag =73;	
	}
	else
	{
		if(f3_flag4>=0&&f3_flag4<1000)//[0,5)
		{
//			if(f3_flag4==1)
//			{
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
//				}
			advance_L(1,f3_flag4/200,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=1000&&f3_flag4<2000)//[5,8]
		{
//			if(f3_flag4==1001)
//			{
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
//				}
			advance_L(1,2+f3_flag4/300,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=2000&&f3_flag4<3500)//[8,11]
		{
//			if(f3_flag4==2001)
//			{
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
//				}
			advance_L(1,3+f3_flag4/400,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=3500&&f3_flag4<7500)//[11,14]
		{
//			if(f3_flag4==3501)
//			{
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
//				}
			advance_L(1,9+f3_flag4/1500,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=7500&&f3_flag4<10000)//[14,16]
		{
//			if(f3_flag4==7501)
//			{
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
//				}
			advance_L(1,11+f3_flag4/2000,1);
			advance_R(1,20,1);		
		}
		
		if(f3_flag4>=10000)
		{
			
//				BUZZER_flag=1;//开启蜂鸣器
//				BUZZER_num=30;//蜂鸣器鸣叫时间
				
			f3_flag1 = 0;
			f3_flag = 74;
		}
	}
	f3_flag4++;
}
if(f3_flag==73)//步骤102，旋转碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 10;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count--;
	if(sensor_error)
		f3_count = 10;
	if(f3_count>0)
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	else
	{
		f3_flag=74;
		f3_flag1=0;
	}	
}
if(f3_flag==74)//左旋转左旋转180°第二次
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count ++;//步数计数器
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag1 =0;//计数器
		f3_flag =75;//跳过步骤1后退，直接进入旋转	
	
	}
	if(f3_count >100)
	{
	f3_flag1 =0;//计数器
	f3_flag =76;//跳过步骤1后退，直接进入旋转
	}
}
if(f3_flag==75)//第二次半圈旋转碰撞后退
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 10;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	f3_count --;//步数计数器
	if(sensor_error)//处于碰撞
	{
		f3_count=10;
	}
	if(f3_count >0)//后退
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	if(f3_count==0)//碰撞解除和后退指令执行完毕，进入步骤70
	{
	f3_flag1 =0;//计数器
	f3_flag =76;//跳过步骤1后退，直接进入旋转	
	}	
}
if(f3_flag==76)//左旋转重点扫地
{
	if(f3_flag1==0)//重置PWM
	{
		f3_count = 0;
		reset_pwm();//重置PWM
		f3_flag1= 1;//不在进入重置
	}
	
	if(sensor_error)//碰撞
	{
		f3_flag=-1;
		f3_flag1=0;
		reset_pwm();//重置PWM
		f2_mode=1;
		f3_flag0 =1;
//			BUZZER_num=30;//蜂鸣器鸣叫时间
//			BUZZER_flag=1;//开启蜂鸣器
	}
	else
	{
		if(f3_flag4>=0&&f3_flag4<1000)//[0,5)
		{
			advance_R(1,f3_flag4/200,1);
			advance_L(1,20,1);		
		}
		if(f3_flag4>=1000&&f3_flag4<2000)//[5,8]
		{
			advance_R(1,2+f3_flag4/300,1);
			advance_L(1,20,1);		
		}
		if(f3_flag4>=2000&&f3_flag4<3500)//[8,11]
		{
			advance_R(1,3+f3_flag4/400,1);
			advance_L(1,20,1);		
		}
		if(f3_flag4>=3500&&f3_flag4<7500)//[11,14]
		{
			advance_R(1,9+f3_flag4/1500,1);
			advance_L(1,20,1);		
		}
		if(f3_flag4>=7500&&f3_flag4<=10000)//[14,16]
		{
			advance_R(1,11+f3_flag4/2000,1);
			advance_L(1,20,1);		
		}
		if(f3_flag4<=0)
		{
			f3_flag=-1;
			f3_flag1=0;
			reset_pwm();//重置PWM
				f2_mode=1;
			f3_flag0 =1;

		}
		f3_flag4--;		
	}		
}











}	




void back(int back_flag,int back_flag2)//后退
{
	if(f3_flag1==1)//重置PWM
	{
		f3_crash1 = back_flag2;
		reset_pwm();//重置PWM
		f3_flag1= 2;//不在进入重置

	}
	if(sensor_error )	//后退计数器		//碰撞或者地检出问题
	{

		f3_crash1=back_flag2;	//后退次数
	}
	if(f3_crash1>0)
	{

			advance_L(0,20,0);
			advance_R(0,20,0);
			f3_crash1--;
	}
	if(f3_crash1==0)//碰撞解除和后退指令执行完毕，进入步骤2
	{
		f3_flag= back_flag;
		f3_flag1= 0;//不在进入重置
	}	

}








void f3_stop(void)			//	停止运行	
{
	if(f3_flag==-1)//步骤-1 暂停
	{
		if(f3_flag1==0)//重置PWM
		{
			if(TK_ZJ!=1)
			{
				Brush = 0;//边刷关闭
				fan = 0;//风机关闭
				fan_open = 0;//风机开启1,关闭0
			}
			f3_fengji=0;//弓形风机启动
			f3_count =0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
	}
}	
	


void f3_run_WIFI(void)	//	WIFI方向按键	
{
//wifi方向按键
if(f3_flag==220)//步骤220 前进
{
	if(f3_flag1==0)//重置PWM
	{
		Brush=1;//边刷
//		fan=1;//风机
		fan_open = 1;//风机开启1,关闭0
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
	}		

	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);
	}
	else//碰撞
	{
		Brush=0;//边刷
		fan=0;//风机
		fan_open = 0;//风机开启1,关闭0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
if(f3_flag==221)//步骤221 后退
{
	if(f3_flag1==0)//重置PWM
	{
		Brush=1;//边刷
//		fan=1;//风机
		fan_open = 1;//风机开启1,关闭0
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
	}		
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	else//碰撞
	{
		Brush=0;//边刷
		fan=0;//风机
		fan_open = 0;//风机开启1,关闭0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
if(f3_flag==222)//步骤222 左
{
	if(f3_flag1==0)//重置PWM
	{
		Brush=1;//边刷
//		fan=1;//风机
		fan_open = 1;//风机开启1,关闭0
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
	}		
	if(sensor_error==0)
	{

		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else//碰撞
	{
		Brush=0;//边刷
		fan=0;//风机
		fan_open = 0;//风机开启1,关闭0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}	
if(f3_flag==223)//步骤222 右
{
	if(f3_flag1==0)//重置PWM
	{
		Brush=1;//边刷
//		fan=1;//风机
		fan_open = 1;//风机开启1,关闭0
		reset_pwm();
		f3_flag1 = 1;//不在进入重置
	}		
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(0,20,0);
	}
	else//碰撞
	{
		Brush=0;//边刷
		fan=0;//风机
		fan_open = 0;//风机开启1,关闭0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
	
	
}







void f3_run_ZJ(void)//	自检轮子测试	
{

//自检测试
	if(f3_flag==200)//步骤200 左轮前进
	{		
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_L(1,25,1);
	}	
	if(f3_flag==201)//步骤200 右轮前进
	{
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_R(1,25,1);
	}
	if(f3_flag==202)//步骤200 左轮后退
	{
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_L(0,25,1);
	}		
	if(f3_flag==203)//步骤200 右轮后退
	{
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_R(0,25,1);
	}		
	if(f3_flag==204)//步骤200 2轮前进
	{
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_L(1,25,1);
			advance_R(1,25,1);
	}	
	if(f3_flag==205)//步骤200 2轮后退
	{
		if(f3_flag1==0)//重置PWM
		{
			f3_count = 0;//步数计数器，用于走的长度
			reset_pwm();
			f3_flag1 = 1;//不在进入重置
		}		
			advance_L(0,25,1);
			advance_R(0,25,1);
	}		

}	
	
void f3_run_first(void)//	弓形、定点、快慢闪初始化
{
if((f2_mode==0||f2_mode==4||f2_mode==5)&&f3_flag0&&TK_ZJ==0)//停止.4为快闪5为慢闪
{
	f3_fengji=0;//弓形风机启动
	Brush = 0;//边刷关闭
	fan = 0;//风机关闭
	fan_open = 0;//风机开启1,关闭0
	f3_flag0 =0;//按键启动标志，一般用于按键按下执行第一次操作
	f3_flag=-1 ;//步骤运行，共100多步骤
	f3_flag1 = 0;//记录标志，一般用于步骤第一次复位
}	

if(f2_mode==1&&f3_flag0)//弓形
{
	
	f3_fengji=0;//弓形风机启动
	Brush = 1;//边刷启动
//	fan = 1;//风机关闭
	fan_open = 1;//风机开启1,关闭0
	f3_flag0 =0;//按键启动标志，一般用于按键按下执行第一次操作
	f3_flag=0 ;//步骤运行，共100多步骤
	f3_flag1 = 0;//记录标志，一般用于步骤第一次复位
}


if(f2_mode==10&&f3_flag0)//定点
{
	f3_fengji=0;//弓形风机启动
	Brush = 1;//边刷启动
//	fan = 1;//风机关闭
	fan_open = 1;//风机开启1,关闭0
	f3_flag0 =0;//按键启动标志，一般用于按键按下执行第一次操作
	f3_flag=70 ;//步骤运行，共100多步骤
	f3_flag1 = 0;//记录标志，一般用于步骤第一次复位
}
}
	
	


	
	
	
	
	
	
	
	
	
	
	
	
/*****************PWM初始化*********************************************/
static void initialize_pwm()
{
	PWM0PH = 0x07;						//周期高4位设置为0x07
	PWM0PL = 0xFF;						//周期低8位设置为0xFF
	PWM1PH = 0x07;						//周期高4位设置为0x07
	PWM1PL = 0xFF;						//周期低8位设置为0xFF	
	PWM2PH = 0x07;						//周期高4位设置为0x07
	PWM2PL = 0xFF;						//周期低8位设置为0xFF	
	//占空比计算= 0x0155 / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
	//			= 0x0155 / (16000000 / 8)			
	// 			= 341 	 / 2000000
	//			= 170.5us		   占空比为 170.5/511.5 = 33.3%
/*****************右轮*********************************************/
	PWM0DH = 0;						//00PWM0高4位占空比0x01			P2.2
	PWM0DL = 0;						//00PWM0低8位占空比0x55			P2.3
	PWM0DTH = 0;						//03PWM01高4位占空比0x01 		P2.2
	PWM0DTL = 00;						//55PWM01低8位占空比0x55		P2.3
/*****************左轮*********************************************/
	PWM1DH = 0;						//00PWM0高4位占空比0x01			P
	PWM1DL = 0;						//00PWM0低8位占空比0x55
	PWM1DTH = 0;						//01PWM01高4位占空比0x01
	PWM1DTL = 00;						//55PWM01低8位占空比0x55
	
	
	
	PWM2DH = 0;						//00PWM0高4位占空比0x01			P
	PWM2DL = 0;						//00PWM0低8位占空比0x55
	PWM2DTH = 0;						//01PWM01高4位占空比0x01
	PWM2DTL = 00;						//55PWM01低8位占空比0x55
	
	
	
//	PWM2PH = 0x03;						//周期高4位设置为0x03
//	PWM2PL = 0xFF;						//周期低8位设置为0xFF
//	PWM2DH = 2;						//00PWM0高4位占空比0x01			P
//	PWM2DL = 0;						//00PWM0低8位占空比0x55
//	PWM2DTH = 2;						//01PWM01高4位占空比0x01
//	PWM2DTL = 00;						//55PWM01低8位占空比0x55

	PWMEN |= 0x77;						//使能PWM0以及PWM01   ，使能PWM1以及PWM01
}	
	
	
	
void reset_pwm(void)//PWM重置
{
	/*****************右轮*********************************************/
	PWM0DH = 0;						//00PWM0高4位占空比0x01			P2.2
	PWM0DL = 0;						//00PWM0低8位占空比0x55			P2.3
	PWM0DTH = 0;						//03PWM01高4位占空比0x01 		P2.2
	PWM0DTL = 00;						//55PWM01低8位占空比0x55		P2.3
/*****************左轮*********************************************/
	PWM1DH = 0;						//00PWM0高4位占空比0x01			P
	PWM1DL = 0;						//00PWM0低8位占空比0x55
	PWM1DTH = 0;						//01PWM01高4位占空比0x01
	PWM1DTL = 00;						//55PWM01低8位占空比0x55
	
	
	num_L = 0;//编码器
	num_R = 0;//编码器
	num_L2 = 0;//编码器
	num_R2 = 0;//编码器
}	
	
	
	
	
	
	
	
	
	
	
//	
///*****************************************
//go 1前进 0后退
//v		速度

//********************************************/
	
void advance_L(int go,int v,int mode)//左边
{
//	int count_LR = 0;
	if (PWM1DTH==0&&PWM1DTL==0&&go==1)//前进初始化
		{	
//		PWM1DTH=2;
//		PWM1DTL=2;
		sum_L = 0;					//累积偏差
		count_L_1 = 0;			//编码器本次偏差
		count_L_2 = 0;			//编码器上次偏差
		num_L=0;						//左轮编码器
		
		count_sum_L = 0;		//系统偏差，用于pid的d
		count_sum1_L = 0;		//系统偏差，当前偏差
		count_sum2_L = 0;		//系统偏差，上一次偏差
		count_sum3_L = 0;		//系统偏差，上上次偏差
		pwm_L=600;	
			
	}
	if(PWM1DH==0&&PWM1DL==0&&go==0)//后退初始化
	{	
//		PWM1DL = 2;	
//		PWM1DH = 2;
		sum_L = 0;					//累积偏差
		count_L_1 = 0;			//编码器本次偏差
		count_L_2 = 0;			//编码器本次偏差
		num_L=0;						//左轮编码器
		
		count_sum_L = 0;		//系统偏差，用于pid的d
		count_sum1_L = 0;		//系统偏差，当前偏差
		count_sum2_L = 0;		//系统偏差，上一次偏差
		count_sum3_L = 0;		//系统偏差，上上次偏差
		pwm_L=600;

	}
	count_L_2 = count_L_1;			//上次偏差
	count_L_1 = v -	num_L;			//本次偏差,与编码器做差
	count_sum3_L = count_sum2_L;//上上一次系统偏差
	count_sum2_L = count_sum1_L;//上一次系统偏差
	count_sum1_L = count_L_1;		//当前系统偏差
	sum_L += count_L_1;					//总的累积偏差
	count_sum_L = count_sum1_L-2*count_sum2_L+count_sum3_L;//系统偏差，用于pid的d

		/**************************************************************************
		函数功能：速度闭环控制PID控制器
		P 用于提高响应速度、I 用于减小静差、D 用于抑制震荡。
		返回  值：电机PWM
		根据位置式离散PID公式 
		pwm +=Kp*[e(k)-e(k-1)] + Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
		e(k)代表本次偏差 
		e(k-1)代表上一次的偏差  
		∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
		pwm代表输出
		**************************************************************************/
//	count_LR = num_R2-num_L2;
//	if(count_LR>100)
//		count_LR=100;
//	else if(count_LR<-100)
//		count_LR=-100;
	

		if(mode==0)//正常模式 8 3 3
			pwm_L+=15*count_L_1+8*(count_L_1-count_L_2)+2*count_sum_L;
//			pwm_L+=10*(count_L_1-count_L_2)+5*count_L_1+4*count_sum_L;
		else//mode==1非同步模式 4 2 2
			pwm_L+=5*count_L_1+10*(count_L_1-count_L_2)+5*count_sum_L;
		if(pwm_L>2000)//左轮限制最大速度
		{
			pwm_L=2000;
		}
		if(pwm_L<0)//左轮限制最小速度
		{
			pwm_L=0;	
		}
		if(go)//正方向
		{
			if(v==0)
			{
				PWM1DTH = 0;//左轮高位PWM
				PWM1DTL = 0;//左轮低位PWM
			
			}
			else
			{
				PWM1DTH = pwm_L/255;//左轮高位PWM
				PWM1DTL = pwm_L%255;//左轮低位PWM
			}
		}
		else//反方向
			{
			if(v==0)
			{
				PWM1DH = 0;//左轮高位PWM
				PWM1DL = 0;//左轮低位PWM
			
			}
			else
			{
				PWM1DH = pwm_L/255;//左轮高位PWM
				PWM1DL = pwm_L%255;//左轮低位PWM
			}
		}
		num_L1=num_L;
		num_L = 0;

}




void advance_R(int go,int v,int mode)//右边
{
//	int count_RL = 0;
	if (PWM0DH==0&&PWM0DL==0&&go==1)//前进初始化
		{	
//		PWM0DH=2;
//		PWM0DL=2;
		sum_R = 0;					//累积偏差
		count_R_1 = 0;			//编码器本次偏差
		count_R_2 = 0;			//编码器上次偏差
		num_R=0;						//左轮编码器
		
		count_sum_R = 0;		//系统偏差，用于pid的d
		count_sum1_R = 0;		//系统偏差，当前偏差
		count_sum2_R = 0;		//系统偏差，上一次偏差
		count_sum3_R = 0;		//系统偏差，上上次偏差
		pwm_R=600;

	}
	if(PWM0DTH==0&&PWM0DTL==0&&go==0)//后退初始化
	{	
//		PWM0DTH = 2;	
//		PWM0DTL = 2;
		sum_R = 0;					//累积偏差
		count_R_1 = 0;			//编码器本次偏差
		count_R_2 = 0;			//编码器本次偏差
		num_R=0;						//左轮编码器
		
		count_sum_R = 0;		//系统偏差，用于pid的d
		count_sum1_R = 0;		//系统偏差，当前偏差
		count_sum2_R = 0;		//系统偏差，上一次偏差
		count_sum3_R = 0;		//系统偏差，上上次偏差
		pwm_R=600;
	}
	count_R_2 = count_R_1;			//上次偏差
	count_R_1 = v -	num_R;			//本次偏差,与编码器做差
	count_sum3_R = count_sum2_R;//上上一次系统偏差
	count_sum2_R = count_sum1_R;//上一次系统偏差
	count_sum1_R = count_R_1;		//当前系统偏差
	sum_R += count_R_1;					//总的累积偏差
	count_sum_R = count_sum1_R-2*count_sum2_R+count_sum3_R;//系统偏差，用于pid的d
		/**************************************************************************
		函数功能：速度闭环控制PID控制器
		P 用于提高响应速度、I 用于减小静差、D 用于抑制震荡。
		返回  值：电机PWM
		根据位置式离散PID公式 
		pwm +=Kp*[e(k)-e(k-1)] + Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
		e(k)代表本次偏差 
		e(k-1)代表上一次的偏差  
		∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
		pwm代表输出
		**************************************************************************/
//		count_RL = num_L2-num_R2;
//		if(count_RL>100)
//		count_RL=100;
//		else if(count_RL<(-1000))
//		count_RL=-100;
		
//						IE &=~ 0x10;				//允许UART1中断
//				SBUF = count_RL/10;//task1发送8位串口数据
//				while(!(SCON & 0x02));
//				SCON &=~ 0x02;			//清除发送中断标志位
		
		
//		val_right=40*(float)task1_count0+20*task1+30*(float)(task1_count0-task1_count01); 
//		val_right+=+1*(task1_count22-task1_count11)+14*(task1_count0-task1_count01) + 7*task1_count0 +10*task1_sum0; 
		if(mode==0)//同步模式 8 3 3
			pwm_R+=15*count_R_1+8*(count_R_1-count_R_2)+2*count_sum_R;
		else//mode==1非同步模式 4 2 2
			pwm_R+=5*count_R_1+10*(count_R_1-count_R_2)+5*count_sum_R;
	if(pwm_R>2000)//左轮限制最大速度
		{
			pwm_R=2000;
		}
		if(pwm_R<0)//左轮限制最小速度
		{
			pwm_R=0;	
		}

		
		
		if(go)//正方向
		{		
			if(v==0)
			{
				PWM0DH = 0;//左轮高位PWM
				PWM0DL = 0;//左轮低位PWM
			}
			else
			{
				PWM0DH = pwm_R/255;//左轮高位PWM
				PWM0DL = pwm_R%255;//左轮低位PWM
			}
				

		}
		else //反方向
		{	
			if(v==0)
			{
				PWM0DTH = 0;//左轮高位PWM
				PWM0DTL = 0;//左轮低位PWM
			}
			else
			{
					PWM0DTH = pwm_R/255;//左轮高位PWM
					PWM0DTL = pwm_R%255;//左轮低位PWM
			}


		}
		num_R1 = num_R;
		num_R = 0;

}






/***************************************************************************************
  * @说明  	INT17中断服务函数
  *	@参数	无
  * @返回值 无
  * @注		无
***************************************************************************************/
void INT8_17_Rpt() interrupt INT8_17_VECTOR //右边
{
	if(PINTF2&0x02)						//判断INT17中断标志位
	{
		if(f2_mode!=3&&f2_mode!=11)//不在充电和待机
		{
			num_R++;
			num_R2++;
			if(run_stop_flag_R ==1)//90°直角开始标志，在开始直角中触发开启
			{
				if(num_R2>1050)
				{
					run_stop_flag =1;//90°直角停止标志，在中断内触发
					reset_pwm();//重置PWM
					run_stop_flag_R =0;//90°直角开始标志，在开始直角中触发开启
				}		
			}
			if(run_stop_flag_R ==2)//90°直角开始标志，在碰撞直角中触发开启
			{
				if(num_R2>1050)
				{
					run_stop_flag =1;//90°直角停止标志，在中断内触发
					reset_pwm();//重置PWM
					run_stop_flag_R =0;//90°直角开始标志，在碰撞直角中触发开启
				}		
			}			
		}
		PINTF2 &=~ 0x02;				//清除INT17中断标志位	
		
	}
}

/****************中断记录左边轮编码器****************************/
void P2INT_Rpt() interrupt P2INT_VECTOR 
{
	
	if(PINTF2&0x80)						//判断P2中断标志位
	{
		if(f2_mode!=3&&f2_mode!=11)//不在充电和待机
		{
			num_L++;
			num_L2++;
			if(run_stop_flag_L ==1)//90°直角开始标志，在开始直角中触发开启
			{
				if(num_L2>1050)
				{
					run_stop_flag =1;//90°直角停止标志，在中断内触发
					reset_pwm();//重置PWM
					run_stop_flag_L =0;//90°直角开始标志，在开始直角中触发开启
				}		
			}	
			if(run_stop_flag_L ==2)//90°直角开始标志，在碰撞直角中触发开启
			{
				if(num_L2>1050)
				{
					run_stop_flag =1;//90°直角停止标志，在中断内触发
					reset_pwm();//重置PWM
					run_stop_flag_L =0;//90°直角开始标志，在碰撞直角中触发开启
				}		
			}
			
		}
		PINTF2 &=~ 0x80;				//清除P2中断标志位	

	}
}	








