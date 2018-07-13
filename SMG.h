#ifndef _SMG_H_
#define _SMG_H_
sbit LED0 =P1^0;
sbit LED1 =P1^1;
sbit DU =P2^6;	 //HL-1	HJ-3G
sbit WE =P2^7;  //HL-1  HJ-3G
sbit FM =P1^2; //P2.3  HL-1
sbit EN =P2^5; //LCD1602
sbit RST =P1^2; //DS1302 
unsigned char code dofly_DuanMa[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值
unsigned char dis_num;
void hjc52_init(void)
{
FM=0;	//FM
EN=0;	//LCD1602
RST=0; //DS1302
}
void showsmg(unsigned char num){
	hjc52_init();
  DU=1;	//打开锁存   
  P0=dofly_DuanMa[num];		  //000000110
  DU=0;	//关

  WE=1;
  P0=0xdf;  //1101 1110
  DU=0;
}
#endif