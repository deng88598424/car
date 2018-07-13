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
//按下K4(S4)按键，1秒左右启电小车（注意不要接入测速模块）
//按下复位健可以停止小车	
//注意程序只做参考之用，要达到最理想的寻迹效果，还需要同学们细心调试。
//注意：电机是走不了直线的，需要同学位细心调试PWM 使两边电机转速达到平衡。

#include<AT89X52.H>		  //包含51单片机头文件，内部有各种寄存器定义
#include<HJ-2WD_PWM_FK.H>		  //飞卡小车专用头文件
#include<SMG.H>
#include<INFRARED.H>
#define uchar unsigned char
#define uint  unsigned int
sbit LATCH1=P2^6;//定义锁存使能端口 段锁存
sbit LATCH2=P2^7;//                 位锁存
sbit KEY1=P0^2;	//按下为0
sbit KEY2=P0^3;
sbit LED_2=P0^7;//灯亮为0
sbit LED_1=P0^6;
sbit LCD1602=P2^5;
unsigned char n,count,angle;//距离标志位，0.5ms次数，角度标识
unsigned int a,num;
unsigned char infrared_data;//定义红外接收数据； 
uchar i,flag;
void DelayUs2x(uchar t)
{   
	while(--t);
}
void DelayMs(uchar t)
{
	while(t--)
	{
	    //大致延时1mS	
		DelayUs2x(245);
	    DelayUs2x(245);
	}
}

/*------------------------------------------------
                    定时器01初始化
//定时器1工作方式1 (舵机 )，定时器0 电机PWM调速控制信号
------------------------------------------------*/
void Time1_Int() interrupt 3//舵机
{
	TH1=0xff;
	TL1=0xa3;
	if(count<angle)//判断0.5ms次数是否小于角度标识
    pwm=1;//确实小于，pwm输出高电平
    else
    pwm=0;//大于则输出低电平
	count=(count+1);//0.5ms次数加1
	count=count%40;//次数始终保持为40即保持周期为20ms
}
void Time0_Init(){
			TMOD=0X21;
        	TH0= 0XFc;		  //1ms定时
         	TL0= 0X18;
           	TR0= 1;
        	ET0= 1;
	        EA = 1;

			TH1=0xff;
	        TL1=0xa3;
			ET1=1;
			TR1= 1;
}
void GetSignal(uchar led){
	stop();	
	while(1){
		if(LED_1==0||LED_2==0){
			break ;
		}
	}  
}
void SendSignal(unsigned char key){
unsigned char i=0;
for(i=0;i<200;i++){
	 switch(key) {
	 	case 0x01: KEY1=0; break;
		case 0x02: KEY2=0; break;
		default:break;
	 }
	 }
KEY1=1;
KEY2=1;
}
void GoCar(void) {
	long timecount=2500;
	while(timecount--){
	run(8);
	}
		
	timecount=1050;
	while(timecount--){
		leftrun(8);
	}
	timecount=9600;
	while(timecount--){
		run(8);
	}
	timecount=1500;
	while(timecount--){
		leftrun(8);
	}
}
void Turn90(){
long timecount=0;

	timecount=3000;
	while(timecount--){
	leftrun(8);
	}
}
void control(unsigned char num){
	switch(num%16){
		case 5:{
			Turn90();
		} break;
	}
}


void StopCar(unsigned char choose) {
	long timecount=0;
	if(choose==0x01){
	    timecount=3600;
		while(timecount--){
		run(8);
		}
			
		timecount=1850;
		while(timecount--){
			leftrun(8);
		}
		timecount=5900;
		while(timecount--){
			run(8);
		}
		timecount=1500;
		while(timecount--){
			leftrun(8);
		}
		
		timecount=4200;
		while(timecount--){
			run(8);
		}
		timecount=3100;
		while(timecount--){
			rightrun(8);
		}
	}
	else if(choose==0x02){
	    timecount=2500;
		while(timecount--){
			rightrun(8);
		}
		timecount=5900;
		while(timecount--){
			run(8);
		}
		timecount=2000;
		while(timecount--){
			leftrun(8);
		}
		timecount=5200;
		while(timecount--){
			run(8);
		}
		
		timecount=2100;
		while(timecount--){
			leftrun(8);
		}
	}
}
//主函数
	void main(void)
{
long timecount=0;
long stoptime=0;
unsigned char circle=0;
unsigned char flag;
unsigned char speed=8;
    P0=0X00; //关数码管
	P1=0xF0;
	  LCD1602=0;
	  P0=0xfe; //取位码 第一位数码管选通，即二进制1111 1110
  LATCH2=1;      //位锁存
  LATCH2=0;
  num=0;
  a=1;
  showsmg(num);
//本实验学习的按键启动知识	    
B:		for(i=0;i<50;i++) //判断K4(S4)是否按下
		{
		   delay(1);	//1ms内判断50次，如果其中有一次被判断到K4(S4)没按下，便重新检测
		   if(P3_7!=0)//当K4(S4)按下时，启动小车前进. P2.7
		   		goto B; //跳转到标号B，重新检测  
		} 
//本实验学习的知识蜂鸣器，注意要在HJ-2WD_FK头文件里定义IO口
	     BUZZ=0;	//50次检测S4确认是按下之后，蜂鸣器发出“滴”声响，然后启动小车。
	     delay(10);
	     BUZZ=1;//响50ms后关闭蜂鸣器

			   //开总中断
		Time0_Init();

			

	while(1)	//无限循环
	{ 
	 
			 //有信号为0  没有信号为1
        if(Left_1_led==0&&Right_1_led==0)

			  {
			   run(speed);   //调用前进函数
			   run(speed);
			   flag=1;
			  }
			 			  
			  if(Left_1_led==0&&Right_1_led==1)	    //左边检测到黑线
			  {				 	 
			   rightrun(speed);		   //调用小车右转	函数
			   }
			   
			   if(Right_1_led==0&&Left_1_led==1)		//右边检测到黑线
			  {	  				    
					leftrun(speed);		  //调用小车左转  函数
			   }
			   if(Right_1_led==1&&Left_1_led==1)		  
			  {
				  run(speed);
				  DelayMs(10);
				  if(flag!=0){
				  	num++;
					showsmg(num);
					SendSignal(0x01);
				  }
				  flag=0;
				if(num%8==2||num%8==6){
					timecount=300;
						while(timecount--){
						run(8);
					}
					Turn90();
					GoByBlack(8,7000);
					GetSignal();
					
				}
				if(num%8==3||num%8==7){
					Turn90();
				}
							   
			   }					 
	  }
}