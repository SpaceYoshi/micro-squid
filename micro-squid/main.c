#include "util.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "us100.h"
#include "digit_display.h"
#include "onboard_lcd.h"
#include "servo.h"

#define GREEN_LED PA1
#define RED_LED PA0
#define BUZZ_PIN PC0
#define DETECT_DIST 450

// How much difference in distance to accept
// before registering it as movement.
#define MOVE_THRESHOLD 5

// 0: Red light, 1: Green light
volatile uint8_t state = 1;
uint8_t prev_state = 1;
int prev_dist = 0;

ISR(TIMER3_OVF_vect) {
	TCNT3 = 65536 - 23438;

	state = (state == 0 ? 1 : 0);
}

void int_timer_init(void) {
	TCCR3B |= BIT(CS32) | BIT(CS30);
	TCNT3 = 65536 - 23438;
	ETIMSK |= BIT(TOIE3);
	sei();
}

void leds_init(void) {
  // Set LED pins as output
  DDRA |= (1 << GREEN_LED) | (1 << RED_LED);
}

void buzzer_init(void) {
	DDRC |= BIT(BUZZ_PIN);
}

void play_buzzer() {
  PORTC |= BIT(BUZZ_PIN);
  _delay_ms(200);
  PORTC &= ~BIT(BUZZ_PIN);
}

void switch_state() {
	if (state == 0) {
		if (prev_state != state) {
			prev_dist = -1;	
			servo_turn(SERVO_OBSERVING);
			prev_state = state;
		}

		// Red light state
		PORTA |= (1 << RED_LED);
		PORTA &= ~(1 << GREEN_LED);
	} else {
		if (prev_state != state) {
			servo_turn(SERVO_LOOKING_AWAY);
			prev_state = state;	
		}

		// Green light state
		PORTA |= (1 << GREEN_LED);
		PORTA &= ~(1 << RED_LED);
	}
}

int main() {
  us100_init();
  leds_init();
  buzzer_init();
  onboard_lcd_init();
  digit_display_init();
  int_timer_init();
  servo_init();

  uint16_t curr_dist = 0;

  while (1) {
    switch_state(state);

    curr_dist = us100_get_dist_cm();

    if (curr_dist > DETECT_DIST) continue;

	if (state == 0) {
		digit_display_set_num(curr_dist);
	}

    if (state == 0 && prev_dist != -1 && abs(curr_dist - prev_dist) > MOVE_THRESHOLD) {
      // Player moved during red light, eliminate
      play_buzzer();
      break;
    }

    // Update previous distance
    prev_dist = curr_dist;

  }

  onboard_lcd_set_str("yoU deth'd (x_X)");

  PORTA |= BIT(GREEN_LED);
  PORTA |= BIT(RED_LED);

  return 0;
}
