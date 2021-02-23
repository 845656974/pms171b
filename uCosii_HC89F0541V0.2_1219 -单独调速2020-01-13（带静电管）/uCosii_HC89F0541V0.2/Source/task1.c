#include "includes.h"
#include "Source\task1.h"
#include "Source\task3.h"
unsigned char guc_Uartflag = 0;			//UART判断标志位
unsigned char guc_Uartcnt = 0;		    //UART计数使用
unsigned char guc_Uartbuf_a[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//用于存放接收收据
unsigned char guc_i;					//发送数据计数	
bool wifi_first =0;//第一次开启 发送00  第二次发送01


int wifi_num_flag = 0;//呼吸指令
INT8U f1_wifi_mode=0;//控制方向时机器不会工作
//f2
extern INT8U ZJ_wifi_flag;//自检模式下测试wifi
extern int f2_wifi_num ;//1000发一次状态																	 
extern INT8U f2_mode ;//模式0停止 1弓形 2回充 3充电 4快闪 5慢闪 6异常 7方向按键 10定点 11待机 12充电下座 13延边
extern INT8U	BUZZER_flag ;//蜂鸣器
extern INT8U	BUZZER_num ;//蜂鸣器计数器，用于响应时间																	 
extern int f2_chongdian_flag1 ;//充电红跳绿																	 
extern INT8U f2_dianliang_wifi ;//电量 0-100					
extern INT8U error_flag ;//标志,1碰撞，2悬空，3边刷，4风机，5左轮，6右轮，7电量
extern INT8U error_fengji ;//计数，风机异常



//f3																	 
extern bool	f3_flag0 ;//按键启动标志，一般用于按键按下执行第一次操作																	 
extern INT8U 	f3_flag1 ;//记录标志，一般用于第一次复位

extern int f3_flag ;//步骤运行，共100多步骤
extern INT32U f3_fengji ;
extern bool fan_open ;//风机开启1,关闭0

//发送指令 1快闪 2慢闪 3测试wifi
INT8U f1_wifi_num = 0;
INT8U f1_wifi_i1 = 0;//发送dp数据
INT8U f1_wifi_i2 = 0;//发送dp数据

//风机
INT8U f1_fj_xili =0;//风机吸力 0强 1中 2若
INT32U f1_fj_xili1 =1000;//风机转速10000 8000 6000
INT8U f1_fj_xili2 =0;//风机转速

INT16U f1_fj_num = 0;//风机异常计数器

int f1_i=0;
void Task1(void *ppdata) reentrant
{


	ppdata = ppdata;
//	OSTimeDly(100);

	while(1){
		OSTimeDly(5);	

			
		if(f2_mode==11)
		{
			for(f1_i=0;f1_i<12;f1_i++)
			{
				
				guc_Uartbuf_a[f1_i]=0;
			}
			
			guc_Uartcnt=0;
			SCON &=~ 0x70;			
		}
		else
		{
/***********************接收&处理指令函数*******************************/		
		uart1_data();					//接收&处理指令函数
/***********************定时发送扫地机状态******************************/				
		f1_timing_case();			//定时发送扫地机状态		
		
		
		}

/***********************风机转速*****************************************/			
		f1_fan_open()	;	
		
		
		

		
}	}





void f1_fan_open(void)	//风机开启
{
if(f1_fj_num<1000)//风机计数器
{
	f1_fj_num++;
	error_fengji = 0;//风机异常
}
	
//if((f2_mode==1||f2_mode==10||f2_mode==13))//风机延迟2秒启动
if(fan_open)//风机延迟2秒启动
{
//f1_fj_xili =0;//风机吸力 0强 1中 2弱
switch(f1_fj_xili)
{
	case 0://强
		f1_fj_xili1=1000;
		break;
	case 1://中
		f1_fj_xili1=850;
		break;
	case 2://弱
		f1_fj_xili1=700;
		break;

}
	
if(f1_fj_xili1<550)
	error_fengji = 0;//风机异常
if(f3_fengji<f1_fj_xili1)//启动	
{

		error_fengji = 0;//风机异常
	f3_fengji++;
}			

else
	f3_fengji=f1_fj_xili1;


if(f3_fengji<400)//首次启动
	f3_fengji=400;


if(f1_fj_xili==0)//强力
{
	PWM2DH = (f3_fengji*2)/255;//左轮高位PWM
	PWM2DL = (f3_fengji*2)%255;//左轮低位PWM
	
	
	
	
//	if( (f3_fengji%10) < (f3_fengji/100) )
//		fan =1;
//	else
//		fan =0;
//	if(f3_fengji>=1000)//100
//		fan =1;
}
else//中以及低
{
	
		PWM2DH = (f3_fengji*2)/255;//左轮高位PWM
		PWM2DL = (f3_fengji*2)%255;//左轮低位PWM
	
	
	
//	if(f3_fengji<f1_fj_xili1)//启动
//	{
//		if( (f3_fengji%10) < (f3_fengji/100) )
//			fan =1;
//		else
//			fan =0;
//		
//	}
//	else//保持
//	{
//		f1_fj_xili2++;
//		if(f1_fj_xili2<f1_fj_xili1/100)
//			fan =1;
//		else
//			fan =0;
//		if(f1_fj_xili2==10)
//			f1_fj_xili2=0;
//	
//	}
}
	

	if(f3_fengji>=10000)//100
	{
		PWM2DH =	0x07;//左轮高位PWM
		PWM2DL =	0xff;//左轮低位PWM
	
	
	
	}
//		fan =1;
}
else
	{
		f1_fj_num = 0;//风机异常计数器
		PWM2DH = 0;//左轮高位PWM
		PWM2DL = 0;//左轮低位PWM
		
		
//		fan = 0;
	}
		



}









void f1_timing_case(void)	//定时发送扫地机状态
{
		wifi_num_flag++;
		if(wifi_num_flag>2000)
		{
			wifi_num_flag=0;
			guc_Uartflag = 0;
			guc_Uartcnt = 0;
			SCON2 = 0x02;						//8位UART，波特率可变
			SCON = 0x10;						//允许串行接收
			IE |= 0x10;							//使能串口中断	
		
		}
}

void uart1_data(void)					//接收函数
{
if(f1_wifi_num)//快闪&&慢闪&&测试wifi
{
//	SCON &= ~0x10;      //关闭串行接收
	if(f1_wifi_num==1)//快闪
	{
		for(guc_i= 0;guc_i<8;guc_i++)
		{
			SBUF = Buff6[guc_i];//发送8位串口数据
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//清除发送中断标志位
		}
	}
	else if(f1_wifi_num==2)//慢闪
	{
		for(guc_i= 0;guc_i<8;guc_i++)
		{
			SBUF = Buff7[guc_i];//发送8位串口数据
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//清除发送中断标志位
		}
	}
	else if(f1_wifi_num ==3)//测试wifi
	{
		for(guc_i= 0;guc_i<7;guc_i++)
		{
			SBUF = Buff8[guc_i];//发送8位串口数据
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//清除发送中断标志位
		}
	}
	SCON |= 0x10;      //允许串行接收
	f1_wifi_num=0;		
}	


	
if(guc_Uartflag)//接收完成标志
{
	
	IE &=~ 0x10;				//失能UART1中断
	if(guc_Uartbuf_a[0]==0x55)
	{
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==0)//心跳检测
		{
			wifi_num_flag=0;
			if(wifi_first==0)
			{
				wifi_first=1;
				for(guc_i= 0;guc_i<8;guc_i++)
				{
					SBUF = Buff1[guc_i];//发送8位串口数据
					while(!(SCON & 0x02));
					SCON &=~ 0x02;			//清除发送中断标志位
				}
			}				
			else
			{
				for(guc_i= 0;guc_i<8;guc_i++)
				{
					SBUF = Buff2[guc_i];//发送8位串口数据
					while(!(SCON & 0x02));
					SCON &=~ 0x02;			//清除发送中断标志位
				}
			}	
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==1)//查询产品信息
		{
			for(guc_i= 0;guc_i<49;guc_i++)
			{
				SBUF = Buff3[guc_i];//发送8位串口数据
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//清除发送中断标志位
			}
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==2)//查询 MCU 设定模块工作方式
		{
			for(guc_i= 0;guc_i<7;guc_i++)
			{
				SBUF = Buff4[guc_i];//发送8位串口数据
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//清除发送中断标志位
			}
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==3)//报告 WiFi 工作状态
		{
			for(guc_i= 0;guc_i<7;guc_i++)
			{
				SBUF = Buff5[guc_i];//发送8位串口数据
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//清除发送中断标志位
			}
		}
		if(guc_Uartbuf_a[2]==0&&(guc_Uartbuf_a[3]==8||guc_Uartbuf_a[3]==3))//查询 mcu 工作状态
		{
			f2_wifi_num=900;//发送指令;	//查询 mcu 工作状态
		}
		if(guc_Uartbuf_a[3]==0x0e&&guc_Uartbuf_a[5]==2)//自检模式测试wifi
		{
			ZJ_wifi_flag=1;//自检模式下测试wifi
		}
		
		
		
		
		
	//清扫开关&&方向
		if(guc_Uartbuf_a[3]==6)
		{
			f2_wifi_num=900;//发送指令
			if(guc_Uartbuf_a[6]==2&&guc_Uartbuf_a[10]==1)	//清扫开关,打开
			{
				f2_mode=1;
				f3_flag0 = 1;
				BUZZER_num=30;//蜂鸣器鸣叫时间
				BUZZER_flag=1;//开启蜂鸣器
			}
			if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==1)	//自动模式
			{
				if(f2_mode==0)
				{
					f2_mode=1;
					f3_flag0 = 1;		
				}
				else
				{
					f2_mode=0;
					f3_flag = -1;
					f3_flag1=0;
				}
				BUZZER_num=30;//蜂鸣器鸣叫时间
				BUZZER_flag=1;//开启蜂鸣器
			}
		///***********************重点清扫*******************************/					
				if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==2)	//重点清扫
				{
					if(f2_mode==0)
					{
						f2_mode=10;//重点清扫
						f3_flag0 = 1;		
					}
					else
					{
						f2_mode=0;
						f3_flag = -1;
						f3_flag1=0;
					}
					
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器
				}	
			if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==0)	//待机
				{
					f2_mode=0;
					f3_flag = -1;
					f3_flag1=0;
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器
				}
			if(guc_Uartbuf_a[6]==2&&guc_Uartbuf_a[10]==0)	//清扫开关,关闭
			{
				f2_mode=0;
				f3_flag = -1;
				f3_flag1=0;
				BUZZER_num=30;//蜂鸣器鸣叫时间
				BUZZER_flag=1;//开启蜂鸣器
			}
			if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==0)	//前进
				{
					f1_fj_num=0;
					if(f2_mode==1)
						f1_wifi_mode=1;
					else
						f2_mode=0;
					f2_mode=7;
					f3_flag = 220;
					f3_flag1=0;
					
				}
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==1)	//后退
				{
					f1_fj_num=0;
					if(f2_mode==1)
						f1_wifi_mode=1;
					else
						f2_mode=0;
					f2_mode=7;
					f3_flag = 221;
					f3_flag1=0;
					
				}
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==2)	//左
				{
					f1_fj_num=0;
					if(f2_mode==1)
						f1_wifi_mode=1;
					else
						f2_mode=0;
					f2_mode=7;
					f3_flag = 222;
					f3_flag1=0;
					
				}
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==3)	//右
				{
					f1_fj_num=0;
					if(f2_mode==1)
						f1_wifi_mode=1;
					else
						f2_mode=0;
					f2_mode=7;
					f3_flag = 223;
					f3_flag1=0;
					
				}
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==4)	//停止
				{
					if(f1_wifi_mode==1)//运动中使用按键，松开时候继续运动
					{
						f1_fj_num=0;
						reset_pwm();
						f1_wifi_mode=0;
						f2_mode=1;
						f3_flag0 = 1;		
						Brush=1;//边刷
//						fan=1;//风机
						fan_open = 1;//风机开启1,关闭0
					}
					else//静止中使用按键，松开时候停止运动
					{
						f1_fj_num=0;
						Brush=0;//边刷
						fan=0;//风机
						fan_open = 0;//风机开启1,关闭0
						f2_mode=0;
						f3_flag = -1;
						f3_flag1=0;
					}
					
					
				}
				if(guc_Uartbuf_a[6]==13&&guc_Uartbuf_a[10]==1)//寻找机器
				{
					for(guc_i= 0;guc_i<12;guc_i++)
					{
						SBUF = Buff11[guc_i];//发送8位串口数据
						while(!(SCON & 0x02));
						SCON &=~ 0x02;			//清除发送中断标志位
					}	
					BUZZER_num=200;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器
				}
				
				if(guc_Uartbuf_a[6]==14&&guc_Uartbuf_a[7]==4)//吸力选择
				{
					Buff12[10]=guc_Uartbuf_a[10];
					f1_fj_xili =Buff12[10];//风机吸力
					Buff12[11]=33+guc_Uartbuf_a[10];
					for(guc_i= 0;guc_i<12;guc_i++)
					{
						SBUF = Buff12[guc_i];//发送8位串口数据
						while(!(SCON & 0x02));
						SCON &=~ 0x02;			//清除发送中断标志位
					}	
				
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器
				}
		}	
	}
	
	
	
//	for(guc_i= 0;guc_i<guc_Uartcnt;guc_i++)
//	{
//		SBUF = guc_Uartbuf_a[guc_i];//发送8位串口数据
//		while(!(SCON & 0x02));
//		SCON &=~ 0x02;			//清除发送中断标志位
//	}
	IE |= 0x10;					//UART1中断使能
	guc_Uartflag = 0;		//接收完成标志
	guc_Uartcnt = 0;		//接收数据长度
	SCON |= 0x10;				//UART1接收使能
}	



	f1_wifi_i1 = 0;//5条DP指令
		if(f2_wifi_num >1000)
		{
			f2_wifi_num=0;
			f1_wifi_i2 = 0;//DP具体指令
			IE &=~ 0x10;				//失能UART1中断
			for(f1_wifi_i1=0;f1_wifi_i1<5;f1_wifi_i1++)//5条DP指令
			{
				for(f1_wifi_i2=0;f1_wifi_i2<12;f1_wifi_i2++)////DP具体指令
				{
					if(f1_wifi_i2<10)
						Buff[f1_wifi_i2]=Buff10[f1_wifi_i1][f1_wifi_i2];
					else
					{
						if(f1_wifi_i2==10)//功能指令
						{
							if(f1_wifi_i1==0)//清扫开关
							{
								if(f2_mode==1||f2_mode==2||f2_mode==10)//1弓形，2回冲，10定点 
									Buff[f1_wifi_i2] = 1;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
								{
									SBUF = Buff[guc_i];//发送8位串口数据
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//清除发送中断标志位
								}
								
								
							}				
							if(f1_wifi_i1==1)//工作模式
							{
								if(f2_mode==1)//1弓形
									Buff[f1_wifi_i2] = 1;
								else if	(f2_mode==10)//10定点
									Buff[f1_wifi_i2] = 2;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
								{
									SBUF = Buff[guc_i];//发送8位串口数据
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//清除发送中断标志位
								}
								
							}							
							
							if(f1_wifi_i1==2)//工作状态
							{
								if(f2_mode==1)//1弓形
									Buff[f1_wifi_i2] = 1;
								else if	(f2_mode==10)//10定点
									Buff[f1_wifi_i2] = 2;
								else if(f2_mode==3)//充电
								{
									if(f2_chongdian_flag1<100)//充电红跳绿
										Buff[f1_wifi_i2] = 3;
									else
										Buff[f1_wifi_i2] = 4;
								}																
								else
									Buff[f1_wifi_i2] = 0;
								
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
								{
									SBUF = Buff[guc_i];//发送8位串口数据
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//清除发送中断标志位
								}
								
							}
							if(f1_wifi_i1==3)//电量
							{
								
								Buff13[13]= f2_dianliang_wifi;
								Buff13[14]= f2_dianliang_wifi+29;
								f1_wifi_i2++;
								for(guc_i= 0;guc_i<15;guc_i++)//报告 WiFi 工作状态
								{
									SBUF = Buff13[guc_i];//发送8位串口数据
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//清除发送中断标志位
								}
								
//								Buff[f1_wifi_i2] = f2_dianliang_wifi;
//								f1_wifi_i2++;
//								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
//								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
//								{
//									SBUF = Buff[guc_i];//发送8位串口数据
//									while(!(SCON & 0x02));
//									SCON &=~ 0x02;			//清除发送中断标志位
//								}
//								
//								
//								
//Buff13[15] = {0x55,0xaa,0x03,0x07,0x00,0x08,0x06,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x1D};//剩余电流

								
								
								
//								if(f1_wifi_i2<10)
//									Buff[f1_wifi_i2]=Buff10[f1_wifi_i1][f1_wifi_i2];
//								Buff13[10] =  f2_dianliang_wifi;
//								Buff13[11] =  f2_dianliang_wifi+29;
//								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
//								{
//									SBUF = Buff13[guc_i];//发送8位串口数据
//									while(!(SCON & 0x02));
//									SCON &=~ 0x02;			//清除发送中断标志位
//								}
							}
							if(f1_wifi_i1==4)//故障警告
							{
								if(error_flag==1)//1碰撞
									Buff[f1_wifi_i2] = 8;
								else if	(error_flag==2)//2悬空
									Buff[f1_wifi_i2] = 16;
								else if	(error_flag==3)//3边刷
									Buff[f1_wifi_i2] = 4;
								else if	(error_flag==4)//4风机
									Buff[f1_wifi_i2] = 64;
								else if	(error_flag==5)//5左轮
									Buff[f1_wifi_i2] = 1;
								else if	(error_flag==6)//6右轮
									Buff[f1_wifi_i2] = 2;
								else if	(error_flag==7)//7电池电量
									Buff[f1_wifi_i2] = 32;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//报告 WiFi 工作状态
								{
									SBUF = Buff[guc_i];//发送8位串口数据
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//清除发送中断标志位
								}
							}
						}
					}

				}
			}
	IE |= 0x10;					//UART1中断使能
	guc_Uartflag = 0;
	guc_Uartcnt = 0;
	SCON |= 0x10;				//UART1接收使能
		}	
}



/***************************************************************************************
  * @说明  	UART1中断服务函数
  *	@参数	无
  * @返回值 无
  * @注		无
***************************************************************************************/

void UART1_Rpt(void) interrupt UART1_VECTOR
{
	if(SCON & 0x01)						//判断接收中断标志位
	{
		if((guc_Uartcnt==0&&SBUF==85&&guc_Uartcnt<12)||(guc_Uartcnt!=0&&guc_Uartbuf_a[0]==85&&guc_Uartcnt<12))
		{
			guc_Uartbuf_a[guc_Uartcnt++] = SBUF;//转存8位串口接收数据
			if(guc_Uartcnt == 7&& (guc_Uartbuf_a[3]<=2 ||guc_Uartbuf_a[3]==8))
			{
					SCON &=~ 0x10;				//失能UART1接收			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 8&&guc_Uartbuf_a[3]==3)
			{
					SCON &=~ 0x10;				//失能UART1接收			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 9&&guc_Uartbuf_a[3]==14)//WiFi测试
			{
					SCON &=~ 0x10;				//失能UART1接收			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 12)
			{
					SCON &=~ 0x10;				//失能UART1接收			
					guc_Uartflag = 1;
			}
		  SCON &=~ 0x01;					//清除接收中断标志位	
		}
		if((guc_Uartcnt!=0&&SBUF == 85))
		{
			guc_Uartbuf_a[0] = SBUF;//转存8位串口接收数据
			guc_Uartcnt	=1;
		}
		if(guc_Uartcnt==12)
			guc_Uartcnt=0;
	}							
}

















