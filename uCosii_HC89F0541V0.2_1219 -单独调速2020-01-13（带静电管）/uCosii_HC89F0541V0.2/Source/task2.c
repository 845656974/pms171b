#include "includes.h"
#include "Source\task2.h"
/**********************************************
*指示灯
*按键&自检
*
************************************/
unsigned char guc_Uartbuf_a1[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//用于存放接收收据
unsigned char guc_AdcFlag  = 0;         //ADC转换完成标志位


unsigned int f2_kaiji = 0;//用于开机灯闪3次计数 
unsigned int f2_ap = 0;//快闪慢闪模式 3秒快闪5秒慢闪



INT8U f2_LED = 0;//LED闪烁计数
INT8U	BUZZER_flag = 1;//蜂鸣器
INT8U	BUZZER_num = 30;//蜂鸣器计数器，用于响应时间

INT8U f2_mode = 1;//模式0停止 1弓形 2回充 3充电 4快闪 5慢闪 6异常 7方向按键 10定点 11待机 12充电下座 13延边
INT8U TK_ZJ_num=0;//按键自检，过了3就不进入
int TK_ZJ_num1=0;//按键自检，过了5s就不进入
INT8U TK_ZJ=0;//自检模式
INT8U TK_ZJ1=0;//自检模式,防止松开是跳出
INT8U ZJ_num=0;//自检模式下计数
INT8U ZJ_num1=0;//自检模式下项目计数
INT8U ZJ_num2=0;//自检模式下项目计数


INT8U TK_L_num=0;//按键左防抖动
INT8U TK_R_num=0;//按键右防抖动


INT8U ZJ_wifi_flag=0;//自检模式下测试wifi
INT8U TK_ZJ_L=0;//自检模式,左按键
INT8U TK_ZJ_R=0;//自检模式,右按键

/**************充电********************/
INT8U f2_chongdian_flag = 0;//充电插入计数器
INT32U f2_chongdian_jishuqi=0;//3小时自动跳
INT32U f2_chongdian_jishuqi2=0;//开关关闭计时器,防止电压不稳
int f2_chongdian_flag1 = 0;//充电红跳绿
INT8U bat_mode = 0;//充电模式  1快速充电 2涓流充电  3充电完成
INT8U bat_i =0;//防止抖动
INT32U bat_i1 =0;//涓流防止抖动
/*******************待机*****************/
INT32U f2_daiji_flag1 = 0;//待机状态

/**********异常*********/
int f2_buzzer_num = 0;//蜂鸣器
INT8U error_pengzhuang = 0;//计数，碰撞异常
INT8U error_xuankong = 0;//计数，悬空异常
INT8U error_bianshua = 0;//计数，边刷异常
INT8U error_fengji = 0;//计数，风机异常
INT8U error_luanzi_L = 0;//计数，风机异常
INT8U error_luanzi_R = 0;//计数，风机异常
INT8U error_dianliang = 0;//计数，电量低
INT8U error_flag = 0;//标志,1碰撞，2悬空，3边刷，4风机，5左轮，6右轮，7电量
int f2_dianliang =0;//电量 0-100
INT8U f2_dianliang_wifi = 80;//wifi发送电量
INT8U f2_dianliang_num  = 0;//wifi发送电量计数器
int f2_dianliang_sum[10]={0,0,0,0,0,0,0,0,0,0};//wifi发送电量求和
INT8U error_dj2=0;//多个地检悬空暂停延迟
/************wifi****************/
int f2_wifi_num = 0;//1000发一次状态



/****************ADC*******************/
INT16U ADC_0,ADC_1, ADC_2,ADC_3,ADC_4, ADC_5,ADC_6,ADC_7;
INT16U DJ_L_ADC,DJ_M_ADC,DJ_R_ADC;	
INT16U DJ_L_flag,DJ_M_flag,DJ_R_flag;//1为异常0位正常	
INT8U DJ_L_num,DJ_M_num,DJ_R_num;//ADC抖动
bool sensor_error = 0;//传感器状态，地检墙检碰撞
bool sensor_error_dj = 0;//传感器状态，地检

INT8U ADC_ait = 0;

//f1
extern INT8U f1_wifi_num ;//发送指令 1快闪 2慢闪 3测试wifi
//风机
//风机
extern INT8U f1_fj_xili;//风机吸力 0强 1中 2若
extern INT8U f1_fj_xili2 ;//风机转速

//f3
extern int f3_flag;//步骤运行，共100多步骤
extern bool f3_flag0;//按键启动标志，一般用于按键按下执行第一次操作
extern INT8U f3_flag1;//记录标志，一般用于第一次复位
extern INT8U num_L1 ;			//左轮码盘值
extern INT8U  num_R1;				//右轮码盘值
extern bool fan_open ;//风机开启1,关闭0

void Task2(void *ppdata) reentrant
{
	ppdata = ppdata;
	OSTimeDly(100);

	while(1){	
/***********************看门狗*******************************/
	WDTC |= 0x10;                   //清狗
/*************************************************************/		
	f2_wifi_num++;//1000发一次状态	
/***********************LED灯状态显示***************************/		
	f2_ledshow();
/***********************蜂鸣器**********************************/				
	f2_BUZZER();		
/***********************按键检测********************************/			
	f2_touch();
/***********************待机切换*******************************/	
	f2_standby();//待机切换	
/***********************自检模式(出厂自检)**********************/		
	f2_ZJ_MODE();//自检模式
//	OSTimeDly(30);

	if(f2_mode!=11)
	{
		f2_Get_ADC(0);//获取ADC,下次采集为vdd
		OSTimeDly(1);
		DJ_L_ADC=ADC_5;//左地检，此时地检红外打开，接收值应比关闭红外低
		DJ_M_ADC=ADC_6;//中地检，此时地检红外打开，接收值应比关闭红外低
		DJ_R_ADC=ADC_7;//右地检，此时地检红外打开，接收值应比关闭红外低
	}

if(ADC_ait==0)
	{
/***********************异常检测********************************/	
	f2_error();
/***********************异常报警********************************/		
	f2_error_buzzer();		
/***********************充电检测********************************/		
	f2_power();
/************************电池充电&电量&电池异常报错*************/	
	f2_bat();//电池充电&电量 			
}	
	
	
	floor = 0;//地检使能关闭
	OSTimeDly(2);

	if(f2_mode!=11)
	{
		if(ADC_ait)
		{
			f2_Get_ADC(ADC_ait);//获取ADC,下次采集为4V,本次为VDD,为地检ADC
			OSTimeDly(2);
			ADC_ait=0;
		}
		else
		{
			f2_Get_ADC(ADC_ait);//获取ADC,下次采集为VDD,本次为4V,为其他ADC
			OSTimeDly(2);
			ADC_ait=1;		
		}		
	}
	
	


	if(f2_mode!=3&&f2_mode!=11&&ADC_ait==0)
	{
		/*************地检检测************************/		
			f2_DJ_mode(ADC_5,DJ_L_ADC,1);//左地检
			f2_DJ_mode(ADC_6,DJ_M_ADC,2);//左地检
			f2_DJ_mode(ADC_7,DJ_R_ADC,3);//左地检	
	
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = ADC_5/100;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位		
//		
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = ADC_5%100;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位				
//		
//		
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = DJ_L_ADC/100;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位		
//		
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = DJ_L_ADC%100;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位			
//		
//		
//			IE &=~ 0x10;				//允许UART1中断
//			SBUF = 170;//task1发送8位串口数据
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//清除发送中断标志位			

	}


	
	if(f2_mode!=3&&f2_mode!=11)//不在充电和待机
		floor = 1;//地检使能打开
	
	

	
	
	OSTimeDly(1);
	}}

	
void f2_standby(void)//待机切换	
{
/**********待机切换**************************/
if((f2_mode==4||f2_mode==5)&&f2_daiji_flag1<standby_wifi_time)	//快慢闪切换
{
		if(f2_dianliang<=10)//电量小于10
			f2_daiji_flag1=f2_daiji_flag1+10;
		else
			f2_daiji_flag1++;
}
if((f2_mode==4||f2_mode==5)&&f2_daiji_flag1>=standby_wifi_time)	//快慢闪切换
{
	f2_mode=0;
	f3_flag=-1;
	f3_flag1=0;
	f2_daiji_flag1 = 0;
}		
if(f2_mode==6&&f2_daiji_flag1<standby_error_time)	//异常切换
{
		if(f2_dianliang<13&&f2_dianliang>5)//电量小于10
			f2_daiji_flag1++;
		else if(f2_dianliang<=5)
			f2_daiji_flag1=f2_daiji_flag1+10;
		else
			f2_daiji_flag1++;
}		
if(f2_mode==6&&f2_daiji_flag1>=standby_error_time)//异常切换
{
	if(error_flag==7)//电量异常
	{
		f2_mode=11;//休眠模式
		Wifi =0;//关闭wifi
	}
	else
		f2_mode=0;//待机模式
	f2_daiji_flag1=0;
}		
if(f2_mode==0&&f2_daiji_flag1<standby_sleep_time&&TK_ZJ!=1)//停止到待机
	f2_daiji_flag1++;		
if(f2_mode==0&&f2_daiji_flag1>=standby_sleep_time&&TK_ZJ!=1)//停止到待机
{
	Wifi=0;	//关闭wifi
	f2_mode=11;
	f2_daiji_flag1=0;
}			
if(f2_mode==11)//关闭wifi
{
	if(f2_daiji_flag1<10)
		f2_daiji_flag1++;
	if(f2_daiji_flag1==10)
	{
		f2_daiji_flag1++;
		Wifi=0;//关闭wifi
		mapan = 0;//5V供电使能关闭
		/***********************************进入空闲模式***************************************/
		PCON |= 0x01;						//进入空闲模式
	}
	
}	

}

	
void f2_bat(void)//电池充电&电量&电池异常报错
{
	if(f2_mode!=11)
	{
		if(f2_mode!=3)//非充电模式
		{
			if(ADC_4>=2830)//100%电量
				f2_dianliang=100;
			else//5.4
				f2_dianliang=100-(2820-ADC_4)/5.88;
			if( (f2_mode==1||f2_mode==10||f2_mode==7)&&TK_ZJ==0)//工作模式下
			{
				if(f1_fj_xili==0)//风机吸力 0强 1中 2若
					f2_dianliang+=30;
				else if(f1_fj_xili==1)
					f2_dianliang+=20;
				else
					f2_dianliang+=10;
			
			}
				
		}
		else//充电模式
		{
			if(bat_mode!=2)
			{
				if(ADC_4>=2820)//100%电量
					f2_dianliang=100;
				else
					f2_dianliang=(100-(2820-ADC_4)/5.5);
			}

		
		}
		if(f2_dianliang>100)
			f2_dianliang=100;
		if(f2_dianliang<5)
			f2_dianliang=5;

		//求和20次,然后wifi发送
			if(f2_dianliang_num<10)
			{
				f2_dianliang_num++;
				f2_dianliang_sum[f2_dianliang_num]=f2_dianliang;
//				f2_dianliang_sum+=f2_dianliang;
			}
			else
			{
				bubbleSort(f2_dianliang_sum,10);
				f2_dianliang_wifi=f2_dianliang_sum[5];
				f2_dianliang_num=0;
			}

		
	/**********低电量检测***********************/		
		if(f2_dianliang_wifi<=15&&TK_ZJ==0&&f2_mode!=3)
		{
			if(error_dianliang<1000)
				error_dianliang++;
		}
		else
		{
//			if(error_dianliang>10)
//				error_dianliang=error_dianliang-10;
//			else
				error_dianliang=0;
		
		}
		if(error_dianliang==1000)//异常报错
		{
			error_dianliang++;
			error_flag = 7;
			f2_mode = 6;//异常		
			f3_flag=-1;//步骤-1
			f3_flag1=0;
		}	
	/**********充电***********************/				
		if(f2_mode==3)
		{

			error_dianliang=0;
			
			if(f2_chongdian_jishuqi<1080000)//3小时自动跳
				f2_chongdian_jishuqi++;
			else
				f2_chongdian_flag1=100;//亮绿灯
			//bat_mode = 1;//充电模式  1快速充电 2涓流充电  3充电完成
			if(bat_mode==1)//充电模式  1快速充电
			{
				if(power_enabled )
				{
					if(f2_chongdian_jishuqi2<=50)
						f2_chongdian_jishuqi2++;
				}
				else
					f2_chongdian_jishuqi2=0;
				if(f2_chongdian_jishuqi2>=50)
				{
					bat_mode = 2;
					f2_chongdian_jishuqi2 = 0;
				}	
			}
			if(bat_mode==2)//充电模式  2涓流充电
			{
				f2_chongdian_flag1=100;//亮绿灯	
			}	
			

			

			
		}
		

	}

}




void f2_power(void)//充电检测	
{
//充电检测
	if(power==0&&f2_mode!=3&&TK_ZJ==0)
	{
		if(f2_chongdian_flag<3)
		{
			bat_mode = 0;//充电模式  1快速充电 2涓流充电  3充电完成
			f2_chongdian_flag++;//充电插入计数器
			f2_chongdian_flag1=0;//充电红跳绿
			f2_chongdian_jishuqi=0;//3小时自动跳，1000000
		}
		if(f2_chongdian_flag==3)
		{
			bat_i =0;//防止抖动
			bat_mode = 1;//充电模式  1快速充电 2涓流充电  3充电完成
			fan = 0;//关闭风机
			fan_open = 0;//风机开启1,关闭0
			Brush = 0;//关闭边刷
			floor = 0;//地检使能关闭
			f2_wifi_num=980;//发送指令
			BUZZER_num=30;//蜂鸣器鸣叫时间
			BUZZER_flag=1;//开启蜂鸣器	
			f3_flag=-1;
			f2_kaiji = 300;//开机灯闪3次
			f3_flag1 = 0;
			f2_mode = 3;
		}

	}
//拔掉充电
	if(power==1&&f2_mode==3&&TK_ZJ==0)	
	{
		if(f2_chongdian_flag==10)//充电插入计数器
		{
			f2_chongdian_flag--;
			BUZZER_num=30;//蜂鸣器鸣叫时间
			BUZZER_flag=1;//开启蜂鸣器
			f2_kaiji = 300;
		}
		if(f2_chongdian_flag<10)//充电插入计数器
		{
			f2_chongdian_flag--;
			if(f2_chongdian_flag==0)
			{
				f2_wifi_num=980;//发送指令
				f2_kaiji = 300;//开机灯闪
				f2_mode=0;//停止
				f3_flag0 = 1;	//按键启动标志，一般用于按键按下执行第一次操作
				BUZZER_num=30;//蜂鸣器鸣叫时间
				BUZZER_flag=1;//开启蜂鸣器
			}
		}
	}

}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
void f2_touch(void)//按键检测	
{
	if(L1_touch==~touch_test)//左按键按下去
	{
		if(TK_L_num<3)//防抖动
		{
			TK_L_num++;
		}
	}
	if(R1_touch==~touch_test)//右按键按下去
	{
		if(TK_R_num<3)//防抖动
		{
			TK_R_num++;
		}
		if(f2_ap<500&&TK_ZJ==0)//AP模式 
			f2_ap++;//AP模式  3秒快闪5秒慢闪
		if(f2_ap==300)
		{
			f2_daiji_flag1=0;//待机计数器
			f2_mode=4;//快闪
			f3_flag0 = 1;
			BUZZER_flag=1;//开启蜂鸣器
			BUZZER_num=30;//蜂鸣器鸣叫时间
		}			
		if(f2_ap==499)
		{
			f2_daiji_flag1=0;//待机计数器
			f2_mode=5;//慢闪
			BUZZER_num=30;//蜂鸣器鸣叫时间
			BUZZER_flag=1;//开启蜂鸣器
		
		}
	
	}
	if(L1_touch==touch_test)//左按键抬起
	{
		if(TK_L_num==3)
		{
			f2_daiji_flag1 = 0;//待机计数器
			error_dianliang = 0;//电量异常
			f2_kaiji=300;
			TK_ZJ_num++;//自检模式计数器，过3不再进去
			BUZZER_flag=1;//开启蜂鸣器
			BUZZER_num=30;//蜂鸣器鸣叫时间
			if(TK_ZJ&&TK_ZJ1)//自检模式
			{
				ZJ_num2=0;//自检模式下项目计数
				TK_ZJ_L++;//左项目
				TK_ZJ_R = 0;//小项目清零		
			}
			if(f2_mode==11)//待机模式唤醒
			{
				Wifi = 1;
				mapan = 1;//5V供电使能开启
				/***********************************退出空闲模式***************************************/
					PCON &= ~0x01;						//进入空闲模式
			}	
			if(f2_mode==0&&TK_ZJ==0)
				{
					f2_mode=1;//弓形
					f3_flag0 = 1;
				}				
				else
				{
					f2_mode=0;//停止
					f3_flag0 = 1;	
				}	
			f2_wifi_num=950;//发送指令
		}
	

	
		TK_L_num=0;//防抖动
	}
	if(R1_touch==touch_test)//右按键抬起
	{
		if(f2_ap<300)//没有进入快闪或者慢闪
		{
			if(TK_R_num==3)
			{
				f2_daiji_flag1 = 0;//待机计数器
				error_dianliang = 0;//电量异常
				f2_kaiji=300;
				TK_ZJ_num++;//自检模式计数器，过3不再进去
				BUZZER_flag=1;//开启蜂鸣器
				BUZZER_num=30;//蜂鸣器鸣叫时间
				if(TK_ZJ&&TK_ZJ1)//自检模式
				{
					ZJ_num2=0;//自检模式下项目计数
					TK_ZJ_R++;//左项目
				}
				if(f2_mode==11)//待机模式唤醒
				{
					Wifi = 1;
					mapan = 1;//5V供电使能开启
					/***********************************退出空闲模式***************************************/
					PCON &= ~0x01;						//进入空闲模式
				}	
				if(f2_mode==0&&TK_ZJ==0)
				{
					f2_mode=10;//回冲
					f3_flag0 = 1;
				}	
				else
				{
					f2_mode=0;//停止
					f3_flag0 = 1;	
				}
				f2_wifi_num=950;//发送指令
			}
		}
		else if(f2_ap<499)
		{	
			f1_wifi_num=1;//快闪
		}		
		else
		{
			f1_wifi_num=2;//慢闪
		}
		f2_ap=0;//右按键计数器			
		TK_R_num=0;
	}
	if(TK_L_num==3&&TK_R_num==3&&TK_ZJ_num<3&&TK_ZJ==0&&f2_mode!=3)//自检
	{
		f3_flag =-1;
		f3_flag1=0;
		TK_ZJ =1;		
		BUZZER_flag=1;//开启蜂鸣器
		f2_kaiji = 300;//开机闪3次，这里为跳出
	}
//	if(TK_ZJ_num1<200)//2s后不在进入自检
//		TK_ZJ_num1++;
//	else
//		TK_ZJ_num=3;
	
	
	
	
	
	
}





void f2_error(void)
{
	INT16U error_crash_1 =DJ_L_flag+DJ_M_flag+DJ_R_flag;
	if(crash)
		error_crash_1++;
	if(error_crash_1>0)
	sensor_error=1;
	else
	sensor_error=0;
	if(DJ_L_flag+DJ_M_flag+DJ_R_flag>0)//地检标志
	sensor_error_dj=1;
	else
	sensor_error_dj=0;
	if(f2_mode==1||f2_mode==10)//弓形  旋转
	{
		/**********碰撞***********************/
		if(crash==0)
		error_pengzhuang=0;
	else
	{
		if(error_pengzhuang<200)
			error_pengzhuang++;		
	}
	if(error_pengzhuang==200)
	{
		f2_wifi_num=1001;//发送指令
		error_pengzhuang++;
		error_flag = 1;
		f2_mode = 6;//异常
				
		f3_flag=-1;//步骤-1
		f3_flag1=0;
	}
	/**********地检***********************/
	if((DJ_L_flag+DJ_M_flag+DJ_R_flag)<3&&(DJ_L_flag+DJ_M_flag+DJ_R_flag)>0)
	{
		error_dj2=0;
		if(error_xuankong<150)
			error_xuankong++;			
	}
	else if(DJ_L_flag+DJ_M_flag+DJ_R_flag==3)//扫地机拿起
	{
		error_dj2++;
		if(error_dj2>=50)
		{
			f3_flag=-1;//步骤-1
			f3_flag1=0;		
			f2_mode = 0;
			BUZZER_num=30;//蜂鸣器鸣叫时间
			BUZZER_flag=1;//开启蜂鸣器
			error_dj2=0;
		}
				
	}
	else
	{
		error_dj2=0;
		error_xuankong=0;
	}
		
	if(error_xuankong==150)
	{
		f2_wifi_num=1001;//发送指令
		error_xuankong++;
		error_flag = 2;
		f2_mode = 6;//异常		
		f3_flag=-1;//步骤-1
		f3_flag1=0;
	}
	/**********边刷***********************/
	if(ADC_3>100)//ST
	{
		if(error_bianshua<100)
			error_bianshua++;	
	}
	else
		error_bianshua=0;
	if(error_bianshua==100)
	{
		f2_wifi_num=1001;//发送指令
		error_bianshua++;
		error_flag = 3;
		f2_mode = 6;//异常		
		f3_flag=-1;//步骤-1
		f3_flag1=0;
	}
	/**********风机***********************/
	if(fan_open&&f2_mode!=3)
	{
		if(ADC_2>250)//ST
		{
			if(error_fengji<200)
				error_fengji++;	
		}
		else if(ADC_2<=250&&ADC_2>=50)
		{
			if(error_fengji>0)
				error_fengji--;	
			else
				error_fengji=0;	
		}
		else//ADC_2<=80
			error_fengji++;
		
		if(error_fengji==200)
		{
			f2_wifi_num=1001;//发送指令
			error_fengji++;
			error_flag = 4;
			f2_mode = 6;//异常		
			f3_flag=-1;//步骤-1
			f3_flag1=0;
		}		
		if(f2_mode==7)
		{
		error_fengji=0;
		
		}
	
	}
	
	

	
	
	/**********左轮***********************/
	if(ADC_1>80)//ST
//	if(ADC_10>400)//HK
	{
		if(error_luanzi_L<50)
			error_luanzi_L++;	
	}
	else
		error_luanzi_L=0;
	if(error_luanzi_L==30)
	{
		f2_wifi_num=1001;//发送指令
		error_luanzi_L++;
		error_flag = 5;
		f2_mode = 6;//异常		
		f3_flag=-1;//步骤-1
		f3_flag1=0;
	}
	/**********右轮***********************/
	if(ADC_0>80)//ST
	{
		if(error_luanzi_R<50)
			error_luanzi_R++;	
	}
	else
		error_luanzi_R=0;
	if(error_luanzi_R==30)
	{
		f2_wifi_num=1001;//发送指令
		error_luanzi_R++;
		error_flag = 6;
		f2_mode = 6;//异常		
		f3_flag=-1;//步骤-1
		f3_flag1=0;
	}	
	}
	
}
	
	
	
	
	
	
	
	
	

void f2_ZJ_MODE(void)//自检模式	
{
//自检模块	
	if(TK_ZJ)
	{
		if(TK_ZJ1==0 &&L1_touch==touch_test&&R1_touch==touch_test)
		{
			TK_ZJ1=1;
			f3_flag =-1;
			f3_flag1=0;
			Brush = 0;//边刷关闭
			fan = 0;//风机关闭
			fan_open = 0;//风机开启1,关闭0
			BUZZER_num=100;//蜂鸣器鸣叫时间
			ZJ_num= 0;//自检模式下计数
		}
		if(TK_ZJ_L%6==1)//自检大模块1.地检
		{
			f3_flag =-1;
			f3_flag1=0;
			Brush = 0;//边刷关闭
			fan = 0;//风机关闭
			fan_open = 0;//风机开启1,关闭0
			if(TK_ZJ_R%3==0)//自检项目1.1 左地检
			{
				if(DJ_L_flag)//离地
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%3==1)//自检项目1.1 左地检
			{
				if(DJ_M_flag)//离地
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%3==2)//自检项目1.1 左地检
			{
				if(DJ_R_flag)//离地
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
		}
		if(TK_ZJ_L%6==2)//自检大模块2.碰撞
		{	
			if(TK_ZJ_R%2==0)//自检项目2.1 碰撞
			{
				ZJ_wifi_flag = 0;
				ZJ_num1=0;//自检模式下计数
				ZJ_num2 = 0;//自检模式下计数
				if(crash)
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			else //wifi测试
			{
			 if(ZJ_num1==0)
			 {
				ZJ_num1=1;
				f1_wifi_num=3;//测试wifi
			 }
				if(ZJ_wifi_flag)
					f2_GRled(1,0);
				else
				{
					f2_GRled(0,1);
					ZJ_num1++;
					if(ZJ_num1>250)
						ZJ_num1=0;
				}
					
			}
		}
		if(TK_ZJ_L%6==3)//自检大模块3.电压电流测试
		{
			if(TK_ZJ_R%6==0)//自检项目3.1 左轮电流
			{
				if(ZJ_num2==0)
				{
					
					ZJ_num1=0;
					f3_flag=200;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}
				if(ADC_1>30)//左轮AD 
				{
					if(ZJ_num1<10)
						ZJ_num1++;	
				}
				else
				{
					if(ZJ_num1>0)
						ZJ_num1--;			
				}
				if(ZJ_num1>3)	
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==1)//自检项目3.2 右轮
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=201;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}	
				if(ADC_0 >30)//右轮AD 				
				{
					if(ZJ_num1<10)
						ZJ_num1++;	
				}
				else
				{
					if(ZJ_num1>0)
						ZJ_num1--;			
				}
				if(ZJ_num1>3)	
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==2)//自检项目3.3 风扇
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//步骤
					f3_flag1=0;
					ZJ_num2=1;
//					fan=1;//开启风机
					fan_open = 1;//风机开启1,关闭0
				}		
				if(f1_fj_xili2<900)//风机
				{
					if(ADC_2 >350)	//风扇 
						f2_GRled(0,1);
					else
						f2_GRled(1,0);
				}
				else
					if(ADC_2 >200)	//风扇 
						f2_GRled(0,1);
					else
						f2_GRled(1,0);

				
			}
			if(TK_ZJ_R%6==3)//自检项目3.4 边刷
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//步骤
					f3_flag1=0;
					ZJ_num2=1;
					Brush=1;//开启边刷
					fan=0;//关闭风机
					fan_open = 0;//风机开启1,关闭0
				}		
				if(ADC_3>30)	//边刷 
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==4)//自检项目3.5 电池电压
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//步骤
					f3_flag1=0;
					ZJ_num2=1;
					Brush=0;//关闭边刷
					fan=0;//关闭风机
					fan_open = 0;//风机开启1,关闭0
				}		
				if(ADC_4<1900)	//边刷 7v左右,20%的电压
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==5)//自检项目3.6 充电
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}		
				if(power==1)	//充电
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
		}
		if(TK_ZJ_L%6==4)//轮子速度测试
		{
			if(TK_ZJ_R%4==0)//自检项目4.1 左轮后退 码盘
			{	
				if(ZJ_num2==0)
				{
					Brush=0;//关闭边刷
					fan=0;//关闭风机
					fan_open = 0;//风机开启1,关闭0
					ZJ_num1=0;
					f3_flag=202;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}	
				if(num_L1<18||num_L1>30)
				{
					if(ZJ_num1<10)
						ZJ_num1++;
				}
				else
					ZJ_num1 =0;
				if(ZJ_num1>7)
					f2_GRled(0,1);
				else
					f2_GRled(1,0);	
			}
			if(TK_ZJ_R%4==1)//自检项目4.2 右轮后退 码盘
			{	
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=203;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}	
				if(num_R1<18||num_R1>30)
				{
					if(ZJ_num1<10)
						ZJ_num1++;
				}
				else
					ZJ_num1=0;
				if(ZJ_num1>7)
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}			
			if(TK_ZJ_R%4==2)//自检项目4.3 左右轮前进 边刷 风机
			{	
				if(ZJ_num2==0)
				{
					Brush=1;//开启边刷
//					fan=1;//开启风机
					fan_open = 1;//风机开启1,关闭0
					ZJ_num1=0;
					f3_flag=204;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}
				if(ADC_0>20||ADC_1>20||ADC_2>300||ADC_3>30)
				{
					if(ZJ_num1<5)
						ZJ_num1++;
				}
				if(ADC_0<20&&ADC_1<20&&ADC_2<300&&ADC_3<30)
					ZJ_num1=0;
				if(ZJ_num1==5)
					f2_GRled(0,1);
				else
					f2_GRled(1,0);	
			}
			if(TK_ZJ_R%4==3)//自检项目5.2 左右轮后退 边刷 风机
			{	
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=205;//步骤
					f3_flag1=0;
					ZJ_num2=1;
				}	
				if(ADC_0>20||ADC_1>20||ADC_2>300||ADC_3>30)
				{
					if(ZJ_num1<5)
						ZJ_num1++;
				}
				if(ADC_0<20&&ADC_1<20&&ADC_2<300&&ADC_3<30)
					ZJ_num1=0;
				if(ZJ_num1==5)
					f2_GRled(0,1);
				else
					f2_GRled(1,0);	
			}	
		}
		if(TK_ZJ_L%6==5||TK_ZJ_L%6==0)//全屏显示
		{
			if(ZJ_num2==0)
			{
				Brush=0;//关闭边刷
				fan=0;//关闭风机
				fan_open = 0;//风机开启1,关闭0
				ZJ_num1=0;
				f3_flag=-1;//步骤
				f3_flag1=0;
				ZJ_num2=1;
				}	
			if(ZJ_num1<10)
				f2_GRled(1,1);
			else
				f2_GRled(0,0);
			ZJ_num1++;
			if(ZJ_num1==20)
				ZJ_num1=0;	
		}

	}

}





void f2_DJ_mode(INT16U f2_DJ_ADC,INT16U f2_DJ_ADC_old,INT16U f2_DJ_ADC_flag )//地检
{
	bool f2_DJ_mode_i;//本次检测是否是异常
	if((f2_DJ_ADC_old-f2_DJ_ADC)<DJ_num_test&& (f2_DJ_ADC_old>f2_DJ_ADC) )
	{
		
		f2_DJ_mode_i=1;//墙检标志 1为异常
	}
	else
		f2_DJ_mode_i=0;//墙检标志 1为异常	
//	if(f2_DJ_ADC_old<=f2_DJ_ADC)//对着太阳或者悬空很高
//	{
//		f2_DJ_mode_i=1;//墙检标志 1为异常
//	}

		
	
	
	





		switch (f2_DJ_ADC_flag)//判断是哪个墙检碰墙
	{
		case 1://左
			if (f2_DJ_mode_i)
			{
				if(DJ_L_num<DJ_flag_sensitivity)
					DJ_L_num++;			
			}
			else	
				DJ_L_num = 0;
			break;
		case 2://中
			if (f2_DJ_mode_i)
			{
				if(DJ_M_num<DJ_flag_sensitivity)
					DJ_M_num++;		
			}
			else	
				DJ_M_num = 0;
			break;
		case 3://右
			if (f2_DJ_mode_i)
			{
				if(DJ_R_num<DJ_flag_sensitivity)
					DJ_R_num++;				
			}
			else	
				DJ_R_num = 0;//墙检标志 1为异常
			break;
	}	

	if(DJ_L_num==DJ_flag_sensitivity )
		DJ_L_flag=1;//地检标志 1为异常
	else
		DJ_L_flag=0;//地检标志 1为异常
	if(DJ_R_num==DJ_flag_sensitivity )
		DJ_R_flag=1;//地检标志 1为异常
	else
		DJ_R_flag=0;//地检标志 1为异常
	if(DJ_M_num==DJ_flag_sensitivity)
		DJ_M_flag=1;//地检标志 1为异常
	else
		DJ_M_flag=0;//地检标志 1为异常
	
}





void f2_ledshow(void)
{
	if(f2_kaiji<300)//开机闪3次	
	{
		f2_kaiji++;
		if(f2_kaiji%100<50)
		{
			G_led =0;//绿灯亮
			R_led =0;//红灯亮
		}
		else
		{
			G_led =1;//绿灯灭
			R_led =1;//红灯灭
		}
	}
	else//开机闪完3次
	{
		f2_LED++;//LED闪烁计数
		if(f2_mode==0&&TK_ZJ==0)//禁止模式
		{
			f2_LED=0;
	   	f2_GRled(1,1);//绿灯亮,红灯亮
			error_pengzhuang = 0;//碰撞异常
			error_xuankong = 0;//悬空异常
			error_bianshua = 0;//边刷异常
			error_fengji = 0;//计数，风机异常
			error_luanzi_L = 0;//左轮异常
			error_luanzi_R = 0;//右轮异常
			error_flag = 0;//异常标志位
		}
		if(f2_mode==1&&TK_ZJ==0)//直走1
		{	
			if(f2_LED%100<50)
				f2_GRled(1,0);//绿灯亮,红灯灭
			else
				f2_GRled(0,0);//绿灯灭,红灯灭
		}
		if(f2_mode==4&&TK_ZJ==0)//快闪
		{
			if(f2_LED%20<10)
				f2_GRled(1,0);//绿灯亮,红灯灭
			else
				f2_GRled(0,0);//绿灯灭,红灯灭	
		}
		if(f2_mode==5&&TK_ZJ==0)//慢闪
		{
			if(f2_LED%100<50)
				f2_GRled(1,0);//绿灯亮,红灯灭
			else
				f2_GRled(0,0);//绿灯灭,红灯灭	
		}	
		if(f2_mode==6)//异常
		{
			f2_GRled(0,1);//绿灯灭,红灯亮
		}
		if(f2_mode==7)//wifi按键
		{
				f2_daiji_flag1 = 0;//待机计数器
				f2_GRled(1,1);//绿灯亮,红灯亮	
		}		
		if(f2_mode==10&&TK_ZJ==0)//定点模式
		{
			if(f2_LED%100<50)
				f2_GRled(0,0);//绿灯灭,红灯灭	
			else
				f2_GRled(1,1);//绿灯亮,红灯亮
			
		}
		if(f2_mode==11)//待机模式
		{
				f2_GRled(0,0);//绿灯灭,红灯灭	
		}

		if(f2_mode==3&&TK_ZJ==0)//充电模式
		{
				error_dianliang	=0;//充电时候电量异常检测置位0
			if(f2_chongdian_flag1==100)
				f2_GRled(1,0);//绿灯亮,红灯灭
			else
			{
				if(f2_LED%100<50)
				f2_GRled(0,1);//绿灯灭,红灯亮
				else
				f2_GRled(0,0);//绿灯灭,红灯灭			
			}	
		}
		if(f2_LED==100)
			f2_LED=0;
	}
	
}

void f2_error_buzzer(void)//异常报警
{
		/**********异常*******************************/	
		if(f2_mode==6)//异常
		{

			if(error_flag)
			{
				Brush=0;
				fan=0;
				fan_open = 0;//风机开启1,关闭0
				if(error_flag==1)//碰撞卡住，5秒块响1声
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==2)//悬空，5秒块响2声
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==3)//边刷，5秒块响3声
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器	
				}					
					if(f2_buzzer_num==100)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==4)//风机，5秒块响3声，隔1秒快响2
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器	
				}					
					if(f2_buzzer_num==100)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
				if(f2_buzzer_num==200)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
				if(f2_buzzer_num==250)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==5||error_flag==6)//轮子，5秒块响2声，隔1秒快响1
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器	
				}					
				if(f2_buzzer_num==150)
				{
					BUZZER_num=30;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==7)//电量低，隔1秒快响1
				{
					if(f2_buzzer_num%100==0)
				{
					BUZZER_num=10;//蜂鸣器鸣叫时间
					BUZZER_flag=1;//开启蜂鸣器						
				}
					f2_buzzer_num++;		
				}
			}			
				if(f2_buzzer_num==500)
					f2_buzzer_num=0;
		}


}

/*************LED使能*******************/
void f2_GRled(int G,int R)//LED
{
	if(G==1)
		G_led =0;//绿灯亮
	else
		G_led =1;//绿灯灭
	if(R==1)
		R_led =0;//红灯亮
	else
		R_led =1;//红灯灭
}



/*************蜂鸣器*******************/
void f2_BUZZER(void)//蜂鸣器
{

	if(BUZZER_flag)//蜂鸣器
	{
		BUZZER_num--;
		if(BUZZER_num)//蜂鸣器时间
			buzzer=1;//开启蜂鸣器
		else
		{
			buzzer=0;//关闭蜂鸣器
			BUZZER_flag=0;//蜂鸣器已经关闭，标志位重置为0	
		}
	}
}
//读取adc
void f2_Get_ADC(INT16U adc_flag)
{
	if(guc_AdcFlag==1)              //ADC转换结束
		{ 
			ADC_0 = SCR0;//右轮SCR0
			ADC_1 = SCR1;//左轮SCR1
			ADC_2 = SCR2;//风机SCR2
			ADC_3 = SCR3;//边刷SCR3
			ADC_4 = SCR4;//电池SCR4
			ADC_5 = SCR5;//左地检SCR4
			ADC_6 = SCR6;//中地检SCR4
			ADC_7 = SCR7;//右地检SCR4

			if(adc_flag==1)
			{
				ADCC0 &= ~0x01;
				ADCC0 |= 0x01;						//打开ADC转换电源,内部4V参考电压
			}
				
			if(adc_flag==0)
			{
				ADCC0 &= ~0x01;
				ADCC0 |= 0x00;
			}
			guc_AdcFlag = 0;            //标志位清0	
			IE1 |= 0x40;                //打开ADC中断
			ADCC0 &= ~0x20; 
			ADCC0 |= 0x40;	

		}
}


void bubbleSort(int *src, int n)
{
	int i = 0, j = 0, tmp = 0;
	for(i = n - 1; i > 0; --i){
		for(j = 0; j < i; ++j){
			if(src[j] > src[j + 1]){
				tmp = src[j];
				src[j] = src[j + 1];
				src[j + 1] = tmp;     //数据交换
			}
		}
	}
}


void ADC_Rpt() interrupt ADC_VECTOR//电流转换
{
		guc_AdcFlag = 1;                    //ADC转换结束
		IE1 &=~ 0x40;                       //关闭ADC中断
		ADCC0 &=~ 0x20;						//清除ADC中断标志位ADCC0 &=~ 0x20;	

    
} 


