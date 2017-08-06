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

enum PowerStates { PowerOff, PowerOffPressed, PowerOn, PowerOnPressed } PowerState;
void TickPower()
{
	unsigned char button0 = (~PINA) & 0x01;
	
	switch (PowerState) // Power Transitions
	{
		case PowerOff:
			if (button0)
			{
				PowerState = PowerOffPressed;
			}
			else
			{
				PowerState = PowerOff;
			}			
			break;
		
		case PowerOffPressed:
			if (!button0)
			{
				PowerState = PowerOn;
				PWM_on();
			}
			else
			{
				PowerState = PowerOffPressed;
			}
			break;
			
		case PowerOn:
			if (button0)
			{
				PowerState = PowerOnPressed;
			}
			else
			{
				PowerState = PowerOn;
			}
			break;
			
		case PowerOnPressed:
			if (!button0)
			{
				PowerState = PowerOff;
				PWM_off();
			}
			else
			{
				PowerState = PowerOnPressed;
			}
			break;
			
		default:
			PowerState = PowerOff;
			break;
	}
	
	switch (PowerState) // Power Actions
	{
		case PowerOff:
			PORTB = 0x00;
			break;
		
		case PowerOffPressed:
			PORTB = 0x00;
			break;
			
		case PowerOn:
			PORTB = 0xFF;
			break;
			
		case PowerOnPressed:
			PORTB = 0xFF;
			break;
			
		default:
			PORTB = 0x00;
			break;
	}
}

const double C4 = 261.62;
const double D4 = 293.66;
const double E4 = 329.63;
const double F4 = 349.29;
const double G4 = 392.00;
const double A4 = 440.00;
const double B4 = 493.88;
const double C5 = 523.25;
#define frequencySize 8
unsigned int frequencyIterator = 0;
double frequencies[frequencySize] = {};
double freq;

enum IncrementDecrementStates { Init, IncrementPressed, DecrementPressed } IncrementDecrementState;
void TickIncrementDecrement()
{
	unsigned char button1 = (~PINA) & 0x02;
	unsigned char button2 = (~PINA) & 0x04;
	
	switch (IncrementDecrementState) // IncrementDecrement Transitions
	{
		case Init:
			if (button1 && !button2)
			{
				IncrementDecrementState = IncrementPressed;
			}
			else if (!button1 && button2)
			{
				IncrementDecrementState = DecrementPressed;
			}
			else
			{
				IncrementDecrementState = Init;
			}
			break;
		
		case IncrementPressed:
			if (!button1)
			{
				if (frequencyIterator < (frequencySize-1))
				{
					++frequencyIterator;
				}
				IncrementDecrementState = Init;
			}
			else
			{
				IncrementDecrementState = IncrementPressed;
			}
			break;
			
		case DecrementPressed:
			if (!button2)
			{
				if (frequencyIterator > 0)
				{
					--frequencyIterator;
				}
				IncrementDecrementState = Init;				
			}
			else
			{
				IncrementDecrementState = DecrementPressed;
			}
			break;
			
		default:
			IncrementDecrementState = Init;
			break;
	}
	
	freq = frequencies[frequencyIterator];
	set_PWM(freq);
}

void SetupFrequencies()
{
	frequencies[0] = C4;
	frequencies[1] = D4;
	frequencies[2] = E4;
	frequencies[3] = F4;
	frequencies[4] = G4;
	frequencies[5] = A4;
	frequencies[6] = B4;
	frequencies[7] = C5;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PowerState = PowerOff;
	IncrementDecrementState = Init;
	PWM_off();
	SetupFrequencies();
	
    while (1) 
    {
		TickPower();
		TickIncrementDecrement();
    }
}

