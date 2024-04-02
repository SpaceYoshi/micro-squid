#define F_CPU 8e6

#include "util.h"
#include <avr/io.h>
#include <util/delay.h>
#include "servo.h"

void servo_init(void) {
	TCCR1A |= BIT(COM1A1) | BIT(WGM10);
	TCCR1B |= BIT(WGM12) | BIT(CS11) | BIT(CS10);

	DDRB |= BIT(PB5);
	
	OCR1A = SERVO_LOOKING_AWAY;
}

void servo_turn(uint16_t pulse_len) {
	OCR1A = pulse_len;
	_delay_ms(1500);
}
