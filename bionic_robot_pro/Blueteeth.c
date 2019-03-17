#include "blueteeth.h"
uchar send2[MIN];  //需要发送的指令
uchar rece2[MIN] ; //需要接收的数据
uchar rece;        //接收单个字符
uchar key;
void Uart2_Init(void)		//串口2初始化
{
	
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L   = TM2;	  //设定定时初值
	T2H   = TM2>>8; //设定定时初值
	AUXR |= 0x10;		//启动定时器2
	IE2  &= ~0x01;  //禁止串口2中断
	P_SW2|= 0x01;   //设置串口2端口 RXD->P4.6  TXD->P4.7
}

void uart2_send(uchar *s2,uchar n)  //串口2数据发送 
{
	uchar i;
	for(i=0;i<n;i++)
  {
		S2BUF = s2[i];	                     // S2BUF接受/发送缓冲
		while(!(((S2CON|0xFD)&0x02)>>1));    // 等特数据传送	
		S2CON&=~0x02;					               // 清除数据传送标志
	}
}

void uart2_rece(uchar n)     //串口2数据发送
{
	uchar i;
	for(i=0;i<n;i++)
	{
		rece2[i]=S2BUF;                      //S2BUF接受/发送缓冲
		while(!((S2CON|0xFE)&0x01));         // 等特数据传送	
		S2CON&=~0x01;	                       // 清除数据传接收标志
	}
}
void uchar_rece()     //串口2数据接收
{
		rece=S2BUF;                          //S2BUF接受/发送缓冲
		while(!((S2CON|0xFE)&0x01));         // 等特数据传送	
		S2CON&=~0x01;	                       // 清除数据传接收标志
}

void rece_pass()
{
			key=S2BUF;                      //S2BUF接受/发送缓冲
			while(!((S2CON|0xFE)&0x01));         // 等特数据传送	
			S2CON&=~0x01;	                       // 清除数据传接收标志
}

