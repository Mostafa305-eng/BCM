/*
 * UART.h
 *
 * Created: 12/20/2019 10:41:42 AM
 *  Author: Wave7
 */ 


#ifndef UART_H_
#define UART_H_


#include "std_types.h"
#include "common_macros.h"
#include "interrupt.h"
#include "registers.h"


/*
*	Used macros 
*/
#define UART_POLLING					(0U)
#define UART_INTERRUPT					(1U)

#define TRANSMITTER						(0U)
#define RECEIVER						(1U)
#define TRANSCEIVER						(2U)

#define UART_NO_DOUBLE_SPEED			(0U)
#define UART_DOUBLE_SPEED				(1U)

#define UART_ONE_STOP_BIT				(0U)
#define UART_TWO_STOP_BIT				(1U)

#define UART_NO_PARITY					(0U)
#define UART_EVEN_PARITY				(2U)
#define UART_ODD_PARITY					(3U)

#define UART_5_BIT						(0U)
#define UART_6_BIT						(1U)
#define UART_7_BIT						(2U)
#define UART_8_BIT						(3U)
#define UART_9_BIT						(7U)

					
typedef struct 
{
	uint8 u8_InterruptMode;
	uint8 u8_DesiredOperation;	
	uint8 u8_DoubleSpeed;
	uint8 u8_StopBit;
	uint8 u8_ParityBit;
	uint8 u8_DataSize;
	uint32 u32_BaudRate;
	volatile Ptrfunc ptrF_Uart_TransmitComplete_Cbk;
	volatile Ptrfunc ptrF_Uart_ResceiveComplete_Cbk;
	volatile Ptrfunc ptrF_Uart_DataRegisterEmpty_Cbk;
}str_UART_cfg_t;

/***********************************************************************************
 * \brief UART_Init : 
 *
 * \param *pUART_cfg_s : A pointer to a struct that holds the desired UART configuration
 * \return ERROR_STATUS : Indication to the function execution 
 *
 *************************************************************************************/     
extern ERROR_STATUS UART_Init(str_UART_cfg_t *pstr_UART_cfg);

/************************************************************************************* 
 * \brief UART_Deinit : Deinitialize the UART module
 *
 * \param void
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **************************************************************************************/     
extern ERROR_STATUS UART_Deinit(void);

/************************************************************************************** 
 * \brief  UART_SendByte : Sends a certain byte
 *
 * \param u8_Data : The byte the user wants to send
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **************************************************************************************/  
extern ERROR_STATUS UART_SendByte(uint8 u8_Data);

/************************************************************************************** 
 * \brief  UART_ReceiveByte : Receives a certain byte
 *
 * \param pu8_ReceivedData : The sent byte will be stored in the passed pointer 
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **************************************************************************************/  
extern ERROR_STATUS UART_ReceiveByte(uint8 *pu8_ReceivedData);
   
#endif /* UART_H_ */