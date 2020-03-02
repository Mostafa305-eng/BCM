/*
 * BCM.c
 *
 * Created: 1/13/2020 1:03:49 PM
 * Author : Mostafa Metwaly
 */ 

#include "interrupt.h"
#include "BCM.h"
#include "TMU.h"
#include "DIO.h"
#include "led.h"
#include "sleep.h"
#include "UART.h"

#define TRANSMIT		0
#define RECIEVE			1


#define TRANSITION		 TRANSMIT


void testfour()
{
	DIO_Toggle(GPIOA,BIT0);
}

void testfive()
{
	DIO_Toggle(GPIOA,BIT1);
}

#if TRANSITION == TRANSMIT




uint8 alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int main(void)
{	
	LED_cfg_s_t LED_cfg_s={LED_0};
	Led_Init(&LED_cfg_s);
	LED_cfg_s.LedId=LED_1;
	Led_Init(&LED_cfg_s);
	LED_cfg_s.LedId=LED_2;
	Led_Init(&LED_cfg_s);
	LED_cfg_s.LedId=LED_3;
	Led_Init(&LED_cfg_s);
	DIO_Cfg_st myDio_Cfg_s={GPIOA,FULL_PORT,HIGH};
	DIO_init(&myDio_Cfg_s);

	BCM_Init();
	TMU_Init();
			
	BCM_Send(BCM_UART, sizeof(alphabet)/sizeof(uint8), alphabet);
	sei();	
	TMU_Start_Timer(testfour,1,PERIODIC);
	TMU_Start_Timer(testfive,1,PERIODIC);

	while(1)
	{
		
		
		TMU_Dispatcher();
		BCM_TxDispatcher();
				
		CPU_Sleep(IDLE);
	}
}
#endif


#if TRANSITION == RECIEVE

uint8 alphabet[50] ;
int main(void)
{
	BCM_Init();
	TMU_Init();
	BCM_Setup_Receive(BCM_UART, 50, alphabet);
	sei();
		TMU_Start_Timer(testfour,1,PERIODIC);
		TMU_Start_Timer(testfive,1,PERIODIC);
	while(1)
	{
		TMU_Dispatcher();
		BCM_RxDispatcher();
		CPU_Sleep(IDLE);
		
	}
}
#endif
