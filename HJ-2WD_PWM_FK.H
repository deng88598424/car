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
//ע�⣺������߲���ֱ�ߵģ���Ҫͬѧλϸ�ĵ���PWM ʹ���ߵ��ת�ٴﵽƽ�⡣
#ifndef _LED_H_
#define _LED_H_

    //����С������ģ������IO�� 
	sbit IN1=P1^3;
	sbit IN2=P1^4;
	sbit IN3=P1^6;
	sbit IN4=P1^7;
	sbit EN1=P1^4;
	sbit EN2=P1^5;
	
	//����ת����IO��
    sbit pwm=P2^7;//PWM�ź������  ����ź������
	//���������߶���
    sbit BUZZ=P2^3; //MCS-51 P2^6;
	//���峬�����ӿ�
	sbit TRIG=P2^0;
    sbit ECHO=P2^1;  

  	#define Left_1_led        P3_4	 // �󴫸���  P3.3     
	#define Right_1_led       P3_3	 //�Ҵ�����  P3.2
	

   	 
	#define Left_moto_pwm	  P1_4	 //PWM�źŶ�
	#define Right_moto_pwm	  P1_5	  //PWM�źŶ�


  #define Left_moto_go      {P1_2=0,P1_3=1;}    //��ߵ����ǰ��
	#define Left_moto_back    {P1_2=1,P1_3=0;}    //��ߵ�����ת
	#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //��ߵ��ͣת                     
	#define Right_moto_go     {P1_6=1,P1_7=0;}	//�ұߵ����ǰ��
	#define Right_moto_back   {P1_6=0,P1_7=1;}	//�ұߵ�������
	#define Right_moto_Stop   {P1_6=0,P1_7=0;}	//�ұߵ��ͣת 
//	unsigned char PWMSD=9;	 ////�ٶȵ��ڱ��� 0-20������0��С��20���  ���С��8������ܲ���
	unsigned char pwm_val_left  =0;//��������
	unsigned char push_val_left =0;// ����ռ�ձ�N/20
	unsigned char pwm_val_right =0;
	unsigned char push_val_right=0;// �ҵ��ռ�ձ�N/20
	bit Right_moto_stop=1;
	bit Left_moto_stop =1;
	unsigned  int  time=0;
	unsigned int lefttime,righttime;
   
/************************************************************************/	
//��ʱ����	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/
//ǰ��ǰ��
void  run(unsigned char PWMSD)
{
   		push_val_left=PWMSD+1;	 //9 �ٶȵ��ڱ��� 0-20������0��С��20���
	 	push_val_right=PWMSD;
		//push_val_left=9;	 //�ٶȵ��ڱ��� 0-20������0��С��20���
	 //push_val_right=9;
	 Left_moto_go ;   //������ǰ��
	 Right_moto_go ;  //�ҵ����ǰ��
}

//��ת����
     void  leftrun(unsigned char PWMSD)
{	 
     push_val_left=PWMSD;
	 push_val_right=PWMSD;
	 Right_moto_go ;  //�ҵ����ǰ��
     Left_moto_back   ;  //��������
}

//��ת����
     void  rightrun(unsigned char PWMSD)
{ 
	 push_val_left=PWMSD;
	 push_val_right=PWMSD;
     Left_moto_go  ;   //������ǰ��
	 Right_moto_back    ;  //�ҵ��������
}
void stop(void){
	 Left_moto_Stop;
	 Right_moto_Stop; 
}
void back(unsigned char PWMSD){
	   P1=0xFF;
   		push_val_left=PWMSD;	 //9 �ٶȵ��ڱ��� 0-20������0��С��20���
	 	push_val_right=PWMSD;
		//push_val_left=9;	 //�ٶȵ��ڱ��� 0-20������0��С��20���
	 //push_val_right=9;
	 Left_moto_back ;   //������ǰ��
	 Right_moto_back ;  //�ҵ����ǰ��
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
/*                    PWM���Ƶ��ת��                                   */
/************************************************************************/
/*                    ��������                                        */
/*����push_val_left��ֵ�ı���ת��,ռ�ձ�            */
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
/*                    �ҵ������                                  */  
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
		   run(speed);   //����ǰ������
		   run(speed);
		  }
		 			  
		  if(Left_1_led==0&&Right_1_led==1)	    //��߼�⵽����
		 	 
		  {				 	 
		   rightrun(speed);		   //����С����ת	����
	
		   }
		   
		   if(Right_1_led==0&&Left_1_led==1)		//�ұ߼�⵽����
			  
		  {	  				    
				  leftrun(speed);		  //����С����ת  ����
		   }
		   if(Right_1_led==0&&Left_1_led==1){
		   		run(speed);
		   }
	}	
}       
/***************************************************/
///*TIMER0�жϷ����Ӻ�������PWM�ź�*/
 	void timer0()interrupt 1   using 2
{
     TH0=0XFC;	  //1Ms��ʱ
	 TL0=0X18;
	 time++;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
 }
	

/*********************************************************************/	

#endif