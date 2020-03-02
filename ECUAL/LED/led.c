/*
 * CFile1.c
 *
 * Created: 12/3/2019 7:43:21 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/
#include "led.h"
#include "ledConfig.h"
#include "DIO.h"


void Led_Init(LED_cfg_s_t *s_led_cfg){
	DIO_Cfg_st DIO_Cfg_s;
	switch(s_led_cfg->LedId){
		case (LED_0) :
			DIO_Cfg_s.GPIO=LED_0_GPIO;
			DIO_Cfg_s.pins= LED_0_BIT;
			DIO_Cfg_s.dir= HIGH;
			break;	
		case (LED_1) :
			DIO_Cfg_s.GPIO=LED_1_GPIO;
			DIO_Cfg_s.pins= LED_1_BIT;
			DIO_Cfg_s.dir= HIGH;
			break;	
		case (LED_2) :
			DIO_Cfg_s.GPIO=LED_2_GPIO;
			DIO_Cfg_s.pins= LED_2_BIT;
			DIO_Cfg_s.dir= HIGH;
			break;	
		case (LED_3) :
			DIO_Cfg_s.GPIO=LED_3_GPIO;
			DIO_Cfg_s.pins= LED_3_BIT;
			DIO_Cfg_s.dir= HIGH;
			break;	
		default:
			break;
	}
	DIO_init(&DIO_Cfg_s);
	
}



void Led_On(uint8 led_id){
	switch(led_id){
		case (LED_0) :
			DIO_Write(LED_0_GPIO, LED_0_BIT , HIGH);
			break;
		case (LED_1) :
			DIO_Write(LED_1_GPIO, LED_1_BIT , HIGH);
			break;
		case (LED_2) :
			DIO_Write(LED_2_GPIO, LED_2_BIT , HIGH);
			break;
		case (LED_3) :
			DIO_Write(LED_3_GPIO, LED_3_BIT , HIGH);
			break;
	}	
}

void Led_Off(uint8 led_id){
	switch(led_id){
		case (LED_0) :
			DIO_Write(LED_0_GPIO, LED_0_BIT , LOW);
			break;
		case (LED_1) :
			DIO_Write(LED_1_GPIO, LED_1_BIT , LOW);
			break;
		case (LED_2) :
			DIO_Write(LED_2_GPIO, LED_2_BIT , LOW);
			break;
		case (LED_3) :
			DIO_Write(LED_3_GPIO, LED_3_BIT , LOW);
			break;
	}
}


void Led_Toggle(uint8 led_id){
	switch(led_id){
		case (LED_0) :
			DIO_Toggle(LED_0_GPIO, LED_0_BIT );
			break;
		case (LED_1) :
			DIO_Toggle(LED_1_GPIO, LED_1_BIT );
			break;
		case (LED_2) :
			DIO_Toggle(LED_2_GPIO, LED_2_BIT );
			break;
		case (LED_3) :
			DIO_Toggle(LED_3_GPIO, LED_3_BIT );
			break;
	}
}
	