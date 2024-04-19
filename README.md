# micro-squid
Game of Statues, also known as Red Light, Green Light using only the ATmega128 microcontroller and some peripherals. This was a 2-person project for the Microcontrollers subject of Computer Science (Avans University of Applied Sciences).

The game works with an ultrasonic sensor detecting if the player moves on an interval. The servo turns the sensor away on green light and toward the player on red light. The seven-segment displays show the current player's distance from the sensor. The buzzer and LCD screen are used when a player is eliminated (e.g. the player moved too much at red light). Advanced concepts such as timers and interrupts were used.

![Micro-squid setup](microsquid_setup.png)
