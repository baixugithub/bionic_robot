#ifndef _CDS5516_H_
#define _CDS5516_H_

//ͷ�ļ�
#include <stc15f2k60s2.h>
#include <intrins.h>

//�궨��
#define FOSC       20000000L    //ϵͳƵ��    
#define BAUD1      115200       //����Ƶ��
#define TM1        (65536-(FOSC/4/BAUD1))
#define uchar      unsigned char
#define uint       unsigned int	
#define MAX        40

//����ָ���
#define PING       0x01  //��ѯָ��
#define READ_DATA  0x02  //��ָ��
#define WRITE_DATA 0x03  //дָ��
#define REG_WRITE  0x04  //�첽дָ��
#define ACTION     0x05  //ִ���첽дָ��
#define REST       0x06  //��λָ��
#define SYNC_WRITE 0x83  //ͬ��дָ��

//�����ڴ���Ʊ��ַ����
#define EEPROM_ID  0x03  //ID���ڴ��еĵ�ַ
#define EEPROM_BPS 0x04  //BPS�������ʣ����ڴ��еĵ�ַ

//������ò����ʶ���
#define BPS_1M     0x01  //������Ϊ1000000 ���0%
#define BPS_500K   0x03  //������Ϊ500000  ���0%
#define BPS_250K   0x07  //������Ϊ250000  ���0%
#define BPS_115200 0x10  //������Ϊ115200  ���-2.124%
#define BPS_57600  0x22  //������Ϊ57600   ���0.794%
#define BPS_19200  0x67  //������Ϊ19200   ���-0.160%

//ȫ�ֱ�������
extern uchar rece1[MAX];              //��Ҫ���յ�����
 
//�ⲿ��������
extern void Delayms(uchar N);		           //@20.000MHz
extern void Uart1_Init(void);                  //����1��ʼ��
extern void change_ID( uchar OID, uchar NID ); //�Ķ��ID�� OIDΪ��ID  NIDΪ��ID
extern void change_BPS(uchar ID,  uchar BPS);  //�޸Ķ���Ĳ�����
extern void reset_servo(void);                     //��λ
extern void go_forward(void);                      //ǰ��
extern void turn_left(void);                       //��ת
extern void turn_right(void);                      //��ת
extern void draw_back(void);                       //����

#endif
