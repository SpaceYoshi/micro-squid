#include <avr/io.h>
#include "digit_display.h"
#include "util.h"

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define SPI_SCK 1
#define SPI_MOSI 2
#define SPI_MISO 3
#define SPI_SS 0

void select_slave(void) {
	PORT_SPI &= ~BIT(SPI_SS);
}

void deselect_slave(void) {
	PORT_SPI |= BIT(SPI_SS);
}

void write_to_addr(unsigned char address, unsigned char data) {
	select_slave();

	SPDR = address;
	while (!(SPSR & BIT(SPIF)));

	SPDR = data;
	while (!(SPSR & BIT(SPIF)));

	deselect_slave();

	// TODO: needs wait?
}

void spi_master_init(void) {
	// Set MOSI, SCK, SS and SS_display to be output
	DDR_SPI = 0xFF;

	// Set MISO to be input
	DDR_SPI &= ~BIT(SPI_MISO);

	deselect_slave();

	// Disable SPI interrupts (bit 7),
	// enable SPI (bit 6),
	// disable data order (bit 5),
	// set mode to master (bit 4),
	// CPOL and CPHA low (bit 3, 2),
	// set SCK frequency prescaler to 64 (bitrate = 125 kHz)
	SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1);
}

void digit_display_init(void) {
	spi_master_init();
	
	// Register decode mode, BCD mode for all digits
	write_to_addr(0x09, 0xFF);

	// Register intensity, full intensity
	write_to_addr(0x0A, 0x0F);

	// Register scan limit, scan addresses 0 up to and including 3
	write_to_addr(0x0B, 0x03);

	// Register shutdown, normal operation
	write_to_addr(0x0C, 0x01);

	// Clear digits
	for (char i = 1; i < 4; i++) {
		write_to_addr(i, 0);
	}
}

void digit_display_set_num(int num) {
	// Positive numbers with more than 4 digits and negative
	// numbers with more than 3 digits cannot be displayed.
	if (num > 9999 || num < -999) return;

	if (num >= 0) {
		// Digit address 4 will be used to display thousands
		int thousands = num / 1000;
		write_to_addr(4, thousands);
	} else {
		// Digit address 4 will be used to display the sign '-'.
		num = abs(num);

		// Don't ask me why, but the character 'j' evaluates to
		// '-' on the LED display.
		write_to_addr(4, 'j');
	}

	// Split num up into individual digits
	int hundreds = (num % 1000) / 100;
	int tens = (num % 100) / 10;
	int units = num % 10;
	
	write_to_addr(1, units);
	write_to_addr(2, tens);
	write_to_addr(3, hundreds);
}
