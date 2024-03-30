#include <avr/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 8e6
#define TRIG_PIN PD0
#define ECHO_PIN PD1
#define GREEN_LED PB0
#define RED_LED PB1
#define BUZZ_PIN PB2

void init_us100() {
  // Set trigger pin as output
  DDRD |= (1 << TRIG_PIN);
  // Set echo pin as input
  DDRD &= ~(1 << ECHO_PIN);
}

void init_leds() {
  // Set LED pins as output
  DDRB |= (1 << GREEN_LED) | (1 << RED_LED) | (1 << BUZZ_PIN);
}

void us100_trigger_pulse() {
  // Generate 10us pulse on trigger pin
  PORTD |= (1 << TRIG_PIN);
  _delay_us(10);
  PORTD &= ~(1 << TRIG_PIN);
}

uint16_t us100_get_dist_cm() {
  uint32_t pulse_width = 0;

  // Trigger pulse
  us100_trigger_pulse();

  // Measure pulse width, Wait for echo pin to go high
  while (!(PIND & (1 << ECHO_PIN)))
    ;

  while (PIND & (1 << ECHO_PIN))
    pulse_width++; // Measure pulse width

  // Convert pulse width to cm (divide by 58, as speed of sound is ~343 m/s)
  return (uint16_t)(pulse_width / 58);
}

void play_buzzer() {
  // Turn on buzzer
  PORTB |= (1 << BUZZ_PIN);
  _delay_ms(200); // Buzzer sound duration
  // Turn off buzzer
  PORTB &= ~(1 << BUZZ_PIN);
}

int main() {
  init_us100();
  init_leds();

  uint8_t state = 0; // 0: Red light, 1: Green light
  uint16_t prev_dist = 0;
  uint16_t curr_dist = 0;

  while (1) {
    // Measure distance
    curr_dist = us100_get_dist_cm();

    // Check if state needs to be changed
    if (state == 0 && curr_dist > prev_dist) {
      // Player moved during red light, eliminate
      play_buzzer();
      break;
    }

    // Update previous distance
    prev_dist = curr_dist;

    // Toggle between red and green light states
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
      _delay_ms(3000 + rand() % 3000); // Green light duration (3 to 6 seconds)
    }
  }

  while (1) {
    // Player eliminated, do nothing
  }

  return 0;
}
