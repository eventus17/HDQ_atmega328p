#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <avr/power.h>

#include "hdq.h"

#define LED			(1<<PB0)
#define LED_DDR		DDRB
#define LED_PORT 	PORTB

#define LED_OFF		LED_PORT |= LED
#define LED_ON		LED_PORT &= ~LED
#define LED_TOG		LED_PORT ^= LED

/* 

PB2 -> HDQ line
PB0 -> LED

 */

//--------------------------------
// Domyślne F_CPU = 1.000.000 Hz
//--------------------------------

int main(void) {

	
	ADCSRA = 0;	//	ADC off

	
	sei();

	while(1) {

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

	}
}

//-----------------------------------------------------------------------------
// Przerwanie INT0_vect - generowane nawet jeśli PB2 ustawione jest jako wyjście
//-----------------------------------------------------------------------------
ISR(INT0_vect) { 

}