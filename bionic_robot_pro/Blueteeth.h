#ifndef _BLUETEETH_H_
#define _BLUETEETH_H_

//头文件
#include <stc15f2k60s2.h>

//宏定义
#define FOSC       20000000L    //系统频率    
#define BAUD2      9600         //串口频率
#define TM2        (65536-(FOSC/4/BAUD2))
#define uchar      unsigned char
#define uint       unsigned int	
#define MIN        30

//全局变量声明
extern uchar send2[MIN];  //需要发送的指令
extern uchar rece2[MIN];  //需要接收的数据
extern uchar rece;        //接收单个字符 
extern uchar key;

//外部函数声明
extern void Uart2_Init(void);		          //串口2初始化
extern void uart2_send(uchar *s2,uchar n);//串口2数据发送 
extern void uart2_rece(uchar n);          //串口1数据发送
extern void uchar_rece(void);              //串口2数据接收
extern void rece_pass();

#endif
