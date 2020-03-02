/*
 * UART.c
 *
 * Created: 12/28/2019 4:49:21 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************
*								 INCLUDES								*
************************************************************************/

#include "registers.h"
#include "interrupt.h"
#include "UART.h"








/************************************************************************
*								 defines								*
************************************************************************/




/*
*	Definition of UART register's bits
*/

/*CSRA*/
#define UART_MPCM  (0x01)
#define UART_U2X   (0x02)
#define UART_PE    (0x04)
#define UART_DOR   (0x08)
#define UART_FE    (0x10)
#define UART_DRE   (0x20)
#define UART_TXC   (0x40)
#define UART_RXC   (0x80)

/*CSRB*/
#define UART_TXB8  (0x01)
#define UART_RXB8  (0x02)
#define UART_CSZ2 (0x04)
#define UART_TXEN  (0x08)
#define UART_RXEN  (0x10)
#define UART_DRIE  (0x20)
#define UART_TXCIE (0x40)
#define UART_RXCIE (0x80)

/*CSRC*/
#define UART_CPOL  (0x01)
#define UART_CSZ0  (0x02)
#define UART_CSZ1  (0x04)
#define UART_SBS   (0x08)
#define UART_PM0   (0x10)
#define UART_PM1   (0x20)
#define UART_MSEL  (0x40)
#define UART_RSEL  (0x80)

/*
*	Used Pins
*/

#define UART_TX_GPIO					GPIOD
#define UART_TX_BIT						BIT1

#define UART_RX_GPIO					GPIOD
#define UART_RX_BIT						BIT0


#define UBBRL_MASK 0x00ff
#define UBBRH_MASK 0x0f00

#define NO_OF_UART_INT 3
#define RXC_INT		0
#define UDRE_INT	1
#define TXC_INT		2
 

/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/

uint8 u8_USCRC_Buffer ;
uint16 u16_BaudRateBuffer;

uint8 u8_InterruptMode;

static volatile Ptrfunc PTR_UART_CBKs[NO_OF_UART_INT] ;



/******************************************************************************************** 
 * \func name : UART_Init																	*			
 *	\input :pointer  to a structure contains the information needed to initialize the UART	*				
 * \																						*
 * \return ERROR_STATUS : Indication to the function execution								*
 *																							*
 *******************************************************************************************/     

extern ERROR_STATUS UART_Init(str_UART_cfg_t *pstr_UART_cfg)
{

	u8_USCRC_Buffer =0x80;
	u16_BaudRateBuffer=0;
	u8_InterruptMode=0;
	PTR_UART_CBKs[RXC_INT] = NULL;
	PTR_UART_CBKs[UDRE_INT] = NULL;
	PTR_UART_CBKs[TXC_INT] = NULL;
	
	/* clear the bits of tx and rx first before init it */
	CLR_BIT(UCSRB,(UART_TXEN|UART_RXEN));
	
	switch(pstr_UART_cfg->u8_DesiredOperation)
	{
		
		case TRANSMITTER :
			SET_BIT(UCSRB,UART_TXEN);
			break;
		case RECEIVER :
			SET_BIT(UCSRB,UART_RXEN);
			break;
		case TRANSCEIVER :
			SET_BIT(UCSRB,(UART_TXEN|UART_RXEN));
			break; 
		default :
			return E_NOK;
	}
	
	
	CLR_BIT(UCSRA,(UART_RXCIE|UART_TXCIE));
	
	switch(pstr_UART_cfg->u8_InterruptMode)
	{
		case UART_POLLING :
			CLR_BIT(UCSRA,(UART_RXCIE|UART_TXCIE));
			u8_InterruptMode=UART_POLLING;
			break;
		
		case UART_INTERRUPT:
			u8_InterruptMode=UART_INTERRUPT;
			switch(pstr_UART_cfg->u8_DesiredOperation)
			{
				case TRANSMITTER :
					SET_BIT(UCSRB,UART_TXCIE);//UART_TXCIE|UART_DRIE
					break;
				case RECEIVER :
					SET_BIT(UCSRB,UART_RXCIE);
					break;
				case TRANSCEIVER :
					SET_BIT(UCSRB,(UART_RXCIE|UART_TXCIE));//|UART_TXCIE|UART_DRIE
					break; 
				default :
					return E_NOK;
			}
			if ( NULL != pstr_UART_cfg->ptrF_Uart_ResceiveComplete_Cbk )
			{
				PTR_UART_CBKs[RXC_INT] = pstr_UART_cfg->ptrF_Uart_ResceiveComplete_Cbk  ;
			} 
			if(NULL != pstr_UART_cfg->ptrF_Uart_DataRegisterEmpty_Cbk)
			{
				PTR_UART_CBKs[UDRE_INT] = pstr_UART_cfg->ptrF_Uart_DataRegisterEmpty_Cbk  ;
			}
			if(NULL != pstr_UART_cfg->ptrF_Uart_TransmitComplete_Cbk)
			{
				PTR_UART_CBKs[TXC_INT] = pstr_UART_cfg->ptrF_Uart_TransmitComplete_Cbk  ;
			}
			break;
	}
	switch(pstr_UART_cfg->u8_DoubleSpeed)
	{
		CLR_BIT(UCSRA,UART_U2X);
		
		case UART_NO_DOUBLE_SPEED :
			CLR_BIT(UCSRA,UART_U2X);
			u16_BaudRateBuffer = (F_CPU /(16 * pstr_UART_cfg->u32_BaudRate ))-1;
			break;
			
		case UART_DOUBLE_SPEED :
			SET_BIT(UCSRA,UART_U2X) ;
			u16_BaudRateBuffer = (F_CPU /(8 * pstr_UART_cfg->u32_BaudRate ))-1;
			break;
		
		default:
			return E_NOK;	
	}
	switch (pstr_UART_cfg->u8_StopBit)
	{
		CLR_BIT(u8_USCRC_Buffer,UART_SBS);
		
		case UART_ONE_STOP_BIT:
			CLR_BIT(u8_USCRC_Buffer,UART_SBS);
			break;
			
		case UART_TWO_STOP_BIT :
			SET_BIT(u8_USCRC_Buffer,UART_SBS);
			break;
			
		default :
			return E_NOK;
	}
	switch (pstr_UART_cfg->u8_ParityBit)
	{
		CLR_BIT(u8_USCRC_Buffer,(UART_PM0 | UART_PM1));
		
		case UART_NO_PARITY:
			CLR_BIT(u8_USCRC_Buffer,(UART_PM0 | UART_PM1));
			break;
			
		case UART_EVEN_PARITY :
			SET_BIT(u8_USCRC_Buffer,UART_PM1);
			break;

		case UART_ODD_PARITY :
			SET_BIT(u8_USCRC_Buffer,(UART_PM0 | UART_PM1));
			break;
			
		default :
			return E_NOK;
	}
	switch(pstr_UART_cfg->u8_DataSize)
	{
		CLR_BIT(u8_USCRC_Buffer,(UART_CSZ0|UART_CSZ1));
		CLR_BIT(UCSRB,UART_CSZ2);
		
		case UART_5_BIT :
			CLR_BIT(u8_USCRC_Buffer,(UART_CSZ0|UART_CSZ1));
			break;

		case UART_6_BIT :
			SET_BIT(u8_USCRC_Buffer,UART_CSZ0);
			break;		

		case UART_7_BIT :
			SET_BIT(u8_USCRC_Buffer,UART_CSZ1);
			break;
	
		case UART_8_BIT :
			SET_BIT(u8_USCRC_Buffer,(UART_CSZ0|UART_CSZ1));
			break;

		case UART_9_BIT :
			SET_BIT(UCSRB,UART_CSZ2);
			SET_BIT(u8_USCRC_Buffer,(UART_CSZ0|UART_CSZ1));
			break;
		
		default :
			return E_NOK;
	}
	
	/*write in USCRC*/
	UBRRH = u8_USCRC_Buffer;   
	/*write in UBRRL*/
	UBRRL = (u16_BaudRateBuffer & UBBRL_MASK);
	UBRRH = ((u16_BaudRateBuffer & UBBRH_MASK)>>8);
	
	return E_OK;
}

/************************************************************************************** 
 * \brief  UART_SendByte : Sends a certain byte
 *
 * \param u8_Data : The byte the user wants to send
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **************************************************************************************/  
extern ERROR_STATUS UART_SendByte(uint8 u8_Data)
{
	
	switch (u8_InterruptMode)
	{
	case UART_POLLING :
		while(0 == GET_BIT(UCSRA,UART_DRE));
		UDR = u8_Data;
		break;
	case UART_INTERRUPT :
		
		UDR = u8_Data;
		break;
	default:
		return E_NOK;
	}

	return E_OK;
}


/************************************************************************************** 
 * \brief  UART_ReceiveByte : Receives a certain byte
 *
 * \param pu8_ReceivedData : The sent byte will be stored in the passed pointer 
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **************************************************************************************/  
extern ERROR_STATUS UART_ReceiveByte(uint8 *pu8_ReceivedData)
{
	switch (u8_InterruptMode)
	{
	case UART_POLLING :
		while(0 == GET_BIT(UCSRA,UART_RXC));
		*pu8_ReceivedData = UDR;
		break;
	case UART_INTERRUPT :
		*pu8_ReceivedData = UDR;
		break;
	default:
		return E_NOK;
	}
	
	return E_OK;
}


/************************************************************************
*					ISR FUNCTIONS IMPLEMENTATION				        *

************************************************************************/



ISR(USART_RXC_vect)
{
			if ( NULL != PTR_UART_CBKs[RXC_INT] )
			{
				PTR_UART_CBKs[RXC_INT] ()  ;
			}
}

ISR(USART_UDRE_vect)
{
	
	if ( NULL != PTR_UART_CBKs[UDRE_INT] )
	{
		PTR_UART_CBKs[UDRE_INT] ()  ;
	}
	CLR_BIT(UCSRB,UART_DRIE);
}

ISR(USART_TXC_vect)
{
	if ( NULL != PTR_UART_CBKs[TXC_INT] )
	{
		PTR_UART_CBKs[TXC_INT] ()  ;
	}
}
