/*
 * jtan021_jatay001_kryne001_Lab9.c
 *
 * Created: 2/19/2017 11:07:51 PM
 * Author : jonat
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR (TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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

#define C4 261.62
#define D4 293.66
#define E4 329.63
#define F4 349.29
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25
#define PAUSE 0
#define frequencySize 41
unsigned int frequencyIterator = 0;
const double frequencies[frequencySize]
	= { A4, G4, F4, G4, A4, PAUSE, A4, PAUSE, A4, A4,
		G4, PAUSE, G4, PAUSE, G4, G4, A4, C5, PAUSE, C5, C5,
		A4, G4, F4, G4, A4, PAUSE, A4, PAUSE, A4, PAUSE,
		A4, A4, G4, PAUSE, G4, A4, G4, F4, F4, F4 };
double freq;
unsigned int i = 0;
enum PlayStates { PlayOff, PlayOn } PlayState;
void TickPlay()
{
	unsigned char button0 = (~PINA) & 0x01;
	
	switch (PlayState) // Play Transitions
	{
		case PlayOff:
			if (button0)
			{
				PWM_on();
				PlayState = PlayOn;
			}
			else
			{
				PlayState = PlayOff;
			}
			break;
			
		case PlayOn:
			if (frequencyIterator < frequencySize)
			{
				freq = frequencies[frequencyIterator];
				if (freq == PAUSE)
				{
					if (i >= 1)
					{
						i = 0;
						frequencyIterator++;
						set_PWM(freq);
					}
				}
				else
				{
					if (i >= 5)
					{
						i = 0;
						frequencyIterator++;
						set_PWM(freq);
					}
				}
				++i;
				PlayState = PlayOn;
			}
			else
			{
				frequencyIterator = 0;
				PWM_off();
				PlayState = PlayOff;
			}
			break;
			
		default:
			PlayState = PlayOff;
			frequencyIterator = 0;
			break;
	}
	
	switch (PlayState) // Play Actions
	{
		case PlayOff:
			PORTB = 0x00;
			break;
		
		case PlayOn:
			PORTB = 0xFF;
			break;
		
		default:
			PORTB = 0x00;
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_off();
	TimerOn();
	TimerSet(100);
	
    while (1) 
    {
		TickPlay();
		while (!TimerFlag) {};
		TimerFlag = 0;
    }
}

