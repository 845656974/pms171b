#ifndef __TASK3_H
#define	__TASK3_H
static void initialize_pwm();//PWM��ʼ��
void advance_L(int go,int v,int mode);//���
void advance_R(int go,int v,int mode);//�ұ�
void reset_pwm(void);//PWM����




void f3_stop(void);			//	ֹͣ����
void f3_run(void);			//	��������
void f3_run_first(void);//	���Ρ����㡢��������ʼ��
void f3_run_ZJ(void);		//	�Լ����Ӳ���
void f3_run_WIFI(void);	//	WIFI���򰴼�
void back(int back_flag,int back_flag2);//����
#endif