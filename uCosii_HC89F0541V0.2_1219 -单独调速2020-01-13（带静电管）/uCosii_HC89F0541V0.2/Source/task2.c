#include "includes.h"
#include "Source\task2.h"
/**********************************************
*ָʾ��
*����&�Լ�
*
************************************/
unsigned char guc_Uartbuf_a1[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//���ڴ�Ž����վ�
unsigned char guc_AdcFlag  = 0;         //ADCת����ɱ�־λ


unsigned int f2_kaiji = 0;//���ڿ�������3�μ��� 
unsigned int f2_ap = 0;//��������ģʽ 3�����5������



INT8U f2_LED = 0;//LED��˸����
INT8U	BUZZER_flag = 1;//������
INT8U	BUZZER_num = 30;//��������������������Ӧʱ��

INT8U f2_mode = 1;//ģʽ0ֹͣ 1���� 2�س� 3��� 4���� 5���� 6�쳣 7���򰴼� 10���� 11���� 12������� 13�ӱ�
INT8U TK_ZJ_num=0;//�����Լ죬����3�Ͳ�����
int TK_ZJ_num1=0;//�����Լ죬����5s�Ͳ�����
INT8U TK_ZJ=0;//�Լ�ģʽ
INT8U TK_ZJ1=0;//�Լ�ģʽ,��ֹ�ɿ�������
INT8U ZJ_num=0;//�Լ�ģʽ�¼���
INT8U ZJ_num1=0;//�Լ�ģʽ����Ŀ����
INT8U ZJ_num2=0;//�Լ�ģʽ����Ŀ����


INT8U TK_L_num=0;//�����������
INT8U TK_R_num=0;//�����ҷ�����


INT8U ZJ_wifi_flag=0;//�Լ�ģʽ�²���wifi
INT8U TK_ZJ_L=0;//�Լ�ģʽ,�󰴼�
INT8U TK_ZJ_R=0;//�Լ�ģʽ,�Ұ���

/**************���********************/
INT8U f2_chongdian_flag = 0;//�����������
INT32U f2_chongdian_jishuqi=0;//3Сʱ�Զ���
INT32U f2_chongdian_jishuqi2=0;//���عرռ�ʱ��,��ֹ��ѹ����
int f2_chongdian_flag1 = 0;//��������
INT8U bat_mode = 0;//���ģʽ  1���ٳ�� 2������  3������
INT8U bat_i =0;//��ֹ����
INT32U bat_i1 =0;//�����ֹ����
/*******************����*****************/
INT32U f2_daiji_flag1 = 0;//����״̬

/**********�쳣*********/
int f2_buzzer_num = 0;//������
INT8U error_pengzhuang = 0;//��������ײ�쳣
INT8U error_xuankong = 0;//�����������쳣
INT8U error_bianshua = 0;//��������ˢ�쳣
INT8U error_fengji = 0;//����������쳣
INT8U error_luanzi_L = 0;//����������쳣
INT8U error_luanzi_R = 0;//����������쳣
INT8U error_dianliang = 0;//������������
INT8U error_flag = 0;//��־,1��ײ��2���գ�3��ˢ��4�����5���֣�6���֣�7����
int f2_dianliang =0;//���� 0-100
INT8U f2_dianliang_wifi = 80;//wifi���͵���
INT8U f2_dianliang_num  = 0;//wifi���͵���������
int f2_dianliang_sum[10]={0,0,0,0,0,0,0,0,0,0};//wifi���͵������
INT8U error_dj2=0;//����ؼ�������ͣ�ӳ�
/************wifi****************/
int f2_wifi_num = 0;//1000��һ��״̬



/****************ADC*******************/
INT16U ADC_0,ADC_1, ADC_2,ADC_3,ADC_4, ADC_5,ADC_6,ADC_7;
INT16U DJ_L_ADC,DJ_M_ADC,DJ_R_ADC;	
INT16U DJ_L_flag,DJ_M_flag,DJ_R_flag;//1Ϊ�쳣0λ����	
INT8U DJ_L_num,DJ_M_num,DJ_R_num;//ADC����
bool sensor_error = 0;//������״̬���ؼ�ǽ����ײ
bool sensor_error_dj = 0;//������״̬���ؼ�

INT8U ADC_ait = 0;

//f1
extern INT8U f1_wifi_num ;//����ָ�� 1���� 2���� 3����wifi
//���
//���
extern INT8U f1_fj_xili;//������� 0ǿ 1�� 2��
extern INT8U f1_fj_xili2 ;//���ת��

//f3
extern int f3_flag;//�������У���100�ಽ��
extern bool f3_flag0;//����������־��һ�����ڰ�������ִ�е�һ�β���
extern INT8U f3_flag1;//��¼��־��һ�����ڵ�һ�θ�λ
extern INT8U num_L1 ;			//��������ֵ
extern INT8U  num_R1;				//��������ֵ
extern bool fan_open ;//�������1,�ر�0

void Task2(void *ppdata) reentrant
{
	ppdata = ppdata;
	OSTimeDly(100);

	while(1){	
/***********************���Ź�*******************************/
	WDTC |= 0x10;                   //�幷
/*************************************************************/		
	f2_wifi_num++;//1000��һ��״̬	
/***********************LED��״̬��ʾ***************************/		
	f2_ledshow();
/***********************������**********************************/				
	f2_BUZZER();		
/***********************�������********************************/			
	f2_touch();
/***********************�����л�*******************************/	
	f2_standby();//�����л�	
/***********************�Լ�ģʽ(�����Լ�)**********************/		
	f2_ZJ_MODE();//�Լ�ģʽ
//	OSTimeDly(30);

	if(f2_mode!=11)
	{
		f2_Get_ADC(0);//��ȡADC,�´βɼ�Ϊvdd
		OSTimeDly(1);
		DJ_L_ADC=ADC_5;//��ؼ죬��ʱ�ؼ����򿪣�����ֵӦ�ȹرպ����
		DJ_M_ADC=ADC_6;//�еؼ죬��ʱ�ؼ����򿪣�����ֵӦ�ȹرպ����
		DJ_R_ADC=ADC_7;//�ҵؼ죬��ʱ�ؼ����򿪣�����ֵӦ�ȹرպ����
	}

if(ADC_ait==0)
	{
/***********************�쳣���********************************/	
	f2_error();
/***********************�쳣����********************************/		
	f2_error_buzzer();		
/***********************�����********************************/		
	f2_power();
/************************��س��&����&����쳣����*************/	
	f2_bat();//��س��&���� 			
}	
	
	
	floor = 0;//�ؼ�ʹ�ܹر�
	OSTimeDly(2);

	if(f2_mode!=11)
	{
		if(ADC_ait)
		{
			f2_Get_ADC(ADC_ait);//��ȡADC,�´βɼ�Ϊ4V,����ΪVDD,Ϊ�ؼ�ADC
			OSTimeDly(2);
			ADC_ait=0;
		}
		else
		{
			f2_Get_ADC(ADC_ait);//��ȡADC,�´βɼ�ΪVDD,����Ϊ4V,Ϊ����ADC
			OSTimeDly(2);
			ADC_ait=1;		
		}		
	}
	
	


	if(f2_mode!=3&&f2_mode!=11&&ADC_ait==0)
	{
		/*************�ؼ���************************/		
			f2_DJ_mode(ADC_5,DJ_L_ADC,1);//��ؼ�
			f2_DJ_mode(ADC_6,DJ_M_ADC,2);//��ؼ�
			f2_DJ_mode(ADC_7,DJ_R_ADC,3);//��ؼ�	
	
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = ADC_5/100;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ		
//		
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = ADC_5%100;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ				
//		
//		
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = DJ_L_ADC/100;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ		
//		
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = DJ_L_ADC%100;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ			
//		
//		
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = 170;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ			

	}


	
	if(f2_mode!=3&&f2_mode!=11)//���ڳ��ʹ���
		floor = 1;//�ؼ�ʹ�ܴ�
	
	

	
	
	OSTimeDly(1);
	}}

	
void f2_standby(void)//�����л�	
{
/**********�����л�**************************/
if((f2_mode==4||f2_mode==5)&&f2_daiji_flag1<standby_wifi_time)	//�������л�
{
		if(f2_dianliang<=10)//����С��10
			f2_daiji_flag1=f2_daiji_flag1+10;
		else
			f2_daiji_flag1++;
}
if((f2_mode==4||f2_mode==5)&&f2_daiji_flag1>=standby_wifi_time)	//�������л�
{
	f2_mode=0;
	f3_flag=-1;
	f3_flag1=0;
	f2_daiji_flag1 = 0;
}		
if(f2_mode==6&&f2_daiji_flag1<standby_error_time)	//�쳣�л�
{
		if(f2_dianliang<13&&f2_dianliang>5)//����С��10
			f2_daiji_flag1++;
		else if(f2_dianliang<=5)
			f2_daiji_flag1=f2_daiji_flag1+10;
		else
			f2_daiji_flag1++;
}		
if(f2_mode==6&&f2_daiji_flag1>=standby_error_time)//�쳣�л�
{
	if(error_flag==7)//�����쳣
	{
		f2_mode=11;//����ģʽ
		Wifi =0;//�ر�wifi
	}
	else
		f2_mode=0;//����ģʽ
	f2_daiji_flag1=0;
}		
if(f2_mode==0&&f2_daiji_flag1<standby_sleep_time&&TK_ZJ!=1)//ֹͣ������
	f2_daiji_flag1++;		
if(f2_mode==0&&f2_daiji_flag1>=standby_sleep_time&&TK_ZJ!=1)//ֹͣ������
{
	Wifi=0;	//�ر�wifi
	f2_mode=11;
	f2_daiji_flag1=0;
}			
if(f2_mode==11)//�ر�wifi
{
	if(f2_daiji_flag1<10)
		f2_daiji_flag1++;
	if(f2_daiji_flag1==10)
	{
		f2_daiji_flag1++;
		Wifi=0;//�ر�wifi
		mapan = 0;//5V����ʹ�ܹر�
		/***********************************�������ģʽ***************************************/
		PCON |= 0x01;						//�������ģʽ
	}
	
}	

}

	
void f2_bat(void)//��س��&����&����쳣����
{
	if(f2_mode!=11)
	{
		if(f2_mode!=3)//�ǳ��ģʽ
		{
			if(ADC_4>=2830)//100%����
				f2_dianliang=100;
			else//5.4
				f2_dianliang=100-(2820-ADC_4)/5.88;
			if( (f2_mode==1||f2_mode==10||f2_mode==7)&&TK_ZJ==0)//����ģʽ��
			{
				if(f1_fj_xili==0)//������� 0ǿ 1�� 2��
					f2_dianliang+=30;
				else if(f1_fj_xili==1)
					f2_dianliang+=20;
				else
					f2_dianliang+=10;
			
			}
				
		}
		else//���ģʽ
		{
			if(bat_mode!=2)
			{
				if(ADC_4>=2820)//100%����
					f2_dianliang=100;
				else
					f2_dianliang=(100-(2820-ADC_4)/5.5);
			}

		
		}
		if(f2_dianliang>100)
			f2_dianliang=100;
		if(f2_dianliang<5)
			f2_dianliang=5;

		//���20��,Ȼ��wifi����
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

		
	/**********�͵������***********************/		
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
		if(error_dianliang==1000)//�쳣����
		{
			error_dianliang++;
			error_flag = 7;
			f2_mode = 6;//�쳣		
			f3_flag=-1;//����-1
			f3_flag1=0;
		}	
	/**********���***********************/				
		if(f2_mode==3)
		{

			error_dianliang=0;
			
			if(f2_chongdian_jishuqi<1080000)//3Сʱ�Զ���
				f2_chongdian_jishuqi++;
			else
				f2_chongdian_flag1=100;//���̵�
			//bat_mode = 1;//���ģʽ  1���ٳ�� 2������  3������
			if(bat_mode==1)//���ģʽ  1���ٳ��
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
			if(bat_mode==2)//���ģʽ  2������
			{
				f2_chongdian_flag1=100;//���̵�	
			}	
			

			

			
		}
		

	}

}




void f2_power(void)//�����	
{
//�����
	if(power==0&&f2_mode!=3&&TK_ZJ==0)
	{
		if(f2_chongdian_flag<3)
		{
			bat_mode = 0;//���ģʽ  1���ٳ�� 2������  3������
			f2_chongdian_flag++;//�����������
			f2_chongdian_flag1=0;//��������
			f2_chongdian_jishuqi=0;//3Сʱ�Զ�����1000000
		}
		if(f2_chongdian_flag==3)
		{
			bat_i =0;//��ֹ����
			bat_mode = 1;//���ģʽ  1���ٳ�� 2������  3������
			fan = 0;//�رշ��
			fan_open = 0;//�������1,�ر�0
			Brush = 0;//�رձ�ˢ
			floor = 0;//�ؼ�ʹ�ܹر�
			f2_wifi_num=980;//����ָ��
			BUZZER_num=30;//����������ʱ��
			BUZZER_flag=1;//����������	
			f3_flag=-1;
			f2_kaiji = 300;//��������3��
			f3_flag1 = 0;
			f2_mode = 3;
		}

	}
//�ε����
	if(power==1&&f2_mode==3&&TK_ZJ==0)	
	{
		if(f2_chongdian_flag==10)//�����������
		{
			f2_chongdian_flag--;
			BUZZER_num=30;//����������ʱ��
			BUZZER_flag=1;//����������
			f2_kaiji = 300;
		}
		if(f2_chongdian_flag<10)//�����������
		{
			f2_chongdian_flag--;
			if(f2_chongdian_flag==0)
			{
				f2_wifi_num=980;//����ָ��
				f2_kaiji = 300;//��������
				f2_mode=0;//ֹͣ
				f3_flag0 = 1;	//����������־��һ�����ڰ�������ִ�е�һ�β���
				BUZZER_num=30;//����������ʱ��
				BUZZER_flag=1;//����������
			}
		}
	}

}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
void f2_touch(void)//�������	
{
	if(L1_touch==~touch_test)//�󰴼�����ȥ
	{
		if(TK_L_num<3)//������
		{
			TK_L_num++;
		}
	}
	if(R1_touch==~touch_test)//�Ұ�������ȥ
	{
		if(TK_R_num<3)//������
		{
			TK_R_num++;
		}
		if(f2_ap<500&&TK_ZJ==0)//APģʽ 
			f2_ap++;//APģʽ  3�����5������
		if(f2_ap==300)
		{
			f2_daiji_flag1=0;//����������
			f2_mode=4;//����
			f3_flag0 = 1;
			BUZZER_flag=1;//����������
			BUZZER_num=30;//����������ʱ��
		}			
		if(f2_ap==499)
		{
			f2_daiji_flag1=0;//����������
			f2_mode=5;//����
			BUZZER_num=30;//����������ʱ��
			BUZZER_flag=1;//����������
		
		}
	
	}
	if(L1_touch==touch_test)//�󰴼�̧��
	{
		if(TK_L_num==3)
		{
			f2_daiji_flag1 = 0;//����������
			error_dianliang = 0;//�����쳣
			f2_kaiji=300;
			TK_ZJ_num++;//�Լ�ģʽ����������3���ٽ�ȥ
			BUZZER_flag=1;//����������
			BUZZER_num=30;//����������ʱ��
			if(TK_ZJ&&TK_ZJ1)//�Լ�ģʽ
			{
				ZJ_num2=0;//�Լ�ģʽ����Ŀ����
				TK_ZJ_L++;//����Ŀ
				TK_ZJ_R = 0;//С��Ŀ����		
			}
			if(f2_mode==11)//����ģʽ����
			{
				Wifi = 1;
				mapan = 1;//5V����ʹ�ܿ���
				/***********************************�˳�����ģʽ***************************************/
					PCON &= ~0x01;						//�������ģʽ
			}	
			if(f2_mode==0&&TK_ZJ==0)
				{
					f2_mode=1;//����
					f3_flag0 = 1;
				}				
				else
				{
					f2_mode=0;//ֹͣ
					f3_flag0 = 1;	
				}	
			f2_wifi_num=950;//����ָ��
		}
	

	
		TK_L_num=0;//������
	}
	if(R1_touch==touch_test)//�Ұ���̧��
	{
		if(f2_ap<300)//û�н��������������
		{
			if(TK_R_num==3)
			{
				f2_daiji_flag1 = 0;//����������
				error_dianliang = 0;//�����쳣
				f2_kaiji=300;
				TK_ZJ_num++;//�Լ�ģʽ����������3���ٽ�ȥ
				BUZZER_flag=1;//����������
				BUZZER_num=30;//����������ʱ��
				if(TK_ZJ&&TK_ZJ1)//�Լ�ģʽ
				{
					ZJ_num2=0;//�Լ�ģʽ����Ŀ����
					TK_ZJ_R++;//����Ŀ
				}
				if(f2_mode==11)//����ģʽ����
				{
					Wifi = 1;
					mapan = 1;//5V����ʹ�ܿ���
					/***********************************�˳�����ģʽ***************************************/
					PCON &= ~0x01;						//�������ģʽ
				}	
				if(f2_mode==0&&TK_ZJ==0)
				{
					f2_mode=10;//�س�
					f3_flag0 = 1;
				}	
				else
				{
					f2_mode=0;//ֹͣ
					f3_flag0 = 1;	
				}
				f2_wifi_num=950;//����ָ��
			}
		}
		else if(f2_ap<499)
		{	
			f1_wifi_num=1;//����
		}		
		else
		{
			f1_wifi_num=2;//����
		}
		f2_ap=0;//�Ұ���������			
		TK_R_num=0;
	}
	if(TK_L_num==3&&TK_R_num==3&&TK_ZJ_num<3&&TK_ZJ==0&&f2_mode!=3)//�Լ�
	{
		f3_flag =-1;
		f3_flag1=0;
		TK_ZJ =1;		
		BUZZER_flag=1;//����������
		f2_kaiji = 300;//������3�Σ�����Ϊ����
	}
//	if(TK_ZJ_num1<200)//2s���ڽ����Լ�
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
	if(DJ_L_flag+DJ_M_flag+DJ_R_flag>0)//�ؼ��־
	sensor_error_dj=1;
	else
	sensor_error_dj=0;
	if(f2_mode==1||f2_mode==10)//����  ��ת
	{
		/**********��ײ***********************/
		if(crash==0)
		error_pengzhuang=0;
	else
	{
		if(error_pengzhuang<200)
			error_pengzhuang++;		
	}
	if(error_pengzhuang==200)
	{
		f2_wifi_num=1001;//����ָ��
		error_pengzhuang++;
		error_flag = 1;
		f2_mode = 6;//�쳣
				
		f3_flag=-1;//����-1
		f3_flag1=0;
	}
	/**********�ؼ�***********************/
	if((DJ_L_flag+DJ_M_flag+DJ_R_flag)<3&&(DJ_L_flag+DJ_M_flag+DJ_R_flag)>0)
	{
		error_dj2=0;
		if(error_xuankong<150)
			error_xuankong++;			
	}
	else if(DJ_L_flag+DJ_M_flag+DJ_R_flag==3)//ɨ�ػ�����
	{
		error_dj2++;
		if(error_dj2>=50)
		{
			f3_flag=-1;//����-1
			f3_flag1=0;		
			f2_mode = 0;
			BUZZER_num=30;//����������ʱ��
			BUZZER_flag=1;//����������
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
		f2_wifi_num=1001;//����ָ��
		error_xuankong++;
		error_flag = 2;
		f2_mode = 6;//�쳣		
		f3_flag=-1;//����-1
		f3_flag1=0;
	}
	/**********��ˢ***********************/
	if(ADC_3>100)//ST
	{
		if(error_bianshua<100)
			error_bianshua++;	
	}
	else
		error_bianshua=0;
	if(error_bianshua==100)
	{
		f2_wifi_num=1001;//����ָ��
		error_bianshua++;
		error_flag = 3;
		f2_mode = 6;//�쳣		
		f3_flag=-1;//����-1
		f3_flag1=0;
	}
	/**********���***********************/
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
			f2_wifi_num=1001;//����ָ��
			error_fengji++;
			error_flag = 4;
			f2_mode = 6;//�쳣		
			f3_flag=-1;//����-1
			f3_flag1=0;
		}		
		if(f2_mode==7)
		{
		error_fengji=0;
		
		}
	
	}
	
	

	
	
	/**********����***********************/
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
		f2_wifi_num=1001;//����ָ��
		error_luanzi_L++;
		error_flag = 5;
		f2_mode = 6;//�쳣		
		f3_flag=-1;//����-1
		f3_flag1=0;
	}
	/**********����***********************/
	if(ADC_0>80)//ST
	{
		if(error_luanzi_R<50)
			error_luanzi_R++;	
	}
	else
		error_luanzi_R=0;
	if(error_luanzi_R==30)
	{
		f2_wifi_num=1001;//����ָ��
		error_luanzi_R++;
		error_flag = 6;
		f2_mode = 6;//�쳣		
		f3_flag=-1;//����-1
		f3_flag1=0;
	}	
	}
	
}
	
	
	
	
	
	
	
	
	

void f2_ZJ_MODE(void)//�Լ�ģʽ	
{
//�Լ�ģ��	
	if(TK_ZJ)
	{
		if(TK_ZJ1==0 &&L1_touch==touch_test&&R1_touch==touch_test)
		{
			TK_ZJ1=1;
			f3_flag =-1;
			f3_flag1=0;
			Brush = 0;//��ˢ�ر�
			fan = 0;//����ر�
			fan_open = 0;//�������1,�ر�0
			BUZZER_num=100;//����������ʱ��
			ZJ_num= 0;//�Լ�ģʽ�¼���
		}
		if(TK_ZJ_L%6==1)//�Լ��ģ��1.�ؼ�
		{
			f3_flag =-1;
			f3_flag1=0;
			Brush = 0;//��ˢ�ر�
			fan = 0;//����ر�
			fan_open = 0;//�������1,�ر�0
			if(TK_ZJ_R%3==0)//�Լ���Ŀ1.1 ��ؼ�
			{
				if(DJ_L_flag)//���
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%3==1)//�Լ���Ŀ1.1 ��ؼ�
			{
				if(DJ_M_flag)//���
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			if(TK_ZJ_R%3==2)//�Լ���Ŀ1.1 ��ؼ�
			{
				if(DJ_R_flag)//���
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
		}
		if(TK_ZJ_L%6==2)//�Լ��ģ��2.��ײ
		{	
			if(TK_ZJ_R%2==0)//�Լ���Ŀ2.1 ��ײ
			{
				ZJ_wifi_flag = 0;
				ZJ_num1=0;//�Լ�ģʽ�¼���
				ZJ_num2 = 0;//�Լ�ģʽ�¼���
				if(crash)
				f2_GRled(0,1);
				else
				f2_GRled(1,0);
			}
			else //wifi����
			{
			 if(ZJ_num1==0)
			 {
				ZJ_num1=1;
				f1_wifi_num=3;//����wifi
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
		if(TK_ZJ_L%6==3)//�Լ��ģ��3.��ѹ��������
		{
			if(TK_ZJ_R%6==0)//�Լ���Ŀ3.1 ���ֵ���
			{
				if(ZJ_num2==0)
				{
					
					ZJ_num1=0;
					f3_flag=200;//����
					f3_flag1=0;
					ZJ_num2=1;
				}
				if(ADC_1>30)//����AD 
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
			if(TK_ZJ_R%6==1)//�Լ���Ŀ3.2 ����
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=201;//����
					f3_flag1=0;
					ZJ_num2=1;
				}	
				if(ADC_0 >30)//����AD 				
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
			if(TK_ZJ_R%6==2)//�Լ���Ŀ3.3 ����
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//����
					f3_flag1=0;
					ZJ_num2=1;
//					fan=1;//�������
					fan_open = 1;//�������1,�ر�0
				}		
				if(f1_fj_xili2<900)//���
				{
					if(ADC_2 >350)	//���� 
						f2_GRled(0,1);
					else
						f2_GRled(1,0);
				}
				else
					if(ADC_2 >200)	//���� 
						f2_GRled(0,1);
					else
						f2_GRled(1,0);

				
			}
			if(TK_ZJ_R%6==3)//�Լ���Ŀ3.4 ��ˢ
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//����
					f3_flag1=0;
					ZJ_num2=1;
					Brush=1;//������ˢ
					fan=0;//�رշ��
					fan_open = 0;//�������1,�ر�0
				}		
				if(ADC_3>30)	//��ˢ 
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==4)//�Լ���Ŀ3.5 ��ص�ѹ
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//����
					f3_flag1=0;
					ZJ_num2=1;
					Brush=0;//�رձ�ˢ
					fan=0;//�رշ��
					fan_open = 0;//�������1,�ر�0
				}		
				if(ADC_4<1900)	//��ˢ 7v����,20%�ĵ�ѹ
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
			if(TK_ZJ_R%6==5)//�Լ���Ŀ3.6 ���
			{
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=-1;//����
					f3_flag1=0;
					ZJ_num2=1;
				}		
				if(power==1)	//���
					f2_GRled(0,1);
				else
					f2_GRled(1,0);
			}
		}
		if(TK_ZJ_L%6==4)//�����ٶȲ���
		{
			if(TK_ZJ_R%4==0)//�Լ���Ŀ4.1 ���ֺ��� ����
			{	
				if(ZJ_num2==0)
				{
					Brush=0;//�رձ�ˢ
					fan=0;//�رշ��
					fan_open = 0;//�������1,�ر�0
					ZJ_num1=0;
					f3_flag=202;//����
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
			if(TK_ZJ_R%4==1)//�Լ���Ŀ4.2 ���ֺ��� ����
			{	
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=203;//����
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
			if(TK_ZJ_R%4==2)//�Լ���Ŀ4.3 ������ǰ�� ��ˢ ���
			{	
				if(ZJ_num2==0)
				{
					Brush=1;//������ˢ
//					fan=1;//�������
					fan_open = 1;//�������1,�ر�0
					ZJ_num1=0;
					f3_flag=204;//����
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
			if(TK_ZJ_R%4==3)//�Լ���Ŀ5.2 �����ֺ��� ��ˢ ���
			{	
				if(ZJ_num2==0)
				{
					ZJ_num1=0;
					f3_flag=205;//����
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
		if(TK_ZJ_L%6==5||TK_ZJ_L%6==0)//ȫ����ʾ
		{
			if(ZJ_num2==0)
			{
				Brush=0;//�رձ�ˢ
				fan=0;//�رշ��
				fan_open = 0;//�������1,�ر�0
				ZJ_num1=0;
				f3_flag=-1;//����
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





void f2_DJ_mode(INT16U f2_DJ_ADC,INT16U f2_DJ_ADC_old,INT16U f2_DJ_ADC_flag )//�ؼ�
{
	bool f2_DJ_mode_i;//���μ���Ƿ����쳣
	if((f2_DJ_ADC_old-f2_DJ_ADC)<DJ_num_test&& (f2_DJ_ADC_old>f2_DJ_ADC) )
	{
		
		f2_DJ_mode_i=1;//ǽ���־ 1Ϊ�쳣
	}
	else
		f2_DJ_mode_i=0;//ǽ���־ 1Ϊ�쳣	
//	if(f2_DJ_ADC_old<=f2_DJ_ADC)//����̫���������պܸ�
//	{
//		f2_DJ_mode_i=1;//ǽ���־ 1Ϊ�쳣
//	}

		
	
	
	





		switch (f2_DJ_ADC_flag)//�ж����ĸ�ǽ����ǽ
	{
		case 1://��
			if (f2_DJ_mode_i)
			{
				if(DJ_L_num<DJ_flag_sensitivity)
					DJ_L_num++;			
			}
			else	
				DJ_L_num = 0;
			break;
		case 2://��
			if (f2_DJ_mode_i)
			{
				if(DJ_M_num<DJ_flag_sensitivity)
					DJ_M_num++;		
			}
			else	
				DJ_M_num = 0;
			break;
		case 3://��
			if (f2_DJ_mode_i)
			{
				if(DJ_R_num<DJ_flag_sensitivity)
					DJ_R_num++;				
			}
			else	
				DJ_R_num = 0;//ǽ���־ 1Ϊ�쳣
			break;
	}	

	if(DJ_L_num==DJ_flag_sensitivity )
		DJ_L_flag=1;//�ؼ��־ 1Ϊ�쳣
	else
		DJ_L_flag=0;//�ؼ��־ 1Ϊ�쳣
	if(DJ_R_num==DJ_flag_sensitivity )
		DJ_R_flag=1;//�ؼ��־ 1Ϊ�쳣
	else
		DJ_R_flag=0;//�ؼ��־ 1Ϊ�쳣
	if(DJ_M_num==DJ_flag_sensitivity)
		DJ_M_flag=1;//�ؼ��־ 1Ϊ�쳣
	else
		DJ_M_flag=0;//�ؼ��־ 1Ϊ�쳣
	
}





void f2_ledshow(void)
{
	if(f2_kaiji<300)//������3��	
	{
		f2_kaiji++;
		if(f2_kaiji%100<50)
		{
			G_led =0;//�̵���
			R_led =0;//�����
		}
		else
		{
			G_led =1;//�̵���
			R_led =1;//�����
		}
	}
	else//��������3��
	{
		f2_LED++;//LED��˸����
		if(f2_mode==0&&TK_ZJ==0)//��ֹģʽ
		{
			f2_LED=0;
	   	f2_GRled(1,1);//�̵���,�����
			error_pengzhuang = 0;//��ײ�쳣
			error_xuankong = 0;//�����쳣
			error_bianshua = 0;//��ˢ�쳣
			error_fengji = 0;//����������쳣
			error_luanzi_L = 0;//�����쳣
			error_luanzi_R = 0;//�����쳣
			error_flag = 0;//�쳣��־λ
		}
		if(f2_mode==1&&TK_ZJ==0)//ֱ��1
		{	
			if(f2_LED%100<50)
				f2_GRled(1,0);//�̵���,�����
			else
				f2_GRled(0,0);//�̵���,�����
		}
		if(f2_mode==4&&TK_ZJ==0)//����
		{
			if(f2_LED%20<10)
				f2_GRled(1,0);//�̵���,�����
			else
				f2_GRled(0,0);//�̵���,�����	
		}
		if(f2_mode==5&&TK_ZJ==0)//����
		{
			if(f2_LED%100<50)
				f2_GRled(1,0);//�̵���,�����
			else
				f2_GRled(0,0);//�̵���,�����	
		}	
		if(f2_mode==6)//�쳣
		{
			f2_GRled(0,1);//�̵���,�����
		}
		if(f2_mode==7)//wifi����
		{
				f2_daiji_flag1 = 0;//����������
				f2_GRled(1,1);//�̵���,�����	
		}		
		if(f2_mode==10&&TK_ZJ==0)//����ģʽ
		{
			if(f2_LED%100<50)
				f2_GRled(0,0);//�̵���,�����	
			else
				f2_GRled(1,1);//�̵���,�����
			
		}
		if(f2_mode==11)//����ģʽ
		{
				f2_GRled(0,0);//�̵���,�����	
		}

		if(f2_mode==3&&TK_ZJ==0)//���ģʽ
		{
				error_dianliang	=0;//���ʱ������쳣�����λ0
			if(f2_chongdian_flag1==100)
				f2_GRled(1,0);//�̵���,�����
			else
			{
				if(f2_LED%100<50)
				f2_GRled(0,1);//�̵���,�����
				else
				f2_GRled(0,0);//�̵���,�����			
			}	
		}
		if(f2_LED==100)
			f2_LED=0;
	}
	
}

void f2_error_buzzer(void)//�쳣����
{
		/**********�쳣*******************************/	
		if(f2_mode==6)//�쳣
		{

			if(error_flag)
			{
				Brush=0;
				fan=0;
				fan_open = 0;//�������1,�ر�0
				if(error_flag==1)//��ײ��ס��5�����1��
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==2)//���գ�5�����2��
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==3)//��ˢ��5�����3��
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������	
				}					
					if(f2_buzzer_num==100)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==4)//�����5�����3������1�����2
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������	
				}					
					if(f2_buzzer_num==100)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
				if(f2_buzzer_num==200)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
				if(f2_buzzer_num==250)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==5||error_flag==6)//���ӣ�5�����2������1�����1
				{
					if(f2_buzzer_num==0)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					if(f2_buzzer_num==50)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������	
				}					
				if(f2_buzzer_num==150)
				{
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
				if(error_flag==7)//�����ͣ���1�����1
				{
					if(f2_buzzer_num%100==0)
				{
					BUZZER_num=10;//����������ʱ��
					BUZZER_flag=1;//����������						
				}
					f2_buzzer_num++;		
				}
			}			
				if(f2_buzzer_num==500)
					f2_buzzer_num=0;
		}


}

/*************LEDʹ��*******************/
void f2_GRled(int G,int R)//LED
{
	if(G==1)
		G_led =0;//�̵���
	else
		G_led =1;//�̵���
	if(R==1)
		R_led =0;//�����
	else
		R_led =1;//�����
}



/*************������*******************/
void f2_BUZZER(void)//������
{

	if(BUZZER_flag)//������
	{
		BUZZER_num--;
		if(BUZZER_num)//������ʱ��
			buzzer=1;//����������
		else
		{
			buzzer=0;//�رշ�����
			BUZZER_flag=0;//�������Ѿ��رգ���־λ����Ϊ0	
		}
	}
}
//��ȡadc
void f2_Get_ADC(INT16U adc_flag)
{
	if(guc_AdcFlag==1)              //ADCת������
		{ 
			ADC_0 = SCR0;//����SCR0
			ADC_1 = SCR1;//����SCR1
			ADC_2 = SCR2;//���SCR2
			ADC_3 = SCR3;//��ˢSCR3
			ADC_4 = SCR4;//���SCR4
			ADC_5 = SCR5;//��ؼ�SCR4
			ADC_6 = SCR6;//�еؼ�SCR4
			ADC_7 = SCR7;//�ҵؼ�SCR4

			if(adc_flag==1)
			{
				ADCC0 &= ~0x01;
				ADCC0 |= 0x01;						//��ADCת����Դ,�ڲ�4V�ο���ѹ
			}
				
			if(adc_flag==0)
			{
				ADCC0 &= ~0x01;
				ADCC0 |= 0x00;
			}
			guc_AdcFlag = 0;            //��־λ��0	
			IE1 |= 0x40;                //��ADC�ж�
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
				src[j + 1] = tmp;     //���ݽ���
			}
		}
	}
}


void ADC_Rpt() interrupt ADC_VECTOR//����ת��
{
		guc_AdcFlag = 1;                    //ADCת������
		IE1 &=~ 0x40;                       //�ر�ADC�ж�
		ADCC0 &=~ 0x20;						//���ADC�жϱ�־λADCC0 &=~ 0x20;	

    
} 


