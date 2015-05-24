/*
 * LED.h
 *
 * Created: 24.05.2015 23:28:24
 *  Author: ech0
 */ 


#ifndef LED_H_
#define LED_H_


#define LED0_PIO		PIOB
#define LED0_MASK		(0x01 << 27)



__INLINE static void led_config(void)
{
	/* Set up LED pins. */
	LED0_PIO->PIO_PER = LED0_MASK;
	LED0_PIO->PIO_OER = LED0_MASK;
	LED0_PIO->PIO_PUDR = LED0_MASK;
}

__INLINE void SwitchOnLED0( void )
{
	LED0_PIO->PIO_SODR = LED0_MASK;
}

__INLINE void SwitchOffLED0( void )
{
	LED0_PIO->PIO_CODR = LED0_MASK;
}



#endif /* LED_H_ */