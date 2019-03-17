#include "CDS5516.h"
#include "Blueteeth.h"
void main()
{
	Uart1_Init();
	Delayms(10);
	Uart2_Init();
	Delayms(10);
	reset_servo();
	Delayms(50);
	while(1)
	{ 
		Delayms(50);
		uart2_send("Please enter a password ! ",26);
		key=0;
		Delayms(50);
		rece_pass();
		Delayms(10);
		if(key==49)
		{
			uart2_send("Enter password successfully ! ",29);
			while(1)
			{
				uchar_rece(); 
				switch (rece)
				{
					case 'A': go_forward(); uart2_send("Go Forward ! ",13);
						break;
					case 'B': turn_left();  uart2_send("Turn Left ! ",12);
						break;
					case 'C': turn_right(); uart2_send("Turn Right ! ",13);
						break;
					case 'D': draw_back();  uart2_send("Draw Back ! ",12);
						break;
					default: uart2_send("This a wrong ! ",15);
						break;
				}
			}
	 }
		else uart2_send("Enter the password is wrong ! ",30);
	}
}