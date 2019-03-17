#ifndef _CDS5516_H_
#define _CDS5516_H_

//头文件
#include <stc15f2k60s2.h>
#include <intrins.h>

//宏定义
#define FOSC       20000000L    //系统频率    
#define BAUD1      115200       //串口频率
#define TM1        (65536-(FOSC/4/BAUD1))
#define uchar      unsigned char
#define uint       unsigned int	
#define MAX        40

//常用指令定义
#define PING       0x01  //查询指令
#define READ_DATA  0x02  //读指令
#define WRITE_DATA 0x03  //写指令
#define REG_WRITE  0x04  //异步写指令
#define ACTION     0x05  //执行异步写指令
#define REST       0x06  //复位指令
#define SYNC_WRITE 0x83  //同步写指令

//常用内存控制表地址定义
#define EEPROM_ID  0x03  //ID在内存中的地址
#define EEPROM_BPS 0x04  //BPS（波特率）在内存中的地址

//舵机常用波特率定义
#define BPS_1M     0x01  //波特率为1000000 误差0%
#define BPS_500K   0x03  //波特率为500000  误差0%
#define BPS_250K   0x07  //波特率为250000  误差0%
#define BPS_115200 0x10  //波特率为115200  误差-2.124%
#define BPS_57600  0x22  //波特率为57600   误差0.794%
#define BPS_19200  0x67  //波特率为19200   误差-0.160%

//全局变量声明
extern uchar rece1[MAX];              //需要接收的数据
 
//外部函数声明
extern void Delayms(uchar N);		           //@20.000MHz
extern void Uart1_Init(void);                  //串口1初始化
extern void change_ID( uchar OID, uchar NID ); //改舵机ID号 OID为旧ID  NID为新ID
extern void change_BPS(uchar ID,  uchar BPS);  //修改舵机的波特率
extern void reset_servo(void);                     //复位
extern void go_forward(void);                      //前进
extern void turn_left(void);                       //左转
extern void turn_right(void);                      //右转
extern void draw_back(void);                       //后退

#endif
