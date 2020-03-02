/*
 * DIO.c
 *
 * Created: 12/22/2019 9:46:58 PM
 *  Author: Mostafa Metwaly
 */ 


/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/
#include "registers.h"
#include "DIO.h"



/************************************************************************/
/*				  DIO FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/




/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/

ERROR_STATUS DIO_init (DIO_Cfg_st *DIO_info)
{
	switch(DIO_info->GPIO)
	{
		case GPIOA :
			if (DIO_info->dir)
			{
				SET_BIT(PORTA_DIR,DIO_info->pins);
			}
			else
			{
				CLR_BIT(PORTA_DIR,DIO_info->pins);
			}
			break;
		case GPIOB :
			if (DIO_info->dir)
			{
				SET_BIT(PORTB_DIR,DIO_info->pins);
			}
			else
			{
				CLR_BIT(PORTB_DIR,DIO_info->pins);
			}
			break;
		case GPIOC :
			if (DIO_info->dir)
			{
				SET_BIT(PORTC_DIR,DIO_info->pins);
			}
			else
			{
				CLR_BIT(PORTC_DIR,DIO_info->pins);
			}
			break;
		case GPIOD :
			if (DIO_info->dir)
			{
				SET_BIT(PORTD_DIR,DIO_info->pins);
			}
			else
			{
				CLR_BIT(PORTD_DIR,DIO_info->pins);
			}
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}





/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
		switch(GPIO)
	{
		case GPIOA :
			if (value)
			{
				SET_BIT(PORTA_DATA,pins);
			}
			else
			{
				CLR_BIT(PORTA_DATA,pins);
			}
			break;
		case GPIOB :
			if (value)
			{
				SET_BIT(PORTB_DATA,pins);
			}
			else
			{
				CLR_BIT(PORTB_DATA,pins);
			}
			break;
		case GPIOC :
			if (value)
			{
				SET_BIT(PORTC_DATA,pins);
			}
			else
			{
				CLR_BIT(PORTC_DATA,pins);
			}
			break;
		case GPIOD :
			if (value)
			{
				SET_BIT(PORTD_DATA,pins);
			}
			else
			{
				CLR_BIT(PORTD_DATA,pins);
			}
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}




/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT

*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8 GPIO,uint8 pins, uint8 *data)
{
		switch(GPIO)
	{
		case GPIOA :
			*data = GET_BIT(PORTA_PIN,pins);
			break;
		case GPIOB :
			*data = GET_BIT(PORTB_PIN,pins);
			break;
		case GPIOC :
			*data = GET_BIT(PORTC_PIN,pins);
			break;
		case GPIOD :
			*data = GET_BIT(PORTD_PIN,pins);
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8 GPIO, uint8 pins)
{
	switch (GPIO)
	{
		case GPIOA :
			TOGGLE_BIT(PORTA_DATA,pins);
			break;
		case GPIOB :
			TOGGLE_BIT(PORTB_DATA,pins);
			break;
		case GPIOC :
			TOGGLE_BIT(PORTC_DATA,pins);
			break;
		case GPIOD :
			TOGGLE_BIT(PORTD_DATA,pins);
			break;
		default: 
			return E_NOK;
			break;
	}
	return E_OK;
}


