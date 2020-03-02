/*
 * SLEEP.h
 *
 * Created: 1/13/2020
 *  Author: Mostafa Metwaly
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_



/************************************************************************/
/*				 				INCLUDES						        */
/************************************************************************/

#include "std_types.h"

/************************************************************************/
/*								 DEFINES						        */
/************************************************************************/

#define IDLE				0
#define ADC_NOISE_REDUC		1
#define POWER_DOWN			2
#define POWER_SAVE			3
#define STANDBY				4
#define EXT_STANDBY			5



/************************************************************************/
/*						FUNCTIONS' PROTOTYPES					        */
/************************************************************************/



/*********************************************************************
* @brief: CPU_Sleep provides six modes
* 	  IDLE, ADC_NOISE_REDUC, POWER_DOWN, POWER_SAVE, STANDBY, EXT_STANDBY
* 	  Note: STANDBY and EXT_STANDBY sleep modes are only available with
* 	  	external crystal/resonator fuse selection
* @param: E_SleepMode_t defines sleep modes
* Input : E_SleepMode_t
* Output: None
* @return: void
*****************************************************************************/

void CPU_Sleep(uint8 u8_mode);


#endif /* COMMON_SLEEP_SLEEP_H_ */
