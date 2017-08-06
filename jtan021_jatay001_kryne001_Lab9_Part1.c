/*
 * jtan021_jatay001_kryne001_Lab9.c
 *
 * Created: 2/19/2017 11:07:51 PM
 * Author : jonat
 */ 

#include <avr/io.h>

//NOTE*** THIS NEW CODE TARGETS PB6 NOT PB3
void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {Start, Init} State;
const double C4 = 261.62;
const double D4 = 293.66;
const double E4 = 329.63;
double freq;

void Tick()
{
	unsigned char button0 = ~PINA & 0x01;
	unsigned char button1 = ~PINA & 0x02;
	unsigned char button2 = ~PINA & 0x04;
	
	switch(State)
	{
		case Start:
			State = Init;
			freq = 0;
			break;
			
		case Init:
			if (button0 && !button1 && !button2)
			{
				freq = C4;
			}
			else if (!button0 && button1 && !button2)
			{
				freq = D4;
			}
			else if (!button0 && !button1 && button2)
			{
				freq = E4;
			}
			else
			{
				freq = 0;	
			}
			break;
			
		default:
			State = Init;
			freq = 0;
			break;
	}
	
	set_PWM(freq);
	
	if (freq > 0)
	{
		PORTB = 0x40;
	}
	else
	{
		PORTB = 0x00;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	State = Start;
	freq = 0;
	PWM_on();
	
    while (1) 
    {
		Tick();
    }
}

