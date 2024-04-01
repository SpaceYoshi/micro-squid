#include <avr/io.h>
#include "onboard_lcd.h"

#define LCD_EN 3
#define LCD_RS 2

#define BIT(x) (1 << (x))

void lcd_send_byte(void) {
	PORTC |= (1 << LCD_EN);
	_delay_ms(1);
	PORTC &= ~(1 << LCD_EN);
	_delay_ms(1);
}

void onboard_lcd_send_cmd(unsigned char data) {
	PORTC = data & 0xF0;
	lcd_send_byte();
	
	PORTC = 0x04;
	PORTC = (data & 0xF0) << 4;
	lcd_send_byte();

	PORTC = 0x00;
}

void onboard_lcd_init(void) {
	DDRC = 0xFF;
	PORTC = 0x00;
	
	// Table 12 step 2
	PORTC = 0x20;
	lcd_send_byte();
	
	// Table 12 step 3
	PORTC = 0x20;
	lcd_send_byte();
	PORTC = 0x80;
	lcd_send_byte();
	
	// Table 12 step 4
	PORTC = 0x00;
	lcd_send_byte();
	PORTC = 0xF0;
	lcd_send_byte();

	// Table 12 step 5
	PORTC = 0x00;
	lcd_send_byte();
	PORTC = 0x60;
	lcd_send_byte();
	
	onboard_lcd_send_cmd(0x01);
}

void onboard_lcd_set_cursor(unsigned int pos) {
	onboard_lcd_send_cmd(0x80 + pos);
}

void onboard_lcd_show_str(char *str) {
	for (; *str; str++) {
		PORTC = *str & 0xF0;
		PORTC |= (1 << LCD_RS);
		lcd_send_byte();
		
		PORTC = (*str & 0x0F) << 4;
		PORTC |= (1 << LCD_RS);
		lcd_send_byte();
	}
}
