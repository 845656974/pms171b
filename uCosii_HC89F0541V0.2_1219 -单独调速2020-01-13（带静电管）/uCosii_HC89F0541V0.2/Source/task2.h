#ifndef __TASK2_H
#define	__TASK2_H


//按键检测值 新板按下去是低电平，touch_test设置1 旧版按下去是高电平 touch_test设置0
bool touch_test= 1;	//按键检测值 

//3开头为1
// 2开头为0



#define DJ_flag_sensitivity 2	 //地检灵敏度，最低为1,越大越不灵敏
#define DJ_num_test 80 //f2_DJ_mode地检函数值，检测是否悬空，AD值
#define standby_wifi_time 9000//快慢闪切换到正常模式时间 15000*0.008=120s
#define standby_error_time 7500//异常切换到正常模式时间 7500*0.008=60s
#define standby_sleep_time 30000//正常切换到睡眠模式时间 37500*0.008=300s






void f2_ledshow(void);//LED灯状态显示
void f2_GRled(int G,int R);//LED
void f2_BUZZER(void);//蜂鸣器
void f2_ZJ_MODE(void);//自检模式
void f2_DJ_mode(INT16U f2_DJ_ADC,INT16U f2_DJ_ADC_old,INT16U f2_DJ_ADC_flag );//地检
void f2_Get_ADC(INT16U adc_flag);//读取adc
void f2_error_buzzer(void);//异常报警
void f2_touch(void);//按键检测
void f2_power(void);//充电检测
void f2_bat(void);//电池充电&电量&电池异常报错
void f2_standby(void);//待机切换
void f2_error(void);//墙检，地检，碰撞
void bubbleSort(int *src, int n);//冒泡排序

#endif



