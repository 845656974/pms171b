#include "includes.h"
#include "Source\task1.h"
#include "Source\task3.h"
unsigned char guc_Uartflag = 0;			//UART�жϱ�־λ
unsigned char guc_Uartcnt = 0;		    //UART����ʹ��
unsigned char guc_Uartbuf_a[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//���ڴ�Ž����վ�
unsigned char guc_i;					//�������ݼ���	
bool wifi_first =0;//��һ�ο��� ����00  �ڶ��η���01


int wifi_num_flag = 0;//����ָ��
INT8U f1_wifi_mode=0;//���Ʒ���ʱ�������Ṥ��
//f2
extern INT8U ZJ_wifi_flag;//�Լ�ģʽ�²���wifi
extern int f2_wifi_num ;//1000��һ��״̬																	 
extern INT8U f2_mode ;//ģʽ0ֹͣ 1���� 2�س� 3��� 4���� 5���� 6�쳣 7���򰴼� 10���� 11���� 12������� 13�ӱ�
extern INT8U	BUZZER_flag ;//������
extern INT8U	BUZZER_num ;//��������������������Ӧʱ��																	 
extern int f2_chongdian_flag1 ;//��������																	 
extern INT8U f2_dianliang_wifi ;//���� 0-100					
extern INT8U error_flag ;//��־,1��ײ��2���գ�3��ˢ��4�����5���֣�6���֣�7����
extern INT8U error_fengji ;//����������쳣



//f3																	 
extern bool	f3_flag0 ;//����������־��һ�����ڰ�������ִ�е�һ�β���																	 
extern INT8U 	f3_flag1 ;//��¼��־��һ�����ڵ�һ�θ�λ

extern int f3_flag ;//�������У���100�ಽ��
extern INT32U f3_fengji ;
extern bool fan_open ;//�������1,�ر�0

//����ָ�� 1���� 2���� 3����wifi
INT8U f1_wifi_num = 0;
INT8U f1_wifi_i1 = 0;//����dp����
INT8U f1_wifi_i2 = 0;//����dp����

//���
INT8U f1_fj_xili =0;//������� 0ǿ 1�� 2��
INT32U f1_fj_xili1 =1000;//���ת��10000 8000 6000
INT8U f1_fj_xili2 =0;//���ת��

INT16U f1_fj_num = 0;//����쳣������

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
/***********************����&����ָ���*******************************/		
		uart1_data();					//����&����ָ���
/***********************��ʱ����ɨ�ػ�״̬******************************/				
		f1_timing_case();			//��ʱ����ɨ�ػ�״̬		
		
		
		}

/***********************���ת��*****************************************/			
		f1_fan_open()	;	
		
		
		

		
}	}





void f1_fan_open(void)	//�������
{
if(f1_fj_num<1000)//���������
{
	f1_fj_num++;
	error_fengji = 0;//����쳣
}
	
//if((f2_mode==1||f2_mode==10||f2_mode==13))//����ӳ�2������
if(fan_open)//����ӳ�2������
{
//f1_fj_xili =0;//������� 0ǿ 1�� 2��
switch(f1_fj_xili)
{
	case 0://ǿ
		f1_fj_xili1=1000;
		break;
	case 1://��
		f1_fj_xili1=850;
		break;
	case 2://��
		f1_fj_xili1=700;
		break;

}
	
if(f1_fj_xili1<550)
	error_fengji = 0;//����쳣
if(f3_fengji<f1_fj_xili1)//����	
{

		error_fengji = 0;//����쳣
	f3_fengji++;
}			

else
	f3_fengji=f1_fj_xili1;


if(f3_fengji<400)//�״�����
	f3_fengji=400;


if(f1_fj_xili==0)//ǿ��
{
	PWM2DH = (f3_fengji*2)/255;//���ָ�λPWM
	PWM2DL = (f3_fengji*2)%255;//���ֵ�λPWM
	
	
	
	
//	if( (f3_fengji%10) < (f3_fengji/100) )
//		fan =1;
//	else
//		fan =0;
//	if(f3_fengji>=1000)//100
//		fan =1;
}
else//���Լ���
{
	
		PWM2DH = (f3_fengji*2)/255;//���ָ�λPWM
		PWM2DL = (f3_fengji*2)%255;//���ֵ�λPWM
	
	
	
//	if(f3_fengji<f1_fj_xili1)//����
//	{
//		if( (f3_fengji%10) < (f3_fengji/100) )
//			fan =1;
//		else
//			fan =0;
//		
//	}
//	else//����
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
		PWM2DH =	0x07;//���ָ�λPWM
		PWM2DL =	0xff;//���ֵ�λPWM
	
	
	
	}
//		fan =1;
}
else
	{
		f1_fj_num = 0;//����쳣������
		PWM2DH = 0;//���ָ�λPWM
		PWM2DL = 0;//���ֵ�λPWM
		
		
//		fan = 0;
	}
		



}









void f1_timing_case(void)	//��ʱ����ɨ�ػ�״̬
{
		wifi_num_flag++;
		if(wifi_num_flag>2000)
		{
			wifi_num_flag=0;
			guc_Uartflag = 0;
			guc_Uartcnt = 0;
			SCON2 = 0x02;						//8λUART�������ʿɱ�
			SCON = 0x10;						//�����н���
			IE |= 0x10;							//ʹ�ܴ����ж�	
		
		}
}

void uart1_data(void)					//���պ���
{
if(f1_wifi_num)//����&&����&&����wifi
{
//	SCON &= ~0x10;      //�رմ��н���
	if(f1_wifi_num==1)//����
	{
		for(guc_i= 0;guc_i<8;guc_i++)
		{
			SBUF = Buff6[guc_i];//����8λ��������
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//��������жϱ�־λ
		}
	}
	else if(f1_wifi_num==2)//����
	{
		for(guc_i= 0;guc_i<8;guc_i++)
		{
			SBUF = Buff7[guc_i];//����8λ��������
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//��������жϱ�־λ
		}
	}
	else if(f1_wifi_num ==3)//����wifi
	{
		for(guc_i= 0;guc_i<7;guc_i++)
		{
			SBUF = Buff8[guc_i];//����8λ��������
			while(!(SCON & 0x02));
			SCON &=~ 0x02;			//��������жϱ�־λ
		}
	}
	SCON |= 0x10;      //�����н���
	f1_wifi_num=0;		
}	


	
if(guc_Uartflag)//������ɱ�־
{
	
	IE &=~ 0x10;				//ʧ��UART1�ж�
	if(guc_Uartbuf_a[0]==0x55)
	{
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==0)//�������
		{
			wifi_num_flag=0;
			if(wifi_first==0)
			{
				wifi_first=1;
				for(guc_i= 0;guc_i<8;guc_i++)
				{
					SBUF = Buff1[guc_i];//����8λ��������
					while(!(SCON & 0x02));
					SCON &=~ 0x02;			//��������жϱ�־λ
				}
			}				
			else
			{
				for(guc_i= 0;guc_i<8;guc_i++)
				{
					SBUF = Buff2[guc_i];//����8λ��������
					while(!(SCON & 0x02));
					SCON &=~ 0x02;			//��������жϱ�־λ
				}
			}	
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==1)//��ѯ��Ʒ��Ϣ
		{
			for(guc_i= 0;guc_i<49;guc_i++)
			{
				SBUF = Buff3[guc_i];//����8λ��������
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//��������жϱ�־λ
			}
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==2)//��ѯ MCU �趨ģ�鹤����ʽ
		{
			for(guc_i= 0;guc_i<7;guc_i++)
			{
				SBUF = Buff4[guc_i];//����8λ��������
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//��������жϱ�־λ
			}
		}
		if(guc_Uartbuf_a[2]==0&&guc_Uartbuf_a[3]==3)//���� WiFi ����״̬
		{
			for(guc_i= 0;guc_i<7;guc_i++)
			{
				SBUF = Buff5[guc_i];//����8λ��������
				while(!(SCON & 0x02));
				SCON &=~ 0x02;			//��������жϱ�־λ
			}
		}
		if(guc_Uartbuf_a[2]==0&&(guc_Uartbuf_a[3]==8||guc_Uartbuf_a[3]==3))//��ѯ mcu ����״̬
		{
			f2_wifi_num=900;//����ָ��;	//��ѯ mcu ����״̬
		}
		if(guc_Uartbuf_a[3]==0x0e&&guc_Uartbuf_a[5]==2)//�Լ�ģʽ����wifi
		{
			ZJ_wifi_flag=1;//�Լ�ģʽ�²���wifi
		}
		
		
		
		
		
	//��ɨ����&&����
		if(guc_Uartbuf_a[3]==6)
		{
			f2_wifi_num=900;//����ָ��
			if(guc_Uartbuf_a[6]==2&&guc_Uartbuf_a[10]==1)	//��ɨ����,��
			{
				f2_mode=1;
				f3_flag0 = 1;
				BUZZER_num=30;//����������ʱ��
				BUZZER_flag=1;//����������
			}
			if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==1)	//�Զ�ģʽ
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
				BUZZER_num=30;//����������ʱ��
				BUZZER_flag=1;//����������
			}
		///***********************�ص���ɨ*******************************/					
				if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==2)	//�ص���ɨ
				{
					if(f2_mode==0)
					{
						f2_mode=10;//�ص���ɨ
						f3_flag0 = 1;		
					}
					else
					{
						f2_mode=0;
						f3_flag = -1;
						f3_flag1=0;
					}
					
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������
				}	
			if(guc_Uartbuf_a[6]==3&&guc_Uartbuf_a[10]==0)	//����
				{
					f2_mode=0;
					f3_flag = -1;
					f3_flag1=0;
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������
				}
			if(guc_Uartbuf_a[6]==2&&guc_Uartbuf_a[10]==0)	//��ɨ����,�ر�
			{
				f2_mode=0;
				f3_flag = -1;
				f3_flag1=0;
				BUZZER_num=30;//����������ʱ��
				BUZZER_flag=1;//����������
			}
			if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==0)	//ǰ��
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
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==1)	//����
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
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==2)	//��
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
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==3)	//��
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
				if(guc_Uartbuf_a[6]==4&&guc_Uartbuf_a[10]==4)	//ֹͣ
				{
					if(f1_wifi_mode==1)//�˶���ʹ�ð������ɿ�ʱ������˶�
					{
						f1_fj_num=0;
						reset_pwm();
						f1_wifi_mode=0;
						f2_mode=1;
						f3_flag0 = 1;		
						Brush=1;//��ˢ
//						fan=1;//���
						fan_open = 1;//�������1,�ر�0
					}
					else//��ֹ��ʹ�ð������ɿ�ʱ��ֹͣ�˶�
					{
						f1_fj_num=0;
						Brush=0;//��ˢ
						fan=0;//���
						fan_open = 0;//�������1,�ر�0
						f2_mode=0;
						f3_flag = -1;
						f3_flag1=0;
					}
					
					
				}
				if(guc_Uartbuf_a[6]==13&&guc_Uartbuf_a[10]==1)//Ѱ�һ���
				{
					for(guc_i= 0;guc_i<12;guc_i++)
					{
						SBUF = Buff11[guc_i];//����8λ��������
						while(!(SCON & 0x02));
						SCON &=~ 0x02;			//��������жϱ�־λ
					}	
					BUZZER_num=200;//����������ʱ��
					BUZZER_flag=1;//����������
				}
				
				if(guc_Uartbuf_a[6]==14&&guc_Uartbuf_a[7]==4)//����ѡ��
				{
					Buff12[10]=guc_Uartbuf_a[10];
					f1_fj_xili =Buff12[10];//�������
					Buff12[11]=33+guc_Uartbuf_a[10];
					for(guc_i= 0;guc_i<12;guc_i++)
					{
						SBUF = Buff12[guc_i];//����8λ��������
						while(!(SCON & 0x02));
						SCON &=~ 0x02;			//��������жϱ�־λ
					}	
				
					BUZZER_num=30;//����������ʱ��
					BUZZER_flag=1;//����������
				}
		}	
	}
	
	
	
//	for(guc_i= 0;guc_i<guc_Uartcnt;guc_i++)
//	{
//		SBUF = guc_Uartbuf_a[guc_i];//����8λ��������
//		while(!(SCON & 0x02));
//		SCON &=~ 0x02;			//��������жϱ�־λ
//	}
	IE |= 0x10;					//UART1�ж�ʹ��
	guc_Uartflag = 0;		//������ɱ�־
	guc_Uartcnt = 0;		//�������ݳ���
	SCON |= 0x10;				//UART1����ʹ��
}	



	f1_wifi_i1 = 0;//5��DPָ��
		if(f2_wifi_num >1000)
		{
			f2_wifi_num=0;
			f1_wifi_i2 = 0;//DP����ָ��
			IE &=~ 0x10;				//ʧ��UART1�ж�
			for(f1_wifi_i1=0;f1_wifi_i1<5;f1_wifi_i1++)//5��DPָ��
			{
				for(f1_wifi_i2=0;f1_wifi_i2<12;f1_wifi_i2++)////DP����ָ��
				{
					if(f1_wifi_i2<10)
						Buff[f1_wifi_i2]=Buff10[f1_wifi_i1][f1_wifi_i2];
					else
					{
						if(f1_wifi_i2==10)//����ָ��
						{
							if(f1_wifi_i1==0)//��ɨ����
							{
								if(f2_mode==1||f2_mode==2||f2_mode==10)//1���Σ�2�س壬10���� 
									Buff[f1_wifi_i2] = 1;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
								{
									SBUF = Buff[guc_i];//����8λ��������
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//��������жϱ�־λ
								}
								
								
							}				
							if(f1_wifi_i1==1)//����ģʽ
							{
								if(f2_mode==1)//1����
									Buff[f1_wifi_i2] = 1;
								else if	(f2_mode==10)//10����
									Buff[f1_wifi_i2] = 2;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
								{
									SBUF = Buff[guc_i];//����8λ��������
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//��������жϱ�־λ
								}
								
							}							
							
							if(f1_wifi_i1==2)//����״̬
							{
								if(f2_mode==1)//1����
									Buff[f1_wifi_i2] = 1;
								else if	(f2_mode==10)//10����
									Buff[f1_wifi_i2] = 2;
								else if(f2_mode==3)//���
								{
									if(f2_chongdian_flag1<100)//��������
										Buff[f1_wifi_i2] = 3;
									else
										Buff[f1_wifi_i2] = 4;
								}																
								else
									Buff[f1_wifi_i2] = 0;
								
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
								{
									SBUF = Buff[guc_i];//����8λ��������
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//��������жϱ�־λ
								}
								
							}
							if(f1_wifi_i1==3)//����
							{
								
								Buff13[13]= f2_dianliang_wifi;
								Buff13[14]= f2_dianliang_wifi+29;
								f1_wifi_i2++;
								for(guc_i= 0;guc_i<15;guc_i++)//���� WiFi ����״̬
								{
									SBUF = Buff13[guc_i];//����8λ��������
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//��������жϱ�־λ
								}
								
//								Buff[f1_wifi_i2] = f2_dianliang_wifi;
//								f1_wifi_i2++;
//								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
//								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
//								{
//									SBUF = Buff[guc_i];//����8λ��������
//									while(!(SCON & 0x02));
//									SCON &=~ 0x02;			//��������жϱ�־λ
//								}
//								
//								
//								
//Buff13[15] = {0x55,0xaa,0x03,0x07,0x00,0x08,0x06,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x1D};//ʣ�����

								
								
								
//								if(f1_wifi_i2<10)
//									Buff[f1_wifi_i2]=Buff10[f1_wifi_i1][f1_wifi_i2];
//								Buff13[10] =  f2_dianliang_wifi;
//								Buff13[11] =  f2_dianliang_wifi+29;
//								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
//								{
//									SBUF = Buff13[guc_i];//����8λ��������
//									while(!(SCON & 0x02));
//									SCON &=~ 0x02;			//��������жϱ�־λ
//								}
							}
							if(f1_wifi_i1==4)//���Ͼ���
							{
								if(error_flag==1)//1��ײ
									Buff[f1_wifi_i2] = 8;
								else if	(error_flag==2)//2����
									Buff[f1_wifi_i2] = 16;
								else if	(error_flag==3)//3��ˢ
									Buff[f1_wifi_i2] = 4;
								else if	(error_flag==4)//4���
									Buff[f1_wifi_i2] = 64;
								else if	(error_flag==5)//5����
									Buff[f1_wifi_i2] = 1;
								else if	(error_flag==6)//6����
									Buff[f1_wifi_i2] = 2;
								else if	(error_flag==7)//7��ص���
									Buff[f1_wifi_i2] = 32;
								else
									Buff[f1_wifi_i2] = 0;
								f1_wifi_i2++;
								Buff[f1_wifi_i2] = Buff10[f1_wifi_i1][f1_wifi_i2]+Buff[f1_wifi_i2-1];
								for(guc_i= 0;guc_i<12;guc_i++)//���� WiFi ����״̬
								{
									SBUF = Buff[guc_i];//����8λ��������
									while(!(SCON & 0x02));
									SCON &=~ 0x02;			//��������жϱ�־λ
								}
							}
						}
					}

				}
			}
	IE |= 0x10;					//UART1�ж�ʹ��
	guc_Uartflag = 0;
	guc_Uartcnt = 0;
	SCON |= 0x10;				//UART1����ʹ��
		}	
}



/***************************************************************************************
  * @˵��  	UART1�жϷ�����
  *	@����	��
  * @����ֵ ��
  * @ע		��
***************************************************************************************/

void UART1_Rpt(void) interrupt UART1_VECTOR
{
	if(SCON & 0x01)						//�жϽ����жϱ�־λ
	{
		if((guc_Uartcnt==0&&SBUF==85&&guc_Uartcnt<12)||(guc_Uartcnt!=0&&guc_Uartbuf_a[0]==85&&guc_Uartcnt<12))
		{
			guc_Uartbuf_a[guc_Uartcnt++] = SBUF;//ת��8λ���ڽ�������
			if(guc_Uartcnt == 7&& (guc_Uartbuf_a[3]<=2 ||guc_Uartbuf_a[3]==8))
			{
					SCON &=~ 0x10;				//ʧ��UART1����			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 8&&guc_Uartbuf_a[3]==3)
			{
					SCON &=~ 0x10;				//ʧ��UART1����			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 9&&guc_Uartbuf_a[3]==14)//WiFi����
			{
					SCON &=~ 0x10;				//ʧ��UART1����			
					guc_Uartflag = 1;
			}
			if(guc_Uartcnt == 12)
			{
					SCON &=~ 0x10;				//ʧ��UART1����			
					guc_Uartflag = 1;
			}
		  SCON &=~ 0x01;					//��������жϱ�־λ	
		}
		if((guc_Uartcnt!=0&&SBUF == 85))
		{
			guc_Uartbuf_a[0] = SBUF;//ת��8λ���ڽ�������
			guc_Uartcnt	=1;
		}
		if(guc_Uartcnt==12)
			guc_Uartcnt=0;
	}							
}

















