/*
 * jtan021_jatay001_kryne001_lab4_part1.c
 *
 * Created: 1/20/2017 12:34:04 PM
 * Author: student
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
    while(1)
    {
		int value = (~PINA) & 0x0F;
		
		if (value >= 13)
		{
			PORTC = 0x3F;
		}
		else if (value >= 10)
		{
			PORTC = 0x3E;
		}
		else if (value >= 7)
		{
			PORTC = 0x3C;
		}
		else if (value >= 5)
		{
			PORTC = 0x38;
		}
		else if (value >= 3)
		{
			PORTC = 0x70;
		}
		else if (value >= 1)
		{
			PORTC = 0x60;
		}
		else
		{
			PORTC = 0x40;
		}
    }
}
