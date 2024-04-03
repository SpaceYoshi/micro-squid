#ifndef RGB_LED_H_
#define RGB_LED_H_

#include <stdint.h>

void rgb_led_init(void);

void rgb_led_set_clr(uint8_t duty_cycle_red, uint8_t duty_cycle_green);

#endif /* RGB_LED_H_ */