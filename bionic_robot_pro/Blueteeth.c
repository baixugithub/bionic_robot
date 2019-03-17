#include "blueteeth.h"
uchar send2[MIN];  //��Ҫ���͵�ָ��
uchar rece2[MIN] ; //��Ҫ���յ�����
uchar rece;        //���յ����ַ�
uchar key;
void Uart2_Init(void)		//����2��ʼ��
{
	
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L   = TM2;	  //�趨��ʱ��ֵ
	T2H   = TM2>>8; //�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	IE2  &= ~0x01;  //��ֹ����2�ж�
	P_SW2|= 0x01;   //���ô���2�˿� RXD->P4.6  TXD->P4.7
}

void uart2_send(uchar *s2,uchar n)  //����2���ݷ��� 
{
	uchar i;
	for(i=0;i<n;i++)
  {
		S2BUF = s2[i];	                     // S2BUF����/���ͻ���
		while(!(((S2CON|0xFD)&0x02)>>1));    // �������ݴ���	
		S2CON&=~0x02;					               // ������ݴ��ͱ�־
	}
}

void uart2_rece(uchar n)     //����2���ݷ���
{
	uchar i;
	for(i=0;i<n;i++)
	{
		rece2[i]=S2BUF;                      //S2BUF����/���ͻ���
		while(!((S2CON|0xFE)&0x01));         // �������ݴ���	
		S2CON&=~0x01;	                       // ������ݴ����ձ�־
	}
}
void uchar_rece()     //����2���ݽ���
{
		rece=S2BUF;                          //S2BUF����/���ͻ���
		while(!((S2CON|0xFE)&0x01));         // �������ݴ���	
		S2CON&=~0x01;	                       // ������ݴ����ձ�־
}

void rece_pass()
{
			key=S2BUF;                      //S2BUF����/���ͻ���
			while(!((S2CON|0xFE)&0x01));         // �������ݴ���	
			S2CON&=~0x01;	                       // ������ݴ����ձ�־
}

