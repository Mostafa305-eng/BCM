/*
 * TMU.h
 *
 * Created: 1/11/2020 6:21:48 PM
 *  Author: Mostafa Metwaly
 */ 

#ifndef TMU_H_
#define TMU_H_




/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/


#include "std_types.h"
#include "common_macros.h"


/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/
#define TIMER_ID0 0
#define TIMER_ID1 1
#define TIMER_ID2 2

#define ONE_SHOT 0
#define PERIODIC 1



typedef struct str_TMU_cfg_t
{
	uint8 u8_TimerId;
	uint16 u16_Resolution;
	}str_TMU_cfg_t; 



/************************************************************************/
/*						FUNCTIONS' PROTOTYPES		        */
/************************************************************************/



/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 	cautions : if you choose timer 0 or 2 you have only range 1:16ms 
 * it is recommended to use timer 1 , it supports 1ms:4sec
 **************************************************************************/

ERROR_STATUS TMU_Init(void);







/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: call the functions when their time have come .
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Dispatcher( void );






/******************************************************************************
 * fun
 * Input:  
 *		----> Func_ptr	: pointer to function which required to excute
 *		----> period	: the time of the period between each call
 *		----> Repeat	: is this function will be repeated or one shot 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: start the time for the the entered function.
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Start_Timer( Ptrfunc Ptrfunc_Task , uint16 u16_Period ,uint8 u8_Repeating );







/******************************************************************************
 * fun
 * Input:  
 *		----> Func_ptr	: pointer to function which required to stopped
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: stop the time for the the entered function.
 * 							
 **************************************************************************/

ERROR_STATUS TMU_Stop_Timer( Ptrfunc Ptrfunc_Task  );





#endif /* TMU_H_ */