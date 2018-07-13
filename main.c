/*****************HJ-2WD�������*****************
*  ƽ̨���۾��ɿ�51С�� + Keil U4 + STC89C52RC
*  �������ƣ�����С��Ѳ����Դ����(���ת��)
*  ��˾���۾�����
*  �Ա���https://shop37031453.taobao.com/  
*  ��վ��www.hjmcu.com	
*  ����:11.0592MHZ
*  �ۺ���˵������ѿ�Դ����֧�ֿͻ��Լ��޸ĳ�����������
*  ��Ƶ�̳̣���С������ѧϰC������ϸ��Ƶ�̳̣�����ͳһ��������
******************************************************************/
//����K4(S4)������1����������С����ע�ⲻҪ�������ģ�飩
//���¸�λ������ֹͣС��	
//ע�����ֻ���ο�֮�ã�Ҫ�ﵽ�������Ѱ��Ч��������Ҫͬѧ��ϸ�ĵ��ԡ�
//ע�⣺������߲���ֱ�ߵģ���Ҫͬѧλϸ�ĵ���PWM ʹ���ߵ��ת�ٴﵽƽ�⡣

#include<AT89X52.H>		  //����51��Ƭ��ͷ�ļ����ڲ��и��ּĴ�������
#include<HJ-2WD_PWM_FK.H>		  //�ɿ�С��ר��ͷ�ļ�
#include<SMG.H>
#include<INFRARED.H>
#define uchar unsigned char
#define uint  unsigned int
sbit LATCH1=P2^6;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^7;//                 λ����
sbit KEY1=P0^2;	//����Ϊ0
sbit KEY2=P0^3;
sbit LED_2=P0^7;//����Ϊ0
sbit LED_1=P0^6;
sbit LCD1602=P2^5;
unsigned char n,count,angle;//�����־λ��0.5ms�������Ƕȱ�ʶ
unsigned int a,num;
unsigned char infrared_data;//�������������ݣ� 
uchar i,flag;
void DelayUs2x(uchar t)
{   
	while(--t);
}
void DelayMs(uchar t)
{
	while(t--)
	{
	    //������ʱ1mS	
		DelayUs2x(245);
	    DelayUs2x(245);
	}
}

/*------------------------------------------------
                    ��ʱ��01��ʼ��
//��ʱ��1������ʽ1 (��� )����ʱ��0 ���PWM���ٿ����ź�
------------------------------------------------*/
void Time1_Int() interrupt 3//���
{
	TH1=0xff;
	TL1=0xa3;
	if(count<angle)//�ж�0.5ms�����Ƿ�С�ڽǶȱ�ʶ
    pwm=1;//ȷʵС�ڣ�pwm����ߵ�ƽ
    else
    pwm=0;//����������͵�ƽ
	count=(count+1);//0.5ms������1
	count=count%40;//����ʼ�ձ���Ϊ40����������Ϊ20ms
}
void Time0_Init(){
			TMOD=0X21;
        	TH0= 0XFc;		  //1ms��ʱ
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
//������
	void main(void)
{
long timecount=0;
long stoptime=0;
unsigned char circle=0;
unsigned char flag;
unsigned char speed=8;
    P0=0X00; //�������
	P1=0xF0;
	  LCD1602=0;
	  P0=0xfe; //ȡλ�� ��һλ�����ѡͨ����������1111 1110
  LATCH2=1;      //λ����
  LATCH2=0;
  num=0;
  a=1;
  showsmg(num);
//��ʵ��ѧϰ�İ�������֪ʶ	    
B:		for(i=0;i<50;i++) //�ж�K4(S4)�Ƿ���
		{
		   delay(1);	//1ms���ж�50�Σ����������һ�α��жϵ�K4(S4)û���£������¼��
		   if(P3_7!=0)//��K4(S4)����ʱ������С��ǰ��. P2.7
		   		goto B; //��ת�����B�����¼��  
		} 
//��ʵ��ѧϰ��֪ʶ��������ע��Ҫ��HJ-2WD_FKͷ�ļ��ﶨ��IO��
	     BUZZ=0;	//50�μ��S4ȷ���ǰ���֮�󣬷������������Ρ����죬Ȼ������С����
	     delay(10);
	     BUZZ=1;//��50ms��رշ�����

			   //�����ж�
		Time0_Init();

			

	while(1)	//����ѭ��
	{ 
	 
			 //���ź�Ϊ0  û���ź�Ϊ1
        if(Left_1_led==0&&Right_1_led==0)

			  {
			   run(speed);   //����ǰ������
			   run(speed);
			   flag=1;
			  }
			 			  
			  if(Left_1_led==0&&Right_1_led==1)	    //��߼�⵽����
			  {				 	 
			   rightrun(speed);		   //����С����ת	����
			   }
			   
			   if(Right_1_led==0&&Left_1_led==1)		//�ұ߼�⵽����
			  {	  				    
					leftrun(speed);		  //����С����ת  ����
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