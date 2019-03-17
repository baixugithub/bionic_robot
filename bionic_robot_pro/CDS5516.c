#include "CDS5516.h"
uchar send1[MAX]={0xFF,0xFF};  //需要发送的指令
uchar rece1[MAX];              //需要接收的数据

void Delayms(uchar N)		//@20.000MHz
{
	uchar i, j, k, n;
	for(n=0;n<N;n++)
	{
		_nop_();
		_nop_();
		i = 1;
		j = 195;
		k = 136;
		do
		{
			do
			{
				while (--k);
			} while (--j);
		} while (--i);
	}
}

void Uart1_Init(void)         //串口1初始化
{
	SCON  = 0x50;		            //8位数据,可变波特率
	AUXR |= 0x40;		            //定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		            //串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		            //设定定时器1为16位自动重装方式
	TL1   = TM1;		            //设定定时初值
	TH1   = TM1>>8;		          //设定定时初值
	ET1   = 0;		              //禁止定时器1中断
	TR1   = 1;		              //启动定时器1
	AUXR1 =(AUXR1 & 0x7F)|0x40; //设置串口1端口 RXD->P3.6  TXD->P3.7
}  

void uart1_send(uchar *s1,uchar n)  //串口1数据发送 
{
	uchar i;
	for(i=0;i<n;i++)
  {
		SBUF = s1[i];	                   //SUBF接受/发送缓冲
    while(!TI);				               // 等特数据传送	(TI发送中断标志)
		TI = 0;					                 // 清除数据传送标志
	}
}

void uart1_rece(uchar n)              //串口1数据发送
{
	uchar i;
	for(i=0;i<n;i++)
	{
		rece1[i]=SBUF;
		while(!RI);
		RI=0;
	}
}

void change_ID(uchar OID , uchar NID )  //改舵机ID号 OID为旧ID  NID为新ID
{
	uchar Check_Sum;
	uchar length=4;
	Check_Sum = ~ (OID + length +WRITE_DATA + EEPROM_ID + NID);
  send1[2]=OID;
	send1[3]=length;
	send1[4]=WRITE_DATA;
	send1[5]=EEPROM_ID;
	send1[6]= NID;
	send1[7]=Check_Sum;
	uart1_send(send1,length+4);
}

void change_BPS(uchar ID, uchar BPS)        //修改舵机的波特率
{
  uchar Check_Sum;
	uchar length=4;
	Check_Sum = ~ (ID + length + WRITE_DATA + EEPROM_BPS + BPS);
  send1[2]=ID;
	send1[3]=length;
	send1[4]=WRITE_DATA;
	send1[5]=EEPROM_BPS;
	send1[6]= BPS;
	send1[7]=Check_Sum;
	uart1_send(send1,length+4);
}

void single_servo(uchar ID,uint angle ,uint speed)   //单个舵机控制
{
	uchar Check_Sum;
	uchar length=7;
	uchar angle_L,angle_H,speed_L,speed_H;
	angle_L=angle%256;
	angle_H=angle>>8;	
	speed_L=speed%256;
	speed_H=speed>>8;
	Check_Sum = ~ (ID + length + WRITE_DATA + 0x1E + 
	               angle_L + angle_H + speed_L + speed_H );
  send1[2] =ID;
	send1[3] =length;
	send1[4] =WRITE_DATA;         //写指令
	send1[5] =0x1E;               //地址
	send1[6] =angle_L; send1[7] =angle_H;
	send1[8] =speed_L; send1[9] =speed_H;
	send1[10]=Check_Sum;
	uart1_send(send1,length+4);
}

void three_sevro(uchar ID1, uint angle1, uint speed1,
	               uchar ID2, uint angle2, uint speed2,
                 uchar ID3, uint angle3, uint speed3) 
{
	uchar Check_Sum;
	uchar length=19;  //(L+1)*N+4 N为控制舵机的个数 
	uchar angle_L1, angle_H1, speed_L1, speed_H1;
	uchar angle_L2, angle_H2, speed_L2, speed_H2;
	uchar angle_L3, angle_H3, speed_L3, speed_H3;
	angle_L1=angle1%256; angle_H1=angle1>>8; speed_L1=speed1%256; speed_H1=speed1>>8;
	angle_L2=angle2%256; angle_H2=angle2>>8; speed_L2=speed2%256; speed_H2=speed2>>8;	
	angle_L3=angle3%256; angle_H3=angle3>>8; speed_L3=speed3%256; speed_H3=speed3>>8;
	Check_Sum = ~ (0xFE + length + SYNC_WRITE + 0x1E + 0x04 + 
	                ID1 + angle_L1 + angle_H1 + speed_L1 + speed_H1+  
	                ID2 + angle_L2 + angle_H2 + speed_L2 + speed_H2+
	                ID3 + angle_L3 + angle_H3 + speed_L3 + speed_H3);
  send1[2] =0xFE;                     //广播ID
	send1[3] =length;
	send1[4] =SYNC_WRITE;               //同步写指令
	send1[5] =0x1E;                     //地址
	send1[6] =0x04;                     //写入数据的长度
	send1[7] =ID1;
	send1[8] =angle_L1; send1[9] =angle_H1;
	send1[10]=speed_L1; send1[11]=speed_H1;
	send1[12]=ID2;
	send1[13]=angle_L2; send1[14]=angle_H2;
	send1[15]=speed_L2; send1[16]=speed_H2;
	send1[17]=ID3;
	send1[18]=angle_L3; send1[19]=angle_H3;
	send1[20]=speed_L3; send1[21]=speed_H3;
	send1[22]=Check_Sum;
	uart1_send(send1,length+4);
}

void four_sevro(uchar ID1, uint angle1, uint speed1,
	              uchar ID2, uint angle2, uint speed2,
                uchar ID3, uint angle3, uint speed3,
                uchar ID4, uint angle4, uint speed4)    
{
	uchar Check_Sum;
	uchar length=24;  //(L+1)*N+4  N为控制舵机的个数
	uchar angle_L1, angle_H1, speed_L1, speed_H1;
	uchar angle_L2, angle_H2, speed_L2, speed_H2;
	uchar angle_L3, angle_H3, speed_L3, speed_H3;
	uchar angle_L4, angle_H4, speed_L4, speed_H4;
	angle_L1=angle1%256; angle_H1=angle1>>8; speed_L1=speed1%256; speed_H1=speed1>>8;
	angle_L2=angle2%256; angle_H2=angle2>>8; speed_L2=speed2%256; speed_H2=speed2>>8;	
	angle_L3=angle3%256; angle_H3=angle3>>8; speed_L3=speed3%256; speed_H3=speed3>>8;
	angle_L4=angle4%256; angle_H4=angle4>>8; speed_L4=speed4%256; speed_H4=speed4>>8;
	Check_Sum = ~ (0xFE + length + SYNC_WRITE + 0x1E + 0x04 + 
	                ID1 + angle_L1 + angle_H1 + speed_L1 + speed_H1+  
	                ID2 + angle_L2 + angle_H2 + speed_L2 + speed_H2+
	                ID3 + angle_L3 + angle_H3 + speed_L3 + speed_H3+
	                ID4 + angle_L4 + angle_H4 + speed_L4 + speed_H4);
  send1[2] =0xFE;                     //广播ID
	send1[3] =length;
	send1[4] =SYNC_WRITE;               //同步写指令
	send1[5] =0x1E;                     //地址
	send1[6] =0x04;                     //写入数据的长度
	send1[7] =ID1;
	send1[8] =angle_L1; send1[9] =angle_H1;
	send1[10]=speed_L1; send1[11]=speed_H1;
	send1[12]=ID2;
	send1[13]=angle_L2; send1[14]=angle_H2;
	send1[15]=speed_L2; send1[16]=speed_H2;
	send1[17]=ID3;
	send1[18]=angle_L3; send1[19]=angle_H3;
	send1[20]=speed_L3; send1[21]=speed_H3;
	send1[22]=ID4;
	send1[23]=angle_L4; send1[24]=angle_H4;
	send1[25]=speed_L4; send1[26]=speed_H4;
	send1[27]=Check_Sum;
	uart1_send(send1,length+4);
}

void six_sevro(uchar ID1, uint angle1, uint speed1,
	             uchar ID2, uint angle2, uint speed2,
               uchar ID3, uint angle3, uint speed3,
               uchar ID4, uint angle4, uint speed4,
	             uchar ID5, uint angle5, uint speed5,
               uchar ID6, uint angle6, uint speed6)  
{
	uchar Check_Sum;
	uchar length=34;  //(L+1)*N+4  N为控制舵机的个数
	uchar angle_L1, angle_H1, speed_L1, speed_H1;
	uchar angle_L2, angle_H2, speed_L2, speed_H2;
	uchar angle_L3, angle_H3, speed_L3, speed_H3;
	uchar angle_L4, angle_H4, speed_L4, speed_H4;
	uchar angle_L5, angle_H5, speed_L5, speed_H5;
	uchar angle_L6, angle_H6, speed_L6, speed_H6;
	angle_L1=angle1%256; angle_H1=angle1>>8; speed_L1=speed1%256; speed_H1=speed1>>8;
	angle_L2=angle2%256; angle_H2=angle2>>8; speed_L2=speed2%256; speed_H2=speed2>>8;	
	angle_L3=angle3%256; angle_H3=angle3>>8; speed_L3=speed3%256; speed_H3=speed3>>8;
	angle_L4=angle4%256; angle_H4=angle4>>8; speed_L4=speed4%256; speed_H4=speed4>>8;
	angle_L5=angle5%256; angle_H5=angle5>>8; speed_L5=speed5%256; speed_H5=speed5>>8;	
	angle_L6=angle6%256; angle_H6=angle6>>8; speed_L6=speed6%256; speed_H6=speed6>>8;
	Check_Sum = ~ (0xFE + length + SYNC_WRITE + 0x1E + 0x04 + 
	                ID1 + angle_L1 + angle_H1 + speed_L1 + speed_H1+  
	                ID2 + angle_L2 + angle_H2 + speed_L2 + speed_H2+
	                ID3 + angle_L3 + angle_H3 + speed_L3 + speed_H3+
	                ID4 + angle_L4 + angle_H4 + speed_L4 + speed_H4+  
	                ID5 + angle_L5 + angle_H5 + speed_L5 + speed_H5+
	                ID6 + angle_L6 + angle_H6 + speed_L6 + speed_H6);
  send1[2] =0xFE;                     //广播ID
	send1[3] =length;
	send1[4] =SYNC_WRITE;               //同步写指令
	send1[5] =0x1E;                     //地址
	send1[6] =0x04;                     //写入数据的长度
	send1[7] =ID1;
	send1[8] =angle_L1; send1[9] =angle_H1;
	send1[10]=speed_L1; send1[11]=speed_H1;
	send1[12]=ID2;
	send1[13]=angle_L2; send1[14]=angle_H2;
	send1[15]=speed_L2; send1[16]=speed_H2;
	send1[17]=ID3;
	send1[18]=angle_L3; send1[19]=angle_H3;
	send1[20]=speed_L3; send1[21]=speed_H3;
	send1[22]=ID4;
	send1[23]=angle_L4; send1[24]=angle_H4;
	send1[25]=speed_L4; send1[26]=speed_H4;
	send1[27]=ID5;
	send1[28]=angle_L5; send1[29]=angle_H5;
	send1[30]=speed_L5; send1[31]=speed_H5;
	send1[32]=ID6;
	send1[33]=angle_L6; send1[34]=angle_H6;
	send1[35]=speed_L6; send1[36]=speed_H6;
	send1[37]=Check_Sum;
	uart1_send(send1,length+4);
}

void reset_servo()
{	
	three_sevro( 1,550,350, 2,512,350, 3,570,350);Delayms(40);
	three_sevro( 4,550,350, 5,450,350, 6,460,350);Delayms(40);
	three_sevro( 7,560,350, 8,560,350, 9,530,350);Delayms(40);
	three_sevro(10,430,350,11,490,350,12,270,350);Delayms(40);
}

void run()
{
	four_sevro(3,650,650, 4,420,650,
                        9,470,650,10,510,650);Delayms(15);
	six_sevro( 4,630,650, 5,480,650, 6,450,650,
	          10,320,650,11,450,650,12,280,650);Delayms(15);
	six_sevro( 4,630,650, 5,450,650, 6,460,650, 
	          10,320,650,11,490,650,12,270,650);Delayms(15);
  six_sevro( 1,580,650, 2,490,650, 3,610,650,
	           7,540,650, 8,580,650, 9,590,650);Delayms(15);
	six_sevro( 1,550,650, 2,512,650, 3,610,650,
	           7,560,650, 8,560,650, 9,590,650);Delayms(15);
	four_sevro(3,570,600, 4,550,600, 
	                       9,530,600,10,430,600);Delayms(15);	
}

void go_forward()       //前进
{
	 six_sevro( 4,630,650, 5,480,650, 6,450,650,
	           10,350,650,11,450,650,12,280,650);Delayms(15);
	 six_sevro( 4,630,650, 5,450,650, 6,460,650, 
	           10,350,650,11,490,650,12,270,650);Delayms(15);
	 six_sevro( 1,580,650, 2,490,650, 3,490,650,
	            7,540,650, 8,580,650, 9,610,650);Delayms(15);
	 six_sevro( 1,550,650, 2,512,650, 3,490,650,
	            7,560,650, 8,560,650, 9,610,650);Delayms(15);
   four_sevro(3,570,650, 4,550,650,
                         9,530,650,10,430,650);Delayms(15);	                      
}

void turn_left()        //左转
{
   six_sevro( 4,470,650, 5,480,650, 6,450,650,
	           10,350,650,11,450,650,12,280,650);Delayms(15);
	 six_sevro( 4,470,650, 5,450,650, 6,460,650, 
	           10,350,650,11,490,650,12,270,650);Delayms(15);
	 six_sevro( 1,580,650, 2,490,650, 3,490,650,
	            7,540,650, 8,580,650, 9,450,650);Delayms(15);
	 six_sevro( 1,550,650, 2,512,650, 3,490,650,
	            7,560,650, 8,560,650, 9,450,650);Delayms(15);
	 four_sevro(3,570,650, 4,550,650, 
	                       9,530,650,10,430,650);Delayms(15);	
}

void turn_right()       //右转
{
   six_sevro( 4,630,650, 5,480,650, 6,450,650,
	           10,510,650,11,450,650,12,280,650);Delayms(15);
	 six_sevro( 4,640,650, 5,450,650, 6,460,650, 
	           10,510,650,11,490,650,12,270,600);Delayms(15);
	 six_sevro( 1,580,650, 2,490,650, 3,640,650,
	            7,540,650, 8,580,650, 9,610,650);Delayms(15);
	 six_sevro( 1,550,650, 2,512,650, 3,640,650,
	            7,560,650, 8,560,650, 9,610,650);Delayms(15);
	four_sevro( 3,570,650, 4,540,650, 
	                       9,530,650,10,430,650);Delayms(15);
}

void draw_back()       //后退
{
   six_sevro( 4,470,650, 5,480,650, 6,450,650,
	           10,510,650,11,450,650,12,280,650);Delayms(15);
	 six_sevro( 4,470,650, 5,450,650, 6,460,650, 
	           10,510,650,11,490,650,12,270,650);Delayms(15);
	 six_sevro( 1,580,650, 2,490,650, 3,650,650,
	            7,540,650, 8,580,650, 9,450,650);Delayms(15);
	 six_sevro( 1,550,650, 2,512,650, 3,650,650,
	            7,560,650, 8,560,650, 9,450,650);Delayms(15);
  four_sevro( 3,570,650, 4,550,650, 
	                       9,530,650,10,430,650);Delayms(15);
}


