  #include "LPC11xx.h"
	#include <stdio.h>
	#include <string.h>
	#define UART_BPS 9600
	char GcRcvBuf[20];
	uint32_t ulADCData;
	void Delay(uint32_t ulTime)
	{
		uint32_t i;
		i=0;
		while(ulTime--)
		{
			for (i=0;i<5000;i++);
		}
	}
	void ADC_Init(void)
	{
		LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);
		LPC_IOCON->R_PIO0_11 &= ~0xBF;
		LPC_IOCON->R_PIO0_11 |= 0x02;
		LPC_SYSCON->PDRUNCFG &=~(0x01<<4);
		LPC_SYSCON->SYSAHBCLKCTRL |=(0x01<<13);
		LPC_ADC->CR=(0x01<<0)|
		((SystemCoreClock/1000000-1)<<8)|
		(0<<16)|
		(0<<17)|
		(0<<24)|
		(0<<27);
	}
	void UART_Init(void)
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON->PIO1_6&=~0x07;
	LPC_IOCON->PIO1_6|=(1<<0);
	LPC_IOCON->PIO1_7&=~0x07;
	LPC_IOCON->PIO1_7|=(1<<0);
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<12);
	LPC_SYSCON->UARTCLKDIV=0x01;
	LPC_UART->LCR=0x83;
	usFdiv=(SystemCoreClock/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;
	LPC_UART->DLM=usFdiv/256;
	LPC_UART->DLL=usFdiv%256;
	LPC_UART->LCR=0x03;
	LPC_UART->FCR=0x07;
} 
void LEDInit(void)
{
	LPC_GPIO2 ->DIR |= 0xFFF;	
}
void UART_SendByte(uint8_t ucDat)
{
	LPC_UART->THR=ucDat;
	while((LPC_UART->LSR&0x40)==0);
}
void UART_SendStr(char * pucStr)
{
	while(1)
	{
	if(*pucStr=='\0')break;
	UART_SendByte (*pucStr++);
	}
}	
		
int main(void)
{
	uint32_t i;
	uint32_t ulADCData;
	uint32_t ulADCBuf;
	UART_Init ();
	ADC_Init();
	LEDInit();

	while(1)
	{
		ulADCData=0;
		for(i=0;i<10;i++)
		{
			LPC_ADC->CR|=(1<<24);
			while((LPC_ADC->DR[0]&0x80000000)==0);
			LPC_ADC->CR|=(1<<24);
			while((LPC_ADC->DR[0]&0x80000000)==0);
			ulADCBuf=LPC_ADC->DR[0];
			ulADCBuf=(ulADCBuf>>6)&0x3ff;
			ulADCData+=ulADCBuf;
		}
	ulADCData=ulADCData/10;
		
		
		if (ulADCData>=100) 
	{		
	LPC_GPIO2->DATA&=~(1<<0);
	}
	else LPC_GPIO2->DATA|=(1<<0);

	if (ulADCData>=200) 
	{		
	LPC_GPIO2->DATA&=~(1<<1);
	}
	else LPC_GPIO2->DATA|=(1<<1);

	if (ulADCData>=300) 
	{		
	LPC_GPIO2->DATA&=~(1<<2);
	}
	else LPC_GPIO2->DATA|=(1<<2);

	if (ulADCData>=400) 
	{		
	LPC_GPIO2->DATA&=~(1<<3);
	}
	else LPC_GPIO2->DATA|=(1<<3);

	if (ulADCData>=500) 
	{		
	LPC_GPIO2->DATA&=~(1<<4);
	}
	else LPC_GPIO2->DATA|=(1<<4);

	if (ulADCData>=600) 
	{		
	LPC_GPIO2->DATA&=~(1<<5);
	}
	else LPC_GPIO2->DATA|=(1<<5);

	if (ulADCData>=700) 
	{		
	LPC_GPIO2->DATA&=~(1<<6);
	}
	else LPC_GPIO2->DATA|=(1<<6);

	if (ulADCData>=800) 
	{		
	LPC_GPIO2->DATA&=~(1<<7);
	}
	else LPC_GPIO2->DATA|=(1<<7);
	
	
	ulADCData=(ulADCData*3300)/1024;	

	sprintf(GcRcvBuf,"VINO=%4d mV\r\n",ulADCData);
	UART_SendStr(GcRcvBuf);
		
	

	Delay(200);
}
}