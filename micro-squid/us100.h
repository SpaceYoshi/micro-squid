#ifndef US100_H_
#define US100_H_

#define TRIG_PIN PD0
#define ECHO_PIN PD1

#include <stdint.h>

void us100_init(void);

uint16_t us100_get_dist_cm(void);

#endif /* US100_H_ */
