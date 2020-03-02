/*
 * BCM.c
 *
 * Created: 1/13/2020 4:43:13 PM
 *  Author: Mostafa Metwaly
 */ 


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/
#include "BCM.h"
#include "BCM_Cfg.h"
#include "retval.h"
#include "UART.h"

#include "led.h"





/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/



#define IDLE           (0)
#define Tx    		   (1)
#define Tx_Complete    (2)
#define Rx_Complete    (3)
#define Rx             (4)

#define UART_ID		'u'
#define SPI_ID		's'

#define INIT		1
#define NOTINIT		0
#define WORKING		2
#define NOTWORKING	3

#define UNLOCKED	5
#define LOCKED		6


#define BCM_SEND_ID					(0U)
#define BCM_SEND_SIZE_HIGH			(1U)
#define BCM_SEND_SIZE_LOW			(2U)
#define BCM_SEND_DATA				(3U)
#define BCM_SEND_CHECKSUM			(4U)
#define BCM_RECEIVE_ID				(5U)
#define BCM_RECEIVE_SIZE_HIGH		(6U)
#define BCM_RECEIVE_SIZE_LOW		(7U)
#define BCM_RECEIVE_DATA			(8U)
#define BCM_RECEIVE_CHECKSUM		(9U)


/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/
/*init state : to check the module if initialized or not
workingState : check in dispatcher on each peripheral is working or not
bcm_sm_state : represents the state machine of dispatcher
TranseivingState : represents which data i will send
u16_DataCounter:to count number of data received or sent*/

typedef struct BCM_ConfigType
{
	uint8	u8_BcmId;
	uint16	u16_DataSize;
	uint8	*pu8_Data;
	uint8	u8_CheckSum;
	uint8	u8_LockedState;
	uint8	u8_InitState;
	uint8	u8_WorkingState;
	uint8	u8_BCM_SM_State;
	uint8	u8_TranseivingState;
	uint16	u16_DataCounter;
}str_BCM_ConfigType;

/************************************************************************
*		                Global and static Variables                     *
************************************************************************/

static str_BCM_ConfigType str_BCM_Cfg[BCM_MAX_CH_NUM];

uint8 u8_tempReading;





/************************************************************************
*						 FUNCTIONS IMPLEMENTATION				        *
*************************************************************************/



/****************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is called in ISR and  changes the state machine 
 * of the dispatcher.
 * 							
 ******************************************************************************************/

static void BCM_SendComplelte_CBK(void)
{
	Led_Toggle(LED_3);
	if (str_BCM_Cfg[BCM_UART].u8_BCM_SM_State==Tx)
	{
		str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx_Complete;
	}
	else
	{	
	}
}

/****************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is called in ISR , reads the value from UART and changes the state machine 
 * of the dispatcher.
 * 							
 ******************************************************************************************/


static void BCM_ReceiveComplelte_CBK (void)
{
	if (str_BCM_Cfg[BCM_UART].u8_BCM_SM_State==Rx)
	{
		str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx_Complete;
		UART_ReceiveByte(&u8_tempReading);
	}	
}






/****************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the BCM module.
 * 							
 ******************************************************************************************/
ERROR_STATUS BCM_Init(void)
{
	u8_tempReading=0;
	uint8 u8_retval=BCM_BASE_ERR+SUCCESS;

	str_BCM_Cfg[BCM_UART].u8_BcmId=0;
	str_BCM_Cfg[BCM_UART].u16_DataSize=0;
	str_BCM_Cfg[BCM_UART].u8_CheckSum=0;
	str_BCM_Cfg[BCM_UART].pu8_Data=NULL;
	str_BCM_Cfg[BCM_UART].u8_LockedState=UNLOCKED;
	str_BCM_Cfg[BCM_UART].u8_InitState=INIT;
	str_BCM_Cfg[BCM_UART].u8_WorkingState=NOTWORKING;
	str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=IDLE;
	str_BCM_Cfg[BCM_UART].u16_DataCounter=0;
	
	
	str_BCM_Cfg[BCM_SPI].u8_BcmId=0;
	str_BCM_Cfg[BCM_SPI].u16_DataSize=0;
	str_BCM_Cfg[BCM_SPI].u8_CheckSum=0;
	str_BCM_Cfg[BCM_SPI].pu8_Data=NULL;
	str_BCM_Cfg[BCM_SPI].u8_LockedState=UNLOCKED;
	str_BCM_Cfg[BCM_SPI].u8_InitState=INIT;
	str_BCM_Cfg[BCM_SPI].u8_WorkingState=NOTWORKING;
	str_BCM_Cfg[BCM_SPI].u8_BCM_SM_State=IDLE;

	return u8_retval;
}




/**********************************************************************************************
 * Input: 
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts sending data through desired channel.
 * 							
 ************************************************************************************************/
ERROR_STATUS BCM_Send(uint8 u8_BCM_Type, uint16 u16_BCM_Size, uint8 *ptru8_BCM_Data)
{
	uint8 u8_retval=BCM_BASE_ERR+SUCCESS;

	switch (u8_BCM_Type)
	{
	case BCM_UART:
		if (str_BCM_Cfg[BCM_UART].u8_InitState == NOTINIT)
		{
			u8_retval=BCM_BASE_ERR+NOT_INITIALIZED_ERR ;		
		}
		else if (str_BCM_Cfg[BCM_UART].u8_LockedState==LOCKED)
		{
			u8_retval=BCM_BASE_ERR+RESOURCE_NOT_AVILABLE_ERR;
		} 
		else
		{
			str_BCM_Cfg[BCM_UART].u8_BcmId=UART_ID;
			str_BCM_Cfg[BCM_UART].u16_DataSize=u16_BCM_Size;
			str_BCM_Cfg[BCM_UART].u8_CheckSum=0;
			str_BCM_Cfg[BCM_UART].pu8_Data=ptru8_BCM_Data;
			str_BCM_Cfg[BCM_UART].u8_LockedState=LOCKED;
			str_BCM_Cfg[BCM_UART].u8_WorkingState=WORKING;	
			str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx_Complete;
			str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_ID;
			
			

			str_UART_cfg_t myUart={UART_INTERRUPT,TRANSMITTER,UART_DOUBLE_SPEED,UART_ONE_STOP_BIT\
			,UART_NO_PARITY,UART_8_BIT,9600,BCM_SendComplelte_CBK,NULL,NULL};
		
			UART_Init(&myUart);

		}
		break;
	case BCM_SPI:
		/*not Developed yet */
		u8_retval=BCM_BASE_ERR+RESOURCE_NOT_SUPPORTED_ERR;
	
	default:
		u8_retval=BCM_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
		break;
	}
	return u8_retval;
}



/**************************************************************************************************
 * Input: 
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts receiving data through desired channel.
 * 							
 **************************************************************************************************/
ERROR_STATUS BCM_Setup_Receive(uint8 u8_BCM_Type, uint16 u16_BCM_Size, uint8 *ptru8_BCM_Data)
{
	uint8 u8_retval=BCM_BASE_ERR+SUCCESS;
	
	switch (u8_BCM_Type)
	{
		case BCM_UART:
			if (str_BCM_Cfg[BCM_UART].u8_InitState == NOTINIT)
			{
				u8_retval=BCM_BASE_ERR+NOT_INITIALIZED_ERR ;
			}
			else if (str_BCM_Cfg[BCM_UART].u8_LockedState==LOCKED)
			{
				u8_retval=BCM_BASE_ERR+RESOURCE_NOT_AVILABLE_ERR;
			}
			else
			{
				str_BCM_Cfg[BCM_UART].u8_BcmId=UART_ID;
				str_BCM_Cfg[BCM_UART].u16_DataSize=u16_BCM_Size;
				str_BCM_Cfg[BCM_UART].u8_CheckSum=0;
				str_BCM_Cfg[BCM_UART].pu8_Data=ptru8_BCM_Data;
				str_BCM_Cfg[BCM_UART].u8_LockedState=LOCKED;
				str_BCM_Cfg[BCM_UART].u8_WorkingState=WORKING;
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx;
				str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_ID;
			
			

				str_UART_cfg_t myUart={UART_INTERRUPT,RECEIVER,UART_DOUBLE_SPEED,UART_ONE_STOP_BIT\
				,UART_NO_PARITY,UART_8_BIT,9600,NULL,BCM_ReceiveComplelte_CBK,NULL};
			
				UART_Init(&myUart);
				

			}
			break;
		case BCM_SPI:
			/*not Developed yet */
			u8_retval=BCM_BASE_ERR+RESOURCE_NOT_SUPPORTED_ERR;
		
		default:
			u8_retval=BCM_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
			break;
	}
	return u8_retval;
}









/*************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Tx.
 * 							
 **************************************************************************/
ERROR_STATUS BCM_TxDispatcher(void)
{
	
	uint8 u8_TempSizeData=0;
	uint8 u8_retval=BCM_BASE_ERR+SUCCESS;
	
	if (str_BCM_Cfg[BCM_UART].u8_WorkingState==WORKING)
	{
		switch (str_BCM_Cfg[BCM_UART].u8_BCM_SM_State)
		{

		case Tx_Complete :
			switch (str_BCM_Cfg[BCM_UART].u8_TranseivingState)
			{
			case BCM_SEND_ID:
				UART_SendByte(str_BCM_Cfg[BCM_UART].u8_BcmId);
				str_BCM_Cfg[BCM_UART].u8_CheckSum=0;
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx;
				str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_SIZE_HIGH;
				break;
			
			case BCM_SEND_SIZE_HIGH:
				u8_TempSizeData = (uint8)(str_BCM_Cfg[BCM_UART].u16_DataSize>>8);
				UART_SendByte(u8_TempSizeData);
				//UART_SendByte('h');
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx;
				str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_SIZE_LOW;
				break;
			
			case BCM_SEND_SIZE_LOW:
				u8_TempSizeData = (uint8)(str_BCM_Cfg[BCM_UART].u16_DataSize);
				UART_SendByte(u8_TempSizeData);
				//UART_SendByte('l');
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx;
				str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_DATA;
				break;
			
			case BCM_SEND_DATA:
				UART_SendByte(  str_BCM_Cfg[BCM_UART].pu8_Data[  str_BCM_Cfg[BCM_UART].u16_DataCounter   ]   );
				
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Tx;
				
				str_BCM_Cfg[BCM_UART].u8_CheckSum += \
						str_BCM_Cfg[BCM_UART].pu8_Data[  str_BCM_Cfg[BCM_UART].u16_DataCounter   ];
						
				
				if (str_BCM_Cfg[BCM_UART].u16_DataCounter < (str_BCM_Cfg[BCM_UART].u16_DataSize-1))
				{
					str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_DATA;
					str_BCM_Cfg[BCM_UART].u16_DataCounter++;
				} 
				else
				{
					str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_CHECKSUM;
					str_BCM_Cfg[BCM_UART].u16_DataCounter=0;					
				}
				
				break;
			
			
			case BCM_SEND_CHECKSUM:
				UART_SendByte(str_BCM_Cfg[BCM_UART].u8_CheckSum);
				str_BCM_Cfg[BCM_UART].u8_LockedState=UNLOCKED;
				str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=IDLE;			
				str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_SEND_ID; 
				break;
			}
			break;
		default:
			break;
		}
	}
	/*if (str_BCM_Cfg[BCM_SPI].u8_WorkingState==WORKING)
	{
		u8_retval=u8_retval=BCM_BASE_ERR+RESOURCE_NOT_SUPPORTED_ERR;
	}*/
	return u8_retval;
}

/****************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function unlocks the receiving ability for the desired channel.
 * 							
 ****************************************************************************************/
ERROR_STATUS BCM_RxUnlock(uint8 u8_BCM_Type)
{
	uint8 u8_retval=BCM_BASE_ERR+SUCCESS;
	switch (u8_BCM_Type)
	{
	case BCM_UART:
		str_BCM_Cfg[BCM_UART].u8_LockedState=UNLOCKED;
		break;
	default:
		u8_retval=BCM_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
		break;
	}
	return u8_retval;
}

/***********************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Rx.
 * 							
 ***********************************************************************/
ERROR_STATUS BCM_RxDispatcher(void)
{
		static uint16 u16_TempSizeData=0;
		uint8 u8_retval=BCM_BASE_ERR+SUCCESS;

		if (str_BCM_Cfg[BCM_UART].u8_WorkingState==WORKING)
		{
			switch (str_BCM_Cfg[BCM_UART].u8_BCM_SM_State)
			{
				case Rx_Complete :
					switch (str_BCM_Cfg[BCM_UART].u8_TranseivingState)
					{
						case BCM_RECEIVE_ID:
							if (str_BCM_Cfg[BCM_UART].u8_BcmId == u8_tempReading)
							{
								str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx;
								str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_SIZE_HIGH;
								str_BCM_Cfg[BCM_UART].u8_CheckSum=0;						
							}
							else
							{
								str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=IDLE;
								str_BCM_Cfg[BCM_UART].u8_WorkingState=NOTWORKING;
								str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_ID;							
								u8_retval=BCM_BASE_ERR+RESOURCE_NOT_AVILABLE_ERR;
							}
							break;
					
						case BCM_RECEIVE_SIZE_HIGH:
							u16_TempSizeData&=(uint16)(u8_tempReading<<8);
							str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx;
							str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_SIZE_LOW;
							break;
						case BCM_RECEIVE_SIZE_LOW:

							u16_TempSizeData|=(uint16)(u8_tempReading);
						
							if (str_BCM_Cfg[BCM_UART].u16_DataSize >=u16_TempSizeData)
							{
								str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx;
								str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_DATA;
							} 
							else
							{
								str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=IDLE;
								str_BCM_Cfg[BCM_UART].u8_WorkingState=NOTWORKING;
								u8_retval=BCM_BASE_ERR+RESOURCE_NOT_SUPPORTED_ERR;
							}

							break;
						case BCM_RECEIVE_DATA:
					
							str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=Rx;
					
							str_BCM_Cfg[BCM_UART].u8_CheckSum += u8_tempReading;
					
					
							if (str_BCM_Cfg[BCM_UART].u16_DataCounter < (u16_TempSizeData-1))
							{
								str_BCM_Cfg[BCM_UART].pu8_Data[  str_BCM_Cfg[BCM_UART].u16_DataCounter   ] = u8_tempReading;
								str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_DATA;
								str_BCM_Cfg[BCM_UART].u16_DataCounter++;
							}
							else
							{
								str_BCM_Cfg[BCM_UART].pu8_Data[  str_BCM_Cfg[BCM_UART].u16_DataCounter   ] = u8_tempReading;
								str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_CHECKSUM;
								str_BCM_Cfg[BCM_UART].u16_DataCounter=0;
							}
					
							break;
						case BCM_RECEIVE_CHECKSUM:
							if (str_BCM_Cfg[BCM_UART].u8_CheckSum != u8_tempReading)
							{	
								u8_retval=BCM_BASE_ERR+DATA_CORRUPTED_ERR;
							}
							else
							{
							}
							str_BCM_Cfg[BCM_UART].u8_BCM_SM_State=IDLE;
							str_BCM_Cfg[BCM_UART].u8_TranseivingState=BCM_RECEIVE_ID;
							break;
							
						default:
							break;
						}
						break;
				default:
					break;
			}
		}
		return u8_retval;
}
