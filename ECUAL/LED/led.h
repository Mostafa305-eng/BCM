/*
 * led.h
 *
 *  Created on: Oct 21, 2019
 *      Author: Sprints
 */

#ifndef LED_H_
#define LED_H_

/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/

#include "std_types.h"


/************************************************************************/
/*								 DEFINES						        */
/************************************************************************/


#define LED_0	0
#define LED_1	1
#define LED_2	2
#define LED_3	3



/************************************************************************/
/*			  Structures Definitions		        */
/************************************************************************/
typedef struct LED_cfg_s_t
	{
		uint8 LedId;
	}LED_cfg_s_t;


/************************************************************************/
/*						FUNCTIONS' PROTOTYPES					        */
/************************************************************************/



/**
 * Description: Initialize the LED_x (where x 0, 1, 2, 3) as output
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Init(LED_cfg_s_t *s_led_cfg);
/**
 * Description: Change the LED_x state (where x 0, 1, 2, 3) to be on
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_On(uint8 led_id);
/**
 * Description: Change the LED_x state (where x 0, 1, 2, 3) to be off
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Off(uint8 led_id);
/**
 * Description: Toggle the LED_x state (where x 0, 1, 2, 3)
 * @param led_id: The led to be initialized and it takes
 * 				  one of the enum (En_LedNumber_t) parameters
 *
 */
void Led_Toggle(uint8 led_id);



#endif /* LED_H_ */
