#include "util.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "us100.h"
#include "digit_display.h"
#include "onboard_lcd.h"

#define GREEN_LED PB0
#define RED_LED PB1
#define BUZZ_PIN PB2
#define DETECT_DIST 450

// How much difference in distance to accept
// before registering it as movement.
#define MOVE_THRESHOLD 0

void init_leds() {
  // Set LED pins as output
  DDRB |= (1 << GREEN_LED) | (1 << RED_LED) | (1 << BUZZ_PIN);
}

void play_buzzer() {
  PORTB |= (1 << BUZZ_PIN);
  _delay_ms(200);
  PORTB &= ~(1 << BUZZ_PIN);
}

void handle_state(int state) {
  if (state == 0) {
    // Red light state
    PORTB |= (1 << RED_LED);
    PORTB &= ~(1 << GREEN_LED);

    state = 1;

    _delay_ms(3000);
  } else {
    // Green light state
    PORTB |= (1 << GREEN_LED);
    PORTB &= ~(1 << RED_LED);

    state = 0;

    // Green light duration (3 to 6 seconds)
	for (int i = 0; i < 3000 + rand() % 3000; i++) {
		_delay_ms(1);
	}
  }

}

int main() {
  us100_init();
  init_leds();

  // 0: Red light, 1: Green light
  uint8_t state = 1; 
  uint16_t prev_dist = 0;
  uint16_t curr_dist = 0;

  while (1) {
    curr_dist = us100_get_dist_cm();

    if (curr_dist > DETECT_DIST) continue;

    if (state == 0 && abs(curr_dist - prev_dist) > MOVE_THRESHOLD) {
      // Player moved during red light, eliminate
      play_buzzer();
      break;
    }

    // Update previous distance
    prev_dist = curr_dist;

    handle_state(state);
  }

  onboard_lcd_show_str("Bro literally died");

  PORTB &= ~BIT(GREEN_LED);
  PORTB &= ~BIT(RED_LED);

  while (1) {
    // Player eliminated
    PORTB &= ~BIT(GREEN_LED);
    PORTB |= BIT(RED_LED);
    _delay_ms(300);

    PORTB &= ~BIT(RED_LED);
    PORTB |= BIT(GREEN_LED);
    _delay_ms(300);
  }

  return 0;
}
