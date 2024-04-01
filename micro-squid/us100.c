#include "util.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "us100.h"

void us100_init(void) {
  // Set trigger pin as output
  DDRD |= (1 << TRIG_PIN);
  // Set echo pin as input
  DDRD &= ~(1 << ECHO_PIN);
}

void us100_trigger_pulse(void) {
  // Generate 10us pulse on trigger pin
  PORTD |= (1 << TRIG_PIN);
  _delay_us(10);
  PORTD &= ~(1 << TRIG_PIN);
}

uint16_t us100_get_dist_cm(void) {
  uint32_t pulse_width = 0;

  // Trigger pulse
  us100_trigger_pulse();

  // Measure pulse width, Wait for echo pin to go high
  while (!(PIND & (1 << ECHO_PIN)))
    ;

  while (PIND & (1 << ECHO_PIN))
    pulse_width++;

  // Convert pulse width to cm
  return (uint16_t)(pulse_width / 58);
}
