/*
 * registers.h
 *
 *  Created on: Oct 21, 2019
 *      Author: Sprints
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include "std_types.h"

/*
 * General Registers
 */
#define MCUCR		*((reg_type8)0x55)
#define MCUCSR		*((reg_type8)0x54)
#define GICR		*((reg_type8)0x5B)
#define GIFR		*((reg_type8)0x5A)



/**
 * MCUCR bits
 */

#define ISC00	        (0x01)
#define ISC01	        (0x02)
#define ISC10	        (0x04)
#define ISC11	        (0x08)
#define SM0		(0x10)
#define SM1		(0x20)
#define SM2		(0x40)
#define SE		(0x80)

/**
 * MCUCSR bits
 */

#define PORF	        (0x01)
#define EXTRF	        (0x02)
#define BORF	        (0x04)
#define WDRF	        (0x08)
#define JTRF	        (0x10)
#define ISC2	        (0x40)
#define JTD	        (0x80)

/**
 * GICR bits
 */

#define IVCE	        (0x01)
#define IVSEL	        (0x02)
#define INT2	        (0x20)
#define INT0	        (0x40)
#define INT1	        (0x80)

/**
 * GIFR bits
 */

#define INTF2	        (0x20)
#define INTF0	        (0x40)
#define INTF1	        (0x80)

/*
 * PORTx registers
 */

#define PORTA_DATA 	*((reg_type8)0x3B)
#define PORTA_DIR  	*((reg_type8)0x3A)
#define PORTA_PIN	*((reg_type8)0x39)

#define PORTB_DATA 	*((reg_type8)0x38)
#define PORTB_DIR  	*((reg_type8)0x37)
#define PORTB_PIN	*((reg_type8)0x36)

#define PORTC_DATA 	*((reg_type8)0x35)
#define PORTC_DIR  	*((reg_type8)0x34)
#define PORTC_PIN	*((reg_type8)0x33)


#define PORTD_DATA 	*((reg_type8)0x32)
#define PORTD_DIR  	*((reg_type8)0x31)
#define PORTD_PIN	*((reg_type8)0x30)


/*
 * General Timer registers
 */
#define TIMSK		*((reg_type8)0x59)
#define TIFR		*((reg_type8)0x58)


/*
 * Timer 0 Registers
 */

#define TCCR0		(*((reg_type8)0x53))
#define TCNT0		(*((reg_type8)0x52))
#define OCR0		(*((reg_type8)0x5C))


/*
 * Timer 1 Registers
*/ 

#define TCCR1A		*((reg_type8)0x4f)
#define TCCR1B		*((reg_type8)0x4E)
#define	TCCR1		*((reg_type16)0x4E)
#define	TCNT1H		*((reg_type8)0x4D)
#define TCNT1L		*((reg_type8)0x4C)
#define	TCNT1		*((reg_type16)0x4C)
#define OCR1AH		*((reg_type8)0x4B)
#define	OCR1AL		*((reg_type8)0x4A)
#define	OCR1A		*((reg_type16)0x4A)
#define OCR1BH		*((reg_type8)0x49)
#define OCR1BL		*((reg_type8)0x48)
#define OCR1B		*((reg_type16)0x48)
#define ICR1H		*((reg_type8)0x47)
#define ICR1L		*((reg_type8)0x46)
#define ICR1		*((reg_type16)0x46)


/*
 * Timer 2 Registers
 */
#define TCCR2		*((reg_type16)0x45)
#define TCNT2		*((reg_type16)0x44)
#define OCR2		*((reg_type16)0x43)


/*
*UART registers
*/
#define UCSRC ( *((reg_type8) (0x40) ) )  /* Multiple use with USART_BRRH >> URSEL is set to choose which one will override */
#define UBRRH ( *((reg_type8) (0x40) ) )  /* Multiple use with USART_CSRC >> URSEL is set to choose which one will override */
#define UBRRL ( *((reg_type8) (0x29) ) )

#define UDR   ( *((reg_type8) (0x2c) ) )

#define UCSRA ( *((reg_type8) (0x2b) ) )
#define UCSRB ( *((reg_type8) (0x2a) ) )









#endif /* REGISTERS_H_ */
