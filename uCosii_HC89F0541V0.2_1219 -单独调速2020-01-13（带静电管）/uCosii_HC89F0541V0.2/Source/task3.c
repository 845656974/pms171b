#include "includes.h"
#include "Source\task3.h"

INT8U run_stop_flag =0;//90��ֱ��ֹͣ��־�����ж��ڴ���
INT8U run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
INT8U run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������

INT8U  num_L=0;				//��������ֵ
INT8U num_L1 = 0;			//��������ֵ
int	num_L2=0;					//��������ֵ
int sum_L=0;						//�ۼ�ƫ��
int count_L_1=0;				//����������ƫ��
int count_L_2=0;				//�������ϴ�ƫ��
int count_sum_L = 0;		//ϵͳƫ�����pid��d
int count_sum1_L = 0;		//ϵͳƫ���ǰƫ��
int count_sum2_L = 0;		//ϵͳƫ���һ��ƫ��
int count_sum3_L = 0;		//ϵͳƫ����ϴ�ƫ��
int	pwm_L=0;			//PWMֵ0-2048

INT8U  num_R=0;				//��������ֵ
INT8U  num_R1=0;				//��������ֵ
int	num_R2=0;				//��������ֵ
int sum_R=0;						//�ۼ�ƫ��
int count_R_1=0;				//����������ƫ��
int count_R_2=0;				//�������ϴ�ƫ��
int count_sum_R = 0;		//ϵͳƫ�����pid��d
int count_sum1_R = 0;		//ϵͳƫ���ǰƫ��
int count_sum2_R = 0;		//ϵͳƫ���һ��ƫ��
int count_sum3_R = 0;		//ϵͳƫ����ϴ�ƫ��
int	pwm_R=0;			//PWMֵ0-2048



int f3_crash1= 0;//���˴���
int 	f3_count = 0;//����������
bool	f3_flag0 = 1;//����������־��һ�����ڰ�������ִ�е�һ�β���
INT8U 	f3_flag1 = 0;//��¼��־��һ�����ڵ�һ�θ�λ
INT8U f3_flag3 = 0;//��ת��ϼ�����,6�������������120����ת
int		f3_flag4 = 0;//�ص���ɨģʽ������
INT8U f3_flag5 = 0;//120�����

bool fan_open = 0;//�������1,�ر�0
INT32U f3_fengji = 0;

int f3_flag = -1;//�������У���100�ಽ��

//f2
extern INT8U f2_mode ;//ģʽ0ֹͣ 1���� 2�س� 3��� 4���� 5���� 6�쳣 7���򰴼� 10���� 11����
extern INT8U TK_ZJ;//�Լ�ģʽ

extern INT8U	BUZZER_flag ;//������
extern INT8U	BUZZER_num ;//��������������������Ӧʱ��
extern bool sensor_error ;//������״̬���ؼ�ǽ����ײ



void Task3(void *ppdata) reentrant
{
	initialize_pwm();//��ʼ��PWM

	ppdata = ppdata;
		OSTimeDly(1000);
	while(1){
/***********************���Ρ����㡢��������ʼ��*******************************/		
f3_run_first();//	���Ρ����㡢��������ʼ��
/***********************ֹͣ����*******************************/	
f3_stop();			//	ֹͣ����		
	
/***********************����*******************************/			
f3_run();		
		
/***********************�Լ����Ӳ���*******************************/					
f3_run_ZJ();//	�Լ����Ӳ���			
/***********************WIFI���򰴼�*******************************/					
f3_run_WIFI();//	�Լ����Ӳ���	
	
	
		OSTimeDly(15);
		
		
		
	}  }


	
void f3_run(void)			//	����
{
//			IE &=~ 0x10;				//����UART1�ж�
//			SBUF = 0xFF;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ
//			
//			SBUF = 0xFE;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ

//			SBUF = 0x01;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ
	
//			SBUF = num_L1;//task1����8λ��������
//			while(!(SCON & 0x02));
//			SCON &=~ 0x02;			//��������жϱ�־λ

	
if(f3_flag==0)//����0 ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(10);
	}		
	f3_count++;//����������
	if(sensor_error==0)//��ײ�͵ؼ춼����
	{
			advance_L(1,20,0);
			advance_R(1,20,0);
	}
	else//��ײ
	{
		f3_flag1 = 0;//��¼��־��һ�����ڲ����һ�θ�λ
		f3_flag = 1;//���벽��1
	}
	if(f3_count >600)
	{
	f3_flag1 =0;//������
	f3_flag =2;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}	

if(f3_flag==1)//����1 ��ײ����
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(50);
	}

	back(12,20);	//����20�Σ����벽��12	
}
if(f3_flag==2)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =3;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==3)//����3����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_L(1,15,0);
		advance_R(1,15,0);
	}
	else//��ײ
	{
		f3_flag = 40;//���벽��40
		f3_flag3 =0;//��ת��ϼ�����,6�������������120����ת
		f3_flag1 =0;//������
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=4;
	}	
}
if(f3_flag==4)//����4����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =5;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==5)//����5����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
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
	f3_flag1 =0;//������
	f3_flag =7;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==6)//����6����ײ����
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(50);
	}
	
	back(17,20);	//����10�Σ����벽��17	
}
if(f3_flag==7)//����7����ײ���˽�����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
//	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);	
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =8;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==8)//����8����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_R(1,15,0);
		advance_L(1,15,0);
	}
	else//��ײ
	{
		f3_flag = 20;//���벽��40
		f3_flag3 =0;//��ת��ϼ�����
		f3_flag1 =0;//������
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=9;
	}	
}
if(f3_flag==9)//����9����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =0;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}

if(f3_flag==12)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =2;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =3;//��������1���ˣ�ֱ�Ӿ�����ת
	}	
}

if(f3_flag==17)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =2;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(0,15,0);
		advance_R(1,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =8;//��������1���ˣ�ֱ�Ӿ�����ת
	}
	
}	









if(f3_flag==20)//����20�������ײ�����
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count=15;
		OSTimeDly(50);
	}
	f3_count--;//���˼�����
	advance_L(0,15,0);
	advance_R(0,15,0);
	if(sensor_error)//��ײ
	{
		f3_count=15;	
	}
	if(f3_count==0)//������ϣ�ִ��21
	{
		f3_flag1=0;
		f3_flag=21;
	}	
}	
if(f3_flag==21)//����21���ұ�����
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count=0;
		OSTimeDly(50);
	}
	f3_count++;		//��ת�л�������,�л�����ת���һ����ҵ���
	if(f3_count<50)
	{
		advance_L(1,2,1);	
		advance_R(1,20,1);
	}
	else
	{
		f3_flag =22;
		f3_flag1= 0;//���ڽ�������	
	}
	if(sensor_error)//��ײ
	{
		f3_flag = 22;//���벽��40
		f3_flag1 =0;//������
	}
}	
if(f3_flag==22)//����42���ұ�����
{
	if(f3_flag1==0)//����PWM
	{
		f3_flag3++;//��ת��ϼ�����,6�������������120����ת
		f3_count=0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
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
		f3_flag1= 0;//���ڽ�������
	}
	if(sensor_error)//��ײ
	{
		f3_flag = 20;//���벽��20
		f3_flag1 =0;//������
	}
	if(f3_flag3>=6)//������
	{
		f3_flag3 = 0;
		f3_flag  =24;//����24
		f3_flag1 = 0;
		f3_flag5 = 0;//120�����
	}
}

if(f3_flag==23)//����43��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);	
	}
	else
	{
		f3_flag = 20;//���벽��40������
		f3_flag1 =0;//������
	}
	if(f3_count>200)
	{
		f3_flag = 0;//�ص�50
		f3_flag1 = 0;//������
	}	
}
if(f3_flag==24)//������׼����,120��
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//����������
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
		f3_flag1 = 0;//������
	}			
}
if(f3_flag==25)///120��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		f3_flag5++;//120�����
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//����������
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
		f3_flag5 = 0;//120�����
		f3_flag = 0;
		f3_flag1=0;
	
	}
}
if(f3_flag==26)//����46����ײ����
	{
		if(f3_flag1==0)//����PWM
		{
			reset_pwm();//����PWM
			f3_flag1= 1;//���ڽ�������
			f3_count = 0;
			OSTimeDly(50);
		}
		back(24,20);//����20������44
	}









if(f3_flag==40)//����40�������ײ�����
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count=15;
		OSTimeDly(50);
	}
	f3_count--;//���˼�����
	advance_L(0,15,0);
	advance_R(0,15,0);
	if(sensor_error)//��ײ
	{
		f3_count=15;	
	}
	if(f3_count==0)//������ϣ�ִ��41
	{
		f3_flag1=0;
		f3_flag=41;
	}	
}
if(f3_flag==41)//����41��������������ִ��ǰ��
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count=0;
		OSTimeDly(50);
	}
	f3_count++;		//��ת�л�������,�л�����ת���һ����ҵ���
	if(f3_count<50)
	{
		advance_L(1,20,1);
		advance_R(1,2,1);	
	}
	else
	{
		f3_flag =42;
		f3_flag1= 0;//���ڽ�������
	}	
	if(sensor_error)//��ײ
	{
		f3_flag = 40;//���벽��40
		f3_flag1 =0;//������
	}
}



if(f3_flag==42)//����42���������
{
	if(f3_flag1==0)//����PWM
	{
		f3_flag3++;//��ת��ϼ�����,6�������������120����ת
		f3_count=0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
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
		f3_flag1= 0;//���ڽ�������
	}	
	if(sensor_error)//��ײ
	{
		f3_flag = 40;//���벽��40
		f3_flag1 =0;//������
	}
		if(f3_flag3>=6)//������
	{
		f3_flag3 = 0;
		f3_flag  =44;//����44
		f3_flag1 = 0;
		f3_flag5 = 0;//120�����
	}

}	

if(f3_flag==43)//����43��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);	
	}
	else
	{
		f3_flag = 40;//���벽��40������
		f3_flag1 =0;//������
	}
	if(f3_count>200)
	{
		f3_flag = 50;//�ص�50
		f3_flag1 = 0;//������
	}	
}


if(f3_flag==44)//������׼����,120��
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
		f3_count++;//����������
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
		f3_flag1 = 0;//������
	}			
}

if(f3_flag==45)///120��ִ����ϣ�ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		f3_flag5++;//120�����
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	f3_count++;//����������
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
		f3_flag5 = 0;//120�����
		f3_flag = 50;
		f3_flag1=0;
	
	}
}
if(f3_flag==46)//����46����ײ����
{
	if(f3_flag1==0)//����PWM
	{
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		f3_count = 0;
		OSTimeDly(50);
	}
	back(44,20);//����20������44
}


	
	
	
if(f3_flag==50)//����50 ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(50);
	}		
	f3_count++;//����������
	if(sensor_error==0)
	{

			advance_R(1,20,0);
			advance_L(1,20,0);
	}
	else//��ײ
	{
		f3_flag1 = 0;//��¼��־��һ�����ڲ����һ�θ�λ
		f3_flag = 51;//���벽��51
	}
	if(f3_count >600)
	{
	f3_flag1 =0;//������
	f3_flag =52;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
	
	
if(f3_flag==51)//����51 ��ײ����
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(50);
	}
	
	back(62,20);	//����20�Σ����벽��2	
}
	

if(f3_flag==52)//����52����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =53;//��������1���ˣ�ֱ�Ӿ�����ת
	}
	
}
if(f3_flag==53)//����3����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_R(1,15,0);
		advance_L(1,15,0);
	}
	else//��ײ
	{
		f3_flag = 20;//���벽��20
		f3_flag3 =0;//��ת��ϼ�����
		f3_flag1 =0;//������
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=54;
	}	
}
if(f3_flag==54)//����54����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(0,15,0);
		advance_R(1,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =55;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==55)//����5����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
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
	f3_flag1 =0;//������
	f3_flag =67;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==56)//����6����ײ����
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;//�����������������ߵĳ���
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
		OSTimeDly(50);
	}
	
	back(67,20);	//����10�Σ����벽��57
}
if(f3_flag==57)//����7����ײ���˽�����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =58;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
if(f3_flag==58)//����8����ת��ֱ��
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count++;//����������
	if(sensor_error==0)
	{
		advance_L(1,15,0);
		advance_R(1,15,0);
	}
	else//��ײ
	{
		f3_flag = 40;//���벽��40
		f3_flag3 =0;//��ת��ϼ�����
		f3_flag1 =0;//������
	}
	if(f3_count>80)
	{
		f3_flag1=0;
		f3_flag=59;
	}	
}
if(f3_flag==59)//����9����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =1;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =50;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
		
	
	

if(f3_flag==62)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =2;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_R(1,15,0);
		advance_L(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =53;//��������1���ˣ�ֱ�Ӿ�����ת
	}
}
	
if(f3_flag==67)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		run_stop_flag = 0;//90��ֱ��ֹͣ��־�����ж��ڴ���
		run_stop_flag_R =2;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
		OSTimeDly(50);
	}
	f3_count ++;//����������
	if(sensor_error==0&&run_stop_flag ==0)//��ײ�͵ؼ춼����,���ж��ڵ�90�������û�е�1
	{
		advance_L(1,15,0);
		advance_R(0,15,0);
	}
	if(sensor_error || run_stop_flag ||f3_count>100)//��ײ�͵ؼ��쳣||90�����||���������75
	{
	f3_flag1 =0;//������
	f3_flag =58;//��������1���ˣ�ֱ�Ӿ�����ת
	}
	
}	



/****************************************************************	
*
*�ص���ɨ
*
*
*****************************************************************/			
if(f3_flag==70)//����2����ת
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	f3_count ++;//����������
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag1 =0;//������
		f3_flag =71;//��������1���ˣ�ֱ�ӽ�����ת	
	
	}
	if(f3_count >100)
	{
	f3_flag1 =0;//������
	f3_flag =72;//��������1���ˣ�ֱ�ӽ�����ת
	}

}

if(f3_flag==71)//��Ȧ��ת��ײ����
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 20;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	f3_count --;//����������
	if(sensor_error)//������ײ
	{
		f3_count=10;
	}
	if(f3_count >0)//����
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	if(f3_count==0)//��ײ����ͺ���ָ��ִ����ϣ����벽��70
	{
	f3_flag1 =0;//������
	f3_flag =70;//��������1���ˣ�ֱ�ӽ�����ת	
	}	
}
if(f3_flag==72)//����ת�ص�ɨ��
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag4=0;//��תģʽ������
		f3_flag1= 1;//���ڽ�������
	}
	
	if(sensor_error)//��ײ
	{
		f3_flag =73;	
	}
	else
	{
		if(f3_flag4>=0&&f3_flag4<1000)//[0,5)
		{
//			if(f3_flag4==1)
//			{
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
//				}
			advance_L(1,f3_flag4/200,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=1000&&f3_flag4<2000)//[5,8]
		{
//			if(f3_flag4==1001)
//			{
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
//				}
			advance_L(1,2+f3_flag4/300,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=2000&&f3_flag4<3500)//[8,11]
		{
//			if(f3_flag4==2001)
//			{
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
//				}
			advance_L(1,3+f3_flag4/400,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=3500&&f3_flag4<7500)//[11,14]
		{
//			if(f3_flag4==3501)
//			{
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
//				}
			advance_L(1,9+f3_flag4/1500,1);
			advance_R(1,20,1);		
		}
		if(f3_flag4>=7500&&f3_flag4<10000)//[14,16]
		{
//			if(f3_flag4==7501)
//			{
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
//				}
			advance_L(1,11+f3_flag4/2000,1);
			advance_R(1,20,1);		
		}
		
		if(f3_flag4>=10000)
		{
			
//				BUZZER_flag=1;//����������
//				BUZZER_num=30;//����������ʱ��
				
			f3_flag1 = 0;
			f3_flag = 74;
		}
	}
	f3_flag4++;
}
if(f3_flag==73)//����102����ת��ײ����
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 10;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
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
if(f3_flag==74)//����ת����ת180��ڶ���
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	f3_count ++;//����������
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else
	{
		f3_flag1 =0;//������
		f3_flag =75;//��������1���ˣ�ֱ�ӽ�����ת	
	
	}
	if(f3_count >100)
	{
	f3_flag1 =0;//������
	f3_flag =76;//��������1���ˣ�ֱ�ӽ�����ת
	}
}
if(f3_flag==75)//�ڶ��ΰ�Ȧ��ת��ײ����
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 10;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	f3_count --;//����������
	if(sensor_error)//������ײ
	{
		f3_count=10;
	}
	if(f3_count >0)//����
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	if(f3_count==0)//��ײ����ͺ���ָ��ִ����ϣ����벽��70
	{
	f3_flag1 =0;//������
	f3_flag =76;//��������1���ˣ�ֱ�ӽ�����ת	
	}	
}
if(f3_flag==76)//����ת�ص�ɨ��
{
	if(f3_flag1==0)//����PWM
	{
		f3_count = 0;
		reset_pwm();//����PWM
		f3_flag1= 1;//���ڽ�������
	}
	
	if(sensor_error)//��ײ
	{
		f3_flag=-1;
		f3_flag1=0;
		reset_pwm();//����PWM
		f2_mode=1;
		f3_flag0 =1;
//			BUZZER_num=30;//����������ʱ��
//			BUZZER_flag=1;//����������
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
			reset_pwm();//����PWM
				f2_mode=1;
			f3_flag0 =1;

		}
		f3_flag4--;		
	}		
}











}	




void back(int back_flag,int back_flag2)//����
{
	if(f3_flag1==1)//����PWM
	{
		f3_crash1 = back_flag2;
		reset_pwm();//����PWM
		f3_flag1= 2;//���ڽ�������

	}
	if(sensor_error )	//���˼�����		//��ײ���ߵؼ������
	{

		f3_crash1=back_flag2;	//���˴���
	}
	if(f3_crash1>0)
	{

			advance_L(0,20,0);
			advance_R(0,20,0);
			f3_crash1--;
	}
	if(f3_crash1==0)//��ײ����ͺ���ָ��ִ����ϣ����벽��2
	{
		f3_flag= back_flag;
		f3_flag1= 0;//���ڽ�������
	}	

}








void f3_stop(void)			//	ֹͣ����	
{
	if(f3_flag==-1)//����-1 ��ͣ
	{
		if(f3_flag1==0)//����PWM
		{
			if(TK_ZJ!=1)
			{
				Brush = 0;//��ˢ�ر�
				fan = 0;//����ر�
				fan_open = 0;//�������1,�ر�0
			}
			f3_fengji=0;//���η������
			f3_count =0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
	}
}	
	


void f3_run_WIFI(void)	//	WIFI���򰴼�	
{
//wifi���򰴼�
if(f3_flag==220)//����220 ǰ��
{
	if(f3_flag1==0)//����PWM
	{
		Brush=1;//��ˢ
//		fan=1;//���
		fan_open = 1;//�������1,�ر�0
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
	}		

	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(1,20,0);
	}
	else//��ײ
	{
		Brush=0;//��ˢ
		fan=0;//���
		fan_open = 0;//�������1,�ر�0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
if(f3_flag==221)//����221 ����
{
	if(f3_flag1==0)//����PWM
	{
		Brush=1;//��ˢ
//		fan=1;//���
		fan_open = 1;//�������1,�ر�0
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
	}		
	if(sensor_error==0)
	{
		advance_L(0,20,0);
		advance_R(0,20,0);
	}
	else//��ײ
	{
		Brush=0;//��ˢ
		fan=0;//���
		fan_open = 0;//�������1,�ر�0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
if(f3_flag==222)//����222 ��
{
	if(f3_flag1==0)//����PWM
	{
		Brush=1;//��ˢ
//		fan=1;//���
		fan_open = 1;//�������1,�ر�0
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
	}		
	if(sensor_error==0)
	{

		advance_L(0,20,0);
		advance_R(1,20,0);
	}
	else//��ײ
	{
		Brush=0;//��ˢ
		fan=0;//���
		fan_open = 0;//�������1,�ر�0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}	
if(f3_flag==223)//����222 ��
{
	if(f3_flag1==0)//����PWM
	{
		Brush=1;//��ˢ
//		fan=1;//���
		fan_open = 1;//�������1,�ر�0
		reset_pwm();
		f3_flag1 = 1;//���ڽ�������
	}		
	if(sensor_error==0)
	{
		advance_L(1,20,0);
		advance_R(0,20,0);
	}
	else//��ײ
	{
		Brush=0;//��ˢ
		fan=0;//���
		fan_open = 0;//�������1,�ر�0
		reset_pwm();
		f3_flag=-1;
		f3_flag1 = 0;
	}
}
	
	
}







void f3_run_ZJ(void)//	�Լ����Ӳ���	
{

//�Լ����
	if(f3_flag==200)//����200 ����ǰ��
	{		
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_L(1,25,1);
	}	
	if(f3_flag==201)//����200 ����ǰ��
	{
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_R(1,25,1);
	}
	if(f3_flag==202)//����200 ���ֺ���
	{
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_L(0,25,1);
	}		
	if(f3_flag==203)//����200 ���ֺ���
	{
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_R(0,25,1);
	}		
	if(f3_flag==204)//����200 2��ǰ��
	{
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_L(1,25,1);
			advance_R(1,25,1);
	}	
	if(f3_flag==205)//����200 2�ֺ���
	{
		if(f3_flag1==0)//����PWM
		{
			f3_count = 0;//�����������������ߵĳ���
			reset_pwm();
			f3_flag1 = 1;//���ڽ�������
		}		
			advance_L(0,25,1);
			advance_R(0,25,1);
	}		

}	
	
void f3_run_first(void)//	���Ρ����㡢��������ʼ��
{
if((f2_mode==0||f2_mode==4||f2_mode==5)&&f3_flag0&&TK_ZJ==0)//ֹͣ.4Ϊ����5Ϊ����
{
	f3_fengji=0;//���η������
	Brush = 0;//��ˢ�ر�
	fan = 0;//����ر�
	fan_open = 0;//�������1,�ر�0
	f3_flag0 =0;//����������־��һ�����ڰ�������ִ�е�һ�β���
	f3_flag=-1 ;//�������У���100�ಽ��
	f3_flag1 = 0;//��¼��־��һ�����ڲ����һ�θ�λ
}	

if(f2_mode==1&&f3_flag0)//����
{
	
	f3_fengji=0;//���η������
	Brush = 1;//��ˢ����
//	fan = 1;//����ر�
	fan_open = 1;//�������1,�ر�0
	f3_flag0 =0;//����������־��һ�����ڰ�������ִ�е�һ�β���
	f3_flag=0 ;//�������У���100�ಽ��
	f3_flag1 = 0;//��¼��־��һ�����ڲ����һ�θ�λ
}


if(f2_mode==10&&f3_flag0)//����
{
	f3_fengji=0;//���η������
	Brush = 1;//��ˢ����
//	fan = 1;//����ر�
	fan_open = 1;//�������1,�ر�0
	f3_flag0 =0;//����������־��һ�����ڰ�������ִ�е�һ�β���
	f3_flag=70 ;//�������У���100�ಽ��
	f3_flag1 = 0;//��¼��־��һ�����ڲ����һ�θ�λ
}
}
	
	


	
	
	
	
	
	
	
	
	
	
	
	
/*****************PWM��ʼ��*********************************************/
static void initialize_pwm()
{
	PWM0PH = 0x07;						//���ڸ�4λ����Ϊ0x07
	PWM0PL = 0xFF;						//���ڵ�8λ����Ϊ0xFF
	PWM1PH = 0x07;						//���ڸ�4λ����Ϊ0x07
	PWM1PL = 0xFF;						//���ڵ�8λ����Ϊ0xFF	
	PWM2PH = 0x07;						//���ڸ�4λ����Ϊ0x07
	PWM2PL = 0xFF;						//���ڵ�8λ����Ϊ0xFF	
	//ռ�ձȼ���= 0x0155 / (Fosc / PWM��Ƶϵ��)		��Fosc��ϵͳʱ�����õĲ��֣�
	//			= 0x0155 / (16000000 / 8)			
	// 			= 341 	 / 2000000
	//			= 170.5us		   ռ�ձ�Ϊ 170.5/511.5 = 33.3%
/*****************����*********************************************/
	PWM0DH = 0;						//00PWM0��4λռ�ձ�0x01			P2.2
	PWM0DL = 0;						//00PWM0��8λռ�ձ�0x55			P2.3
	PWM0DTH = 0;						//03PWM01��4λռ�ձ�0x01 		P2.2
	PWM0DTL = 00;						//55PWM01��8λռ�ձ�0x55		P2.3
/*****************����*********************************************/
	PWM1DH = 0;						//00PWM0��4λռ�ձ�0x01			P
	PWM1DL = 0;						//00PWM0��8λռ�ձ�0x55
	PWM1DTH = 0;						//01PWM01��4λռ�ձ�0x01
	PWM1DTL = 00;						//55PWM01��8λռ�ձ�0x55
	
	
	
	PWM2DH = 0;						//00PWM0��4λռ�ձ�0x01			P
	PWM2DL = 0;						//00PWM0��8λռ�ձ�0x55
	PWM2DTH = 0;						//01PWM01��4λռ�ձ�0x01
	PWM2DTL = 00;						//55PWM01��8λռ�ձ�0x55
	
	
	
//	PWM2PH = 0x03;						//���ڸ�4λ����Ϊ0x03
//	PWM2PL = 0xFF;						//���ڵ�8λ����Ϊ0xFF
//	PWM2DH = 2;						//00PWM0��4λռ�ձ�0x01			P
//	PWM2DL = 0;						//00PWM0��8λռ�ձ�0x55
//	PWM2DTH = 2;						//01PWM01��4λռ�ձ�0x01
//	PWM2DTL = 00;						//55PWM01��8λռ�ձ�0x55

	PWMEN |= 0x77;						//ʹ��PWM0�Լ�PWM01   ��ʹ��PWM1�Լ�PWM01
}	
	
	
	
void reset_pwm(void)//PWM����
{
	/*****************����*********************************************/
	PWM0DH = 0;						//00PWM0��4λռ�ձ�0x01			P2.2
	PWM0DL = 0;						//00PWM0��8λռ�ձ�0x55			P2.3
	PWM0DTH = 0;						//03PWM01��4λռ�ձ�0x01 		P2.2
	PWM0DTL = 00;						//55PWM01��8λռ�ձ�0x55		P2.3
/*****************����*********************************************/
	PWM1DH = 0;						//00PWM0��4λռ�ձ�0x01			P
	PWM1DL = 0;						//00PWM0��8λռ�ձ�0x55
	PWM1DTH = 0;						//01PWM01��4λռ�ձ�0x01
	PWM1DTL = 00;						//55PWM01��8λռ�ձ�0x55
	
	
	num_L = 0;//������
	num_R = 0;//������
	num_L2 = 0;//������
	num_R2 = 0;//������
}	
	
	
	
	
	
	
	
	
	
	
//	
///*****************************************
//go 1ǰ�� 0����
//v		�ٶ�

//********************************************/
	
void advance_L(int go,int v,int mode)//���
{
//	int count_LR = 0;
	if (PWM1DTH==0&&PWM1DTL==0&&go==1)//ǰ����ʼ��
		{	
//		PWM1DTH=2;
//		PWM1DTL=2;
		sum_L = 0;					//�ۻ�ƫ��
		count_L_1 = 0;			//����������ƫ��
		count_L_2 = 0;			//�������ϴ�ƫ��
		num_L=0;						//���ֱ�����
		
		count_sum_L = 0;		//ϵͳƫ�����pid��d
		count_sum1_L = 0;		//ϵͳƫ���ǰƫ��
		count_sum2_L = 0;		//ϵͳƫ���һ��ƫ��
		count_sum3_L = 0;		//ϵͳƫ����ϴ�ƫ��
		pwm_L=600;	
			
	}
	if(PWM1DH==0&&PWM1DL==0&&go==0)//���˳�ʼ��
	{	
//		PWM1DL = 2;	
//		PWM1DH = 2;
		sum_L = 0;					//�ۻ�ƫ��
		count_L_1 = 0;			//����������ƫ��
		count_L_2 = 0;			//����������ƫ��
		num_L=0;						//���ֱ�����
		
		count_sum_L = 0;		//ϵͳƫ�����pid��d
		count_sum1_L = 0;		//ϵͳƫ���ǰƫ��
		count_sum2_L = 0;		//ϵͳƫ���һ��ƫ��
		count_sum3_L = 0;		//ϵͳƫ����ϴ�ƫ��
		pwm_L=600;

	}
	count_L_2 = count_L_1;			//�ϴ�ƫ��
	count_L_1 = v -	num_L;			//����ƫ��,�����������
	count_sum3_L = count_sum2_L;//����һ��ϵͳƫ��
	count_sum2_L = count_sum1_L;//��һ��ϵͳƫ��
	count_sum1_L = count_L_1;		//��ǰϵͳƫ��
	sum_L += count_L_1;					//�ܵ��ۻ�ƫ��
	count_sum_L = count_sum1_L-2*count_sum2_L+count_sum3_L;//ϵͳƫ�����pid��d

		/**************************************************************************
		�������ܣ��ٶȱջ�����PID������
		P ���������Ӧ�ٶȡ�I ���ڼ�С���D ���������𵴡�
		����  ֵ�����PWM
		����λ��ʽ��ɢPID��ʽ 
		pwm +=Kp*[e(k)-e(k-1)] + Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
		e(k)������ƫ�� 
		e(k-1)������һ�ε�ƫ��  
		��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
		pwm�������
		**************************************************************************/
//	count_LR = num_R2-num_L2;
//	if(count_LR>100)
//		count_LR=100;
//	else if(count_LR<-100)
//		count_LR=-100;
	

		if(mode==0)//����ģʽ 8 3 3
			pwm_L+=15*count_L_1+8*(count_L_1-count_L_2)+2*count_sum_L;
//			pwm_L+=10*(count_L_1-count_L_2)+5*count_L_1+4*count_sum_L;
		else//mode==1��ͬ��ģʽ 4 2 2
			pwm_L+=5*count_L_1+10*(count_L_1-count_L_2)+5*count_sum_L;
		if(pwm_L>2000)//������������ٶ�
		{
			pwm_L=2000;
		}
		if(pwm_L<0)//����������С�ٶ�
		{
			pwm_L=0;	
		}
		if(go)//������
		{
			if(v==0)
			{
				PWM1DTH = 0;//���ָ�λPWM
				PWM1DTL = 0;//���ֵ�λPWM
			
			}
			else
			{
				PWM1DTH = pwm_L/255;//���ָ�λPWM
				PWM1DTL = pwm_L%255;//���ֵ�λPWM
			}
		}
		else//������
			{
			if(v==0)
			{
				PWM1DH = 0;//���ָ�λPWM
				PWM1DL = 0;//���ֵ�λPWM
			
			}
			else
			{
				PWM1DH = pwm_L/255;//���ָ�λPWM
				PWM1DL = pwm_L%255;//���ֵ�λPWM
			}
		}
		num_L1=num_L;
		num_L = 0;

}




void advance_R(int go,int v,int mode)//�ұ�
{
//	int count_RL = 0;
	if (PWM0DH==0&&PWM0DL==0&&go==1)//ǰ����ʼ��
		{	
//		PWM0DH=2;
//		PWM0DL=2;
		sum_R = 0;					//�ۻ�ƫ��
		count_R_1 = 0;			//����������ƫ��
		count_R_2 = 0;			//�������ϴ�ƫ��
		num_R=0;						//���ֱ�����
		
		count_sum_R = 0;		//ϵͳƫ�����pid��d
		count_sum1_R = 0;		//ϵͳƫ���ǰƫ��
		count_sum2_R = 0;		//ϵͳƫ���һ��ƫ��
		count_sum3_R = 0;		//ϵͳƫ����ϴ�ƫ��
		pwm_R=600;

	}
	if(PWM0DTH==0&&PWM0DTL==0&&go==0)//���˳�ʼ��
	{	
//		PWM0DTH = 2;	
//		PWM0DTL = 2;
		sum_R = 0;					//�ۻ�ƫ��
		count_R_1 = 0;			//����������ƫ��
		count_R_2 = 0;			//����������ƫ��
		num_R=0;						//���ֱ�����
		
		count_sum_R = 0;		//ϵͳƫ�����pid��d
		count_sum1_R = 0;		//ϵͳƫ���ǰƫ��
		count_sum2_R = 0;		//ϵͳƫ���һ��ƫ��
		count_sum3_R = 0;		//ϵͳƫ����ϴ�ƫ��
		pwm_R=600;
	}
	count_R_2 = count_R_1;			//�ϴ�ƫ��
	count_R_1 = v -	num_R;			//����ƫ��,�����������
	count_sum3_R = count_sum2_R;//����һ��ϵͳƫ��
	count_sum2_R = count_sum1_R;//��һ��ϵͳƫ��
	count_sum1_R = count_R_1;		//��ǰϵͳƫ��
	sum_R += count_R_1;					//�ܵ��ۻ�ƫ��
	count_sum_R = count_sum1_R-2*count_sum2_R+count_sum3_R;//ϵͳƫ�����pid��d
		/**************************************************************************
		�������ܣ��ٶȱջ�����PID������
		P ���������Ӧ�ٶȡ�I ���ڼ�С���D ���������𵴡�
		����  ֵ�����PWM
		����λ��ʽ��ɢPID��ʽ 
		pwm +=Kp*[e(k)-e(k-1)] + Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
		e(k)������ƫ�� 
		e(k-1)������һ�ε�ƫ��  
		��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
		pwm�������
		**************************************************************************/
//		count_RL = num_L2-num_R2;
//		if(count_RL>100)
//		count_RL=100;
//		else if(count_RL<(-1000))
//		count_RL=-100;
		
//						IE &=~ 0x10;				//����UART1�ж�
//				SBUF = count_RL/10;//task1����8λ��������
//				while(!(SCON & 0x02));
//				SCON &=~ 0x02;			//��������жϱ�־λ
		
		
//		val_right=40*(float)task1_count0+20*task1+30*(float)(task1_count0-task1_count01); 
//		val_right+=+1*(task1_count22-task1_count11)+14*(task1_count0-task1_count01) + 7*task1_count0 +10*task1_sum0; 
		if(mode==0)//ͬ��ģʽ 8 3 3
			pwm_R+=15*count_R_1+8*(count_R_1-count_R_2)+2*count_sum_R;
		else//mode==1��ͬ��ģʽ 4 2 2
			pwm_R+=5*count_R_1+10*(count_R_1-count_R_2)+5*count_sum_R;
	if(pwm_R>2000)//������������ٶ�
		{
			pwm_R=2000;
		}
		if(pwm_R<0)//����������С�ٶ�
		{
			pwm_R=0;	
		}

		
		
		if(go)//������
		{		
			if(v==0)
			{
				PWM0DH = 0;//���ָ�λPWM
				PWM0DL = 0;//���ֵ�λPWM
			}
			else
			{
				PWM0DH = pwm_R/255;//���ָ�λPWM
				PWM0DL = pwm_R%255;//���ֵ�λPWM
			}
				

		}
		else //������
		{	
			if(v==0)
			{
				PWM0DTH = 0;//���ָ�λPWM
				PWM0DTL = 0;//���ֵ�λPWM
			}
			else
			{
					PWM0DTH = pwm_R/255;//���ָ�λPWM
					PWM0DTL = pwm_R%255;//���ֵ�λPWM
			}


		}
		num_R1 = num_R;
		num_R = 0;

}






/***************************************************************************************
  * @˵��  	INT17�жϷ�����
  *	@����	��
  * @����ֵ ��
  * @ע		��
***************************************************************************************/
void INT8_17_Rpt() interrupt INT8_17_VECTOR //�ұ�
{
	if(PINTF2&0x02)						//�ж�INT17�жϱ�־λ
	{
		if(f2_mode!=3&&f2_mode!=11)//���ڳ��ʹ���
		{
			num_R++;
			num_R2++;
			if(run_stop_flag_R ==1)//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
			{
				if(num_R2>1050)
				{
					run_stop_flag =1;//90��ֱ��ֹͣ��־�����ж��ڴ���
					reset_pwm();//����PWM
					run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
				}		
			}
			if(run_stop_flag_R ==2)//90��ֱ�ǿ�ʼ��־������ײֱ���д�������
			{
				if(num_R2>1050)
				{
					run_stop_flag =1;//90��ֱ��ֹͣ��־�����ж��ڴ���
					reset_pwm();//����PWM
					run_stop_flag_R =0;//90��ֱ�ǿ�ʼ��־������ײֱ���д�������
				}		
			}			
		}
		PINTF2 &=~ 0x02;				//���INT17�жϱ�־λ	
		
	}
}

/****************�жϼ�¼����ֱ�����****************************/
void P2INT_Rpt() interrupt P2INT_VECTOR 
{
	
	if(PINTF2&0x80)						//�ж�P2�жϱ�־λ
	{
		if(f2_mode!=3&&f2_mode!=11)//���ڳ��ʹ���
		{
			num_L++;
			num_L2++;
			if(run_stop_flag_L ==1)//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
			{
				if(num_L2>1050)
				{
					run_stop_flag =1;//90��ֱ��ֹͣ��־�����ж��ڴ���
					reset_pwm();//����PWM
					run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־���ڿ�ʼֱ���д�������
				}		
			}	
			if(run_stop_flag_L ==2)//90��ֱ�ǿ�ʼ��־������ײֱ���д�������
			{
				if(num_L2>1050)
				{
					run_stop_flag =1;//90��ֱ��ֹͣ��־�����ж��ڴ���
					reset_pwm();//����PWM
					run_stop_flag_L =0;//90��ֱ�ǿ�ʼ��־������ײֱ���д�������
				}		
			}
			
		}
		PINTF2 &=~ 0x80;				//���P2�жϱ�־λ	

	}
}	








