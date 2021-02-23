#ifndef __TASK3_H
#define	__TASK3_H
static void initialize_pwm();//PWM初始化
void advance_L(int go,int v,int mode);//左边
void advance_R(int go,int v,int mode);//右边
void reset_pwm(void);//PWM重置




void f3_stop(void);			//	停止运行
void f3_run(void);			//	正常运行
void f3_run_first(void);//	弓形、定点、快慢闪初始化
void f3_run_ZJ(void);		//	自检轮子测试
void f3_run_WIFI(void);	//	WIFI方向按键
void back(int back_flag,int back_flag2);//后退
#endif