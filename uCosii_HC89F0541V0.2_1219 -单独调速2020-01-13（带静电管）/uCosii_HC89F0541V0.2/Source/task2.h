#ifndef __TASK2_H
#define	__TASK2_H


//�������ֵ �°尴��ȥ�ǵ͵�ƽ��touch_test����1 �ɰ水��ȥ�Ǹߵ�ƽ touch_test����0
bool touch_test= 1;	//�������ֵ 

//3��ͷΪ1
// 2��ͷΪ0



#define DJ_flag_sensitivity 2	 //�ؼ������ȣ����Ϊ1,Խ��Խ������
#define DJ_num_test 80 //f2_DJ_mode�ؼ캯��ֵ������Ƿ����գ�ADֵ
#define standby_wifi_time 9000//�������л�������ģʽʱ�� 15000*0.008=120s
#define standby_error_time 7500//�쳣�л�������ģʽʱ�� 7500*0.008=60s
#define standby_sleep_time 30000//�����л���˯��ģʽʱ�� 37500*0.008=300s






void f2_ledshow(void);//LED��״̬��ʾ
void f2_GRled(int G,int R);//LED
void f2_BUZZER(void);//������
void f2_ZJ_MODE(void);//�Լ�ģʽ
void f2_DJ_mode(INT16U f2_DJ_ADC,INT16U f2_DJ_ADC_old,INT16U f2_DJ_ADC_flag );//�ؼ�
void f2_Get_ADC(INT16U adc_flag);//��ȡadc
void f2_error_buzzer(void);//�쳣����
void f2_touch(void);//�������
void f2_power(void);//�����
void f2_bat(void);//��س��&����&����쳣����
void f2_standby(void);//�����л�
void f2_error(void);//ǽ�죬�ؼ죬��ײ
void bubbleSort(int *src, int n);//ð������

#endif



