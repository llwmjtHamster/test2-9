#include "LPC11xx.H"//LPC1114ͷ�ļ�
uint8_t counter = 0;  //uint8��һ���ṹ�ı�ע��unit8_t������typede���һ������������һ���������ݱ�����ռ��һ�ֽڵ�
#define COM4_ON LPC_GPIO2->DATA &=~(1<<8)
#define COM3_ON LPC_GPIO2->DATA &=~(1<<9)
#define COM2_ON LPC_GPIO2->DATA &=~(1<<10)
#define COM1_ON LPC_GPIO2->DATA &=~(1<<11)
#define ALL_OFF LPC_GPIO2->DATA |=(1<<8)+(1<<9)+(1<<10)+(1<<11)
uint8_t table[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};                       
void PIOINT3_IRQHandler()
{
	
	if ((LPC_GPIO3 ->MIS & (1<<2))==(1<<2))   //�ж��ǲ���GPIO3.2������ж�
	{
		counter ++;
		LPC_GPIO3->IC = (1<<2);  //���ж�  IC��GPIO�ж�����Ĵ������������GPIO�ܽ��ϵ��жϱ�־
	}
	{
		if ((LPC_GPIO3 ->MIS & (1<<3))==(1<<3)) 
			counter ++;
	    LPC_GPIO3->IC = (1<<3);
	}
}
void delay()
{
    uint16_t i = 4000;
    while(i--);
}
void SEGInit(void)
{
	LPC_GPIO2 ->DIR |= 0XFFF;   //����P2.0~P2.7Ϊ���
	
}
void Display(int value)
{	
		LPC_GPIO2->DATA&=~(0XFF);
		LPC_GPIO2->DATA&=~(1<<8);
		LPC_GPIO2->DATA|=table [value/1000];
		delay();
		LPC_GPIO2->DATA|=(1<<8);
		//LPC_GPIO2->DATA=0xFF;
	
		LPC_GPIO2->DATA&=~(0XFF);
		LPC_GPIO2->DATA&=~(1<<9);
		LPC_GPIO2->DATA|=table [value%1000/100];
		delay();
		LPC_GPIO2->DATA|=(1<<9);
		//LPC_GPIO2->DATA=0xFF;
	
		LPC_GPIO2->DATA&=~(0XFF);
		LPC_GPIO2->DATA&=~(1<<10);
		LPC_GPIO2->DATA|=table [value%1000%100/10];
		delay();
		LPC_GPIO2->DATA|=(1<<10);
		//LPC_GPIO2->DATA=0xFF;
		
		LPC_GPIO2->DATA&=~(0XFF);
		LPC_GPIO2->DATA&=~(1<<11);
		LPC_GPIO2->DATA|=table [value%1000%100%10];
		delay();
		LPC_GPIO2->DATA|=(1<<11);
		//LPC_GPIO2->DATA=0xFF;
	}

void KEYInit(void)
{
	LPC_IOCON ->PIO3_1 &=~(0X07);  //��P3.1����ΪGPIO
	LPC_GPIO3 ->DIR    &=~(1<<2);  //��P3.1����Ϊ����
	LPC_IOCON ->PIO3_2 &=~(0X07);
	LPC_GPIO3 ->DIR    &=~(1<<3);

}

int main()
{
	SEGInit();
	KEYInit();
	LPC_GPIO2->DIR|=0xFFF;//?P2_0~P2_7?????
	LPC_GPIO2->DATA|=(1<<8);//?P2_8~p2_11??????
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO3 ->IE |=(1<<2);  //����P3.0�����ϵ��ж�
	LPC_GPIO3 ->IE |=(1<<3); 
	LPC_GPIO3 ->IS &= ~(1<<2);  //����P3.1����Ϊ���ش���
	LPC_GPIO3 ->IS &= ~(1<<3);
  LPC_GPIO3 ->IEV &= ~(1<<2);  //����P3.1Ϊ�����½��ش���
  LPC_GPIO3 ->IEV &= ~(1<<3); 
	NVIC_EnableIRQ(EINT3_IRQn );  //��GPIO3�ж�

	while(1)
  {
    	Display(counter);
	}
	

}