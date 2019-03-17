#ifndef _BLUETEETH_H_
#define _BLUETEETH_H_

//ͷ�ļ�
#include <stc15f2k60s2.h>

//�궨��
#define FOSC       20000000L    //ϵͳƵ��    
#define BAUD2      9600         //����Ƶ��
#define TM2        (65536-(FOSC/4/BAUD2))
#define uchar      unsigned char
#define uint       unsigned int	
#define MIN        30

//ȫ�ֱ�������
extern uchar send2[MIN];  //��Ҫ���͵�ָ��
extern uchar rece2[MIN];  //��Ҫ���յ�����
extern uchar rece;        //���յ����ַ� 
extern uchar key;

//�ⲿ��������
extern void Uart2_Init(void);		          //����2��ʼ��
extern void uart2_send(uchar *s2,uchar n);//����2���ݷ��� 
extern void uart2_rece(uchar n);          //����1���ݷ���
extern void uchar_rece(void);              //����2���ݽ���
extern void rece_pass();

#endif
