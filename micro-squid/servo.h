#ifndef SERVO_H_
#define SERVO_H_

#define SERVO_OBSERVING 50
#define SERVO_LOOKING_AWAY 1014

void servo_init(void);

void servo_turn(unsigned int pulse_len);

#endif /* SERVO_H_ */