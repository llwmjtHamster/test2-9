#include "LPC11xx.H"
//uint32_t interval;
char count;
void PIOINT3_IRQHandler()
{
	
	if ((LPC_GPIO3 ->MIS & (1<<1))==(1<<1))   //�ж��ǲ���GPIO3.1������ж�
	{
		count++;
		LPC_TMR32B1->MR1 = ((SystemCoreClock/1000/2/10)*count+SystemCoreClock/2000);
		LPC_GPIO3->IC = (1<<1);  //���ж�  IC��GPIO�ж�����Ĵ������������GPIO�ܽ��ϵ��жϱ�־
	}
		if ((LPC_GPIO3 ->MIS & (1<<0))==(1<<0)) 	
	{
		count++;
		LPC_TMR32B1->MR1 = (SystemCoreClock/2000-(SystemCoreClock/1000/2/10)*count);
		LPC_GPIO3->IC = (1<<0);
	}
		if ((LPC_TMR32B1->MR1)>=SystemCoreClock/1000)

	{
			LPC_TMR32B1->MR1 = (SystemCoreClock/2000);
			if ((LPC_GPIO3 ->MIS & (1<<1))==(1<<1))   //�ж��ǲ���GPIO3.1������ж�
	   {
		   count++;
		   LPC_TMR32B1->MR1 = ((SystemCoreClock/1000/2/10)*count+SystemCoreClock/2000);
		   LPC_GPIO3->IC = (1<<1);  //���ж�  IC��GPIO�ж�����Ĵ������������GPIO�ܽ��ϵ��жϱ�־
	   }
		  if ((LPC_GPIO3 ->MIS & (1<<0))==(1<<0)) 	
	   {
		   count++;
		   LPC_TMR32B1->MR1 = (SystemCoreClock/2000-(SystemCoreClock/1000/2/10)*count);
		   LPC_GPIO3->IC = (1<<0);
	   }
	}

		if (LPC_TMR32B1->MR1<=0)

		{
			LPC_TMR32B1->MR1 = (SystemCoreClock/2000);
			if ((LPC_GPIO3 ->MIS & (1<<1))==(1<<1))   //�ж��ǲ���GPIO3.1������ж�
	  {
		   count++;
		   LPC_TMR32B1->MR1 = ((SystemCoreClock/1000/2/10)*count+SystemCoreClock/2000);
		   LPC_GPIO3->IC = (1<<1);  //���ж�  IC��GPIO�ж�����Ĵ������������GPIO�ܽ��ϵ��жϱ�־
	  }
		  if ((LPC_GPIO3 ->MIS & (1<<0))==(1<<0)) 	
	  {
		   count++;
		   LPC_TMR32B1->MR1 = (SystemCoreClock/2000-(SystemCoreClock/1000/2/10)*count);
		   LPC_GPIO3->IC = (1<<0);
	  }
    }
			

}
void delay()
{
    uint16_t i = 4000;
    while(i--);
}
 void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
	LPC_IOCON->R_PIO1_2 &= ~(0x07);
	LPC_IOCON->R_PIO1_2 |= 0x03;
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
	LPC_TMR32B1->TCR = 0x02;
	LPC_TMR32B1->PR = 0;
	LPC_TMR32B1->MCR = 0x02<<9;
	LPC_TMR32B1->PWMC = 0x02;
	LPC_TMR32B1->MR1 = interval/2;
	LPC_TMR32B1->MR3 = interval;
	LPC_TMR32B1->TCR = 0x01;
}
void KEYInit(void)
{
	LPC_IOCON ->PIO3_1 &=~(0X07);  //��P3.1����ΪGPIO
	LPC_GPIO3 ->DIR    &=~(1<<0);  //��P3.1����Ϊ����
	LPC_IOCON ->PIO3_2 &=~(0X07);
	LPC_GPIO3 ->DIR    &=~(1<<1);
}
int main ()
{
	
	KEYInit();
	LPC_GPIO3 ->IE |=(1<<0);  //����P3.0�����ϵ��ж�
	LPC_GPIO3 ->IE |=(1<<1); 
	LPC_GPIO3 ->IS &= ~(1<<0);  //����P3.1����Ϊ���ش���
	LPC_GPIO3 ->IS &= ~(1<<1);
  LPC_GPIO3 ->IEV &= ~(1<<0);  //����P3.1Ϊ�����½��ش���
  LPC_GPIO3 ->IEV &= ~(1<<1); 
	NVIC_EnableIRQ(EINT3_IRQn );  //��GPIO3�ж�

	CT32B1_Init(SystemCoreClock/1000);
	while(1);
	
}
