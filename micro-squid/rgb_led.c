#include "util.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "rgb_led.h"

#define RED_PIN     PB4     // OC0
#define GREEN_PIN   PB7     // OC1B

void timer0_init() {
	// Set Fast PWM mode with non-inverted output on OC0 (Pin 11 on Arduino Mega)
	TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS00);
}

void timer2_init() {
	// Set Fast PWM mode with non-inverted output on OC2 (Pin 9 on Arduino Mega)
	TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (1 << CS20);
}

void rgb_led_init(void) {
	// Set RGB pins as outputs
	DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN);

	// Initialize timers for PWM
	timer0_init();
	timer2_init();
}

void rgb_led_set_clr(uint8_t duty_cycle_red, uint8_t duty_cycle_green) {
	OCR0 = duty_cycle_red;
	OCR2 = duty_cycle_green;
}
