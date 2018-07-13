/*****************HJ-2WD舵机控制*****************
*  平台：慧净飞卡51小车 + Keil U4 + STC89C52RC
*  程序名称：智能小车巡黑线源程序(舵机转弯)
*  公司：慧净电子
*  淘宝：https://shop37031453.taobao.com/  
*  网站：www.hjmcu.com	
*  晶振:11.0592MHZ
*  售后技术说明：免费开源，不支持客户自己修改程序分析与改正
*  视频教程：本小车配套学习C语言详细视频教程，资料统一网盘下载
******************************************************************/
//注意：电机是走不了直线的，需要同学位细心调试PWM 使两边电机转速达到平衡。
#ifndef _LED_H_
#define _LED_H_

    //定义小车驱动模块输入IO口 
	sbit IN1=P1^3;
	sbit IN2=P1^4;
	sbit IN3=P1^6;
	sbit IN4=P1^7;
	sbit EN1=P1^4;
	sbit EN2=P1^5;
	
	//定义转向舵机IO口
    sbit pwm=P2^7;//PWM信号输出口  舵机信号输出口
	//蜂鸣器接线定义
    sbit BUZZ=P2^3; //MCS-51 P2^6;
	//定义超声波接口
	sbit TRIG=P2^0;
    sbit ECHO=P2^1;  

  	#define Left_1_led        P3_4	 // 左传感器  P3.3     
	#define Right_1_led       P3_3	 //右传感器  P3.2
	

   	 
	#define Left_moto_pwm	  P1_4	 //PWM信号端
	#define Right_moto_pwm	  P1_5	  //PWM信号端


  #define Left_moto_go      {P1_2=0,P1_3=1;}    //左边电机向前走
	#define Left_moto_back    {P1_2=1,P1_3=0;}    //左边电机向后转
	#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //左边电机停转                     
	#define Right_moto_go     {P1_6=1,P1_7=0;}	//右边电机向前走
	#define Right_moto_back   {P1_6=0,P1_7=1;}	//右边电机向后走
	#define Right_moto_Stop   {P1_6=0,P1_7=0;}	//右边电机停转 
//	unsigned char PWMSD=9;	 ////速度调节变量 0-20。。。0最小，20最大  如果小于8电机可能不动
	unsigned char pwm_val_left  =0;//变量定义
	unsigned char push_val_left =0;// 左电机占空比N/20
	unsigned char pwm_val_right =0;
	unsigned char push_val_right=0;// 右电机占空比N/20
	bit Right_moto_stop=1;
	bit Left_moto_stop =1;
	unsigned  int  time=0;
	unsigned int lefttime,righttime;
   
/************************************************************************/	
//延时函数	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/
//前速前进
void  run(unsigned char PWMSD)
{
   		push_val_left=PWMSD+1;	 //9 速度调节变量 0-20。。。0最小，20最大
	 	push_val_right=PWMSD;
		//push_val_left=9;	 //速度调节变量 0-20。。。0最小，20最大
	 //push_val_right=9;
	 Left_moto_go ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//左转函数
     void  leftrun(unsigned char PWMSD)
{	 
     push_val_left=PWMSD;
	 push_val_right=PWMSD;
	 Right_moto_go ;  //右电机往前走
     Left_moto_back   ;  //左电机后走
}

//右转函数
     void  rightrun(unsigned char PWMSD)
{ 
	 push_val_left=PWMSD;
	 push_val_right=PWMSD;
     Left_moto_go  ;   //左电机往前走
	 Right_moto_back    ;  //右电机往后走
}
void stop(void){
	 Left_moto_Stop;
	 Right_moto_Stop; 
}
void back(unsigned char PWMSD){
	   P1=0xFF;
   		push_val_left=PWMSD;	 //9 速度调节变量 0-20。。。0最小，20最大
	 	push_val_right=PWMSD;
		//push_val_left=9;	 //速度调节变量 0-20。。。0最小，20最大
	 //push_val_right=9;
	 Left_moto_back ;   //左电机往前走
	 Right_moto_back ;  //右电机往前走
}
void turn(unsigned char PWMSD){
	 push_val_left=PWMSD;
	 push_val_right=PWMSD;
	Left_moto_go;
	Right_moto_back;
}
void turn180(void)
{
	lefttime=9500;
	righttime=50000;
	while(lefttime--) {
		turn(8);
	}
	push_val_left=3;
	push_val_right=3;
IF1:if(Right_1_led==0&&Left_1_led==0)
	{
		turn(8);
		goto IF1;
	}
IF3:if(Right_1_led==0&&Left_1_led==1)
	{
		turn(8);
		if(Right_1_led==1&&Left_1_led==0){
		return;}
		goto IF3;
	}else
IF2:if(Right_1_led==1&&Left_1_led==0)
	{
		turn(8);
		if(Right_1_led==0&&Left_1_led==1){
		return;}
		goto IF2;
	} 


}
void  go_turn( unsigned int time){
unsigned int time0=time;
time=time0;
while(time--){
	if(Left_1_led==0&&Right_1_led==1)
	{
	leftrun(5);	
	}
	if(Left_1_led==1&&Right_1_led==0)
	{
	rightrun(5);	
	}
	if(Left_1_led==1&&Right_1_led==1){
		back(5);
	}
	if(Left_1_led==1&&Right_1_led==1){
			run(5);
	}
	if(Left_1_led==0&&Right_1_led==0){
	 	  back(8);
	}
	}  

}

/************************************************************************/
/*                    PWM调制电机转速                                   */
/************************************************************************/
/*                    左电机调速                                        */
/*调节push_val_left的值改变电机转速,占空比            */
		void pwm_out_left_moto(void)
{  
   if(Left_moto_stop)
   {
    if(pwm_val_left<=push_val_left)
	       {
		     Left_moto_pwm=1; 

		   }
	else 
	       {
	         Left_moto_pwm=0;

		   }
	if(pwm_val_left>=20)
	       pwm_val_left=0;
   }
   else    
          {
           Left_moto_pwm=0;

		  }
}
/******************************************************************/
/*                    右电机调速                                  */  
   void pwm_out_right_moto(void)
{ 
  if(Right_moto_stop)
   { 
    if(pwm_val_right<=push_val_right)
	      {
	       Right_moto_pwm=1; 

		   }
	else 
	      {
		   Right_moto_pwm=0;
		  }
	if(pwm_val_right>=20)
	       pwm_val_right=0;
   }
   else    
          {
           Right_moto_pwm=0;

	      }
}
void GoByBlack(unsigned char speed,long time){
	while(time--){
		   if(Left_1_led==0&&Right_1_led==0)
		  {
		   run(speed);   //调用前进函数
		   run(speed);
		  }
		 			  
		  if(Left_1_led==0&&Right_1_led==1)	    //左边检测到黑线
		 	 
		  {				 	 
		   rightrun(speed);		   //调用小车右转	函数
	
		   }
		   
		   if(Right_1_led==0&&Left_1_led==1)		//右边检测到黑线
			  
		  {	  				    
				  leftrun(speed);		  //调用小车左转  函数
		   }
		   if(Right_1_led==0&&Left_1_led==1){
		   		run(speed);
		   }
	}	
}       
/***************************************************/
///*TIMER0中断服务子函数产生PWM信号*/
 	void timer0()interrupt 1   using 2
{
     TH0=0XFC;	  //1Ms定时
	 TL0=0X18;
	 time++;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
 }
	

/*********************************************************************/	

#endif