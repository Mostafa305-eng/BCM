/*
 * TMU.c
 *
 * Created: 1/11/2020 6:21:59 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/
#include "TMU.h"
#include "Timer.h"
#include "TMU_CONFG.h"
#include "TMU_LCONFG.h"
#include "std_types.h"
#include "retval.h"
#include "common_macros.h"








/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define READY	1
#define WAITING	0

#define FOUND		1
#define NOTFOUND	0

#define STARTZERO	0

#define COUNTING	5
#define NOTCOUNTING	6

#define INIT		1
#define NOTINIT		0
#define CONV_FROM_MILLIS_TO_MICROS  1000




/************************************************************************/
/*						Structures Definitions					        */
/************************************************************************/
typedef struct str_TMU_info_t {
	Ptrfunc Ptrfunc_Task; 
	uint16 u16_Period;
	uint16 u16_FuncTimeCounter;
	uint8 u8_Repeating;
	uint8 u8_ReadyFlag;
	}str_TMU_info_t ;




/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/

static str_TMU_info_t gas_TMU_list[TMU_MAX_SIZE];
static uint8 u8_listCounter;
static uint8 u8_NoTimerTicks;
str_Timer_cfg_t str_Timer_cfg;
static uint8 u8_CountingState;
static uint8 u8_InitState;




/************************************************************************/
/*						FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/



/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: it will be executed in ISR of timer . it compares time counter of each 
 *				function after increasing it with the its u16_Period to check if it is ready
 *				or not 
 * 							
 **************************************************************************/
void TMU_Tick_CBK(void)
{
	//uint8 u8_loopCounter=0;

	u8_CountingState=COUNTING;	

		
}






/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 **************************************************************************/




ERROR_STATUS TMU_Init(void)
{
	uint8 u8_retval=TMU_BASE_ERR+SUCCESS;
	/*initialize the static variables*/
	u8_InitState=INIT;
	u8_NoTimerTicks=STARTZERO;
	u8_listCounter=STARTZERO;
	u8_CountingState=NOTCOUNTING;
	
	/*initialize the timer*/
	str_Timer_cfg.Timer_CH_NO=gstr_TMU_cfg.u8_TimerId;
	str_Timer_cfg.Timer_Mode=TIMER_MODE;
	str_Timer_cfg.Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
	str_Timer_cfg.Timer_Prescaler=TIMER_PRESCALER_1024;
	str_Timer_cfg.ptrf_Timer_Cbk =TMU_Tick_CBK ;
	
	Timer_Init(&str_Timer_cfg);
	
	/*calculate number of ticks which the timer will count*/
	uint8 u8_OneTickTime;
	Timer_GetTickTime(str_Timer_cfg.Timer_CH_NO,&u8_OneTickTime);
	u8_NoTimerTicks=(gstr_TMU_cfg.u16_Resolution*(CONV_FROM_MILLIS_TO_MICROS))/u8_OneTickTime;
	 	
	return u8_retval;
}





/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: call the functions when their time have come .
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Dispatcher( void )
{
	uint8 u8_loopCounter;

		/*counting flag will check if the ISR came or not */
	if (u8_CountingState==COUNTING)
	{
		for (u8_loopCounter=0 ; u8_loopCounter<u8_listCounter ; u8_loopCounter++)
		{
			/*increase the counters of each funticon with the unit time which is the 
			resolution the user entered*/
			gas_TMU_list[u8_loopCounter].u16_FuncTimeCounter += gstr_TMU_cfg.u16_Resolution;
			
			/*check if the time of each function is came or not & if it is true make 
			the flag ready to execute the function*/
			if (gas_TMU_list[u8_loopCounter].u16_FuncTimeCounter == gas_TMU_list[u8_loopCounter].u16_Period)
			{
				gas_TMU_list[u8_loopCounter].u8_ReadyFlag=READY;
			}
			else
			{		
			}
		}
		/*reset the flag of the isr*/
		u8_CountingState=NOTCOUNTING;
	}




	for (u8_loopCounter=0;u8_loopCounter<u8_listCounter;u8_loopCounter++)
	{
		/*excute the funtion if it is ready & after executing it make the flag waiting and reset 
		the counter of the flag*/
		if (gas_TMU_list[u8_loopCounter].u8_ReadyFlag==READY)
		{
			gas_TMU_list[u8_loopCounter].Ptrfunc_Task();
			gas_TMU_list[u8_loopCounter].u8_ReadyFlag=WAITING;
			gas_TMU_list[u8_loopCounter].u16_FuncTimeCounter=STARTZERO;
			/*if the function is one shot , delete it by overwriting the last function in 
			the same place and decreasing the number of function counter "listCounter" */
			if (gas_TMU_list[u8_loopCounter].u8_Repeating==ONE_SHOT)
			{
				u8_listCounter--;
				gas_TMU_list[u8_loopCounter] =gas_TMU_list[u8_listCounter];
				u8_loopCounter--;
			}
			else
			{
			}
		}
		else
		{
		}
		/*if is the last exist function is one shot , stop the timer*/

	}
			if (u8_listCounter==0)
			{
				Timer_Stop(str_Timer_cfg.Timer_CH_NO);
			}
			else
			{
			}
	return (TMU_BASE_ERR+SUCCESS);
}







/******************************************************************************
 * fun
 * Input:  
 *		----> Ptrfunc_Task	: pointer to function which required to excute
 *		----> u16_Period	: the time of the u16_Period between each call
 *		----> Repeat	: is this function will be repeated or one shot 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: start the time for the the entered function.
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Start_Timer( Ptrfunc Ptrfunc_Task , uint16 u16_Period ,uint8 u8_Repeating )
{
	uint8 u8_retval=TMU_BASE_ERR+SUCCESS;
	
	if (u8_InitState==NOTINIT)
	{
		u8_retval=TMU_BASE_ERR+NOT_INITIALIZED_ERR;
	}
	/*check if the number of functions reaches the max or not*/
	else if (u8_listCounter == TMU_MAX_SIZE)
	{
		u8_retval=TMU_BASE_ERR+BUFFER_FULL_ERR;
	}
	else if (NULL == Ptrfunc_Task)
	{
		u8_retval=TMU_BASE_ERR+NULL_PTR_ERR;
	}
	else
	{
		/*save the parameter in the struct*/
		gas_TMU_list[u8_listCounter].Ptrfunc_Task=Ptrfunc_Task;
		gas_TMU_list[u8_listCounter].u16_Period=u16_Period;
		gas_TMU_list[u8_listCounter].u8_Repeating=u8_Repeating;
		gas_TMU_list[u8_listCounter].u8_ReadyFlag=WAITING;
		gas_TMU_list[u8_listCounter].u16_FuncTimeCounter=STARTZERO;
		/*if this is the first function , start the timer */
		if (u8_listCounter==0)
		{
			Timer_Start(str_Timer_cfg.Timer_CH_NO,u8_NoTimerTicks);
		}
		else
		{
		}
		u8_listCounter++;
	}
	return u8_retval;
}

/******************************************************************************
 * fun
 * Input:  
 *		----> Ptrfunc_Task	: pointer to function which required to stopped
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: stop the time for the the entered function.
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Stop_Timer( Ptrfunc Ptrfunc_Task  )
{
	uint8 u8_loopCounter;
	uint8 u8_foundFlag=NOTFOUND;
	
	uint8 u8_retval=TMU_BASE_ERR+SUCCESS;
	
	
	if (NULL == Ptrfunc_Task)
	{
		u8_retval=TMU_BASE_ERR+NULL_PTR_ERR;
	}
	else
	{
		/*search for the required function by its name to stop*/
		for (u8_loopCounter=0;u8_loopCounter<u8_listCounter;u8_loopCounter++)
		{
			if (gas_TMU_list[u8_loopCounter].Ptrfunc_Task==Ptrfunc_Task)
			{
				u8_listCounter--;
				u8_foundFlag=FOUND;
				break;
			}
			else
			{
			}
		}

		if (u8_foundFlag==FOUND)
		{
			gas_TMU_list[u8_loopCounter] = gas_TMU_list[u8_listCounter];
			/*if it is the last element stop the timer*/
			if (u8_listCounter==0)   
			{
				Timer_Stop(str_Timer_cfg.Timer_CH_NO);
			}
			else
			{	
			}
		} 
		else
		{
			u8_retval=TMU_BASE_ERR+INVALID_ARGU_ERR;
		}
	
	}
	return u8_retval;
}

