/*
 * jtan021_jatay001_kryne001_Lab7_Part3
 *
 * Created: 1/26/2017 12:45:14 PM
 * Author : Jonathan Tan, Jeff Atayan, Kyler Ryne
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char curr;
unsigned char i;
unsigned char j;
unsigned char sound;
unsigned char 

enum threeLEDs_States { threeLEDs_Start, threeLEDs_Bit0, threeLEDs_Bit1, threeLEDs_Bit2 } threeLEDs_State;
enum blinkingLED_States { blinkingLED_Start, blinkingLED_High, blinkingLED_Low } blinkingLED_State;
enum combineLEDs_States { combineLEDs_Start, combineLEDs_Out } combineLEDs_State;
enum sound_States {sound_Start, sound_Init, sound_High, sound_Low } sound_State;

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

void Tick()
{
	unsigned char button0 = (~PINA) & 0x01;
	unsigned char button1 = (~PINA) & 0x02;
	unsigned char button2 = (~PINA) & 0x04;
	
	switch(threeLEDs_State) // threeLEDs transitions
	{
		case threeLEDs_Start:
			i = 0;
			threeLEDs_State = threeLEDs_Bit0;
			break;
			
		case threeLEDs_Bit0:
			if (i == 500)
			{
				i = 0;
				threeLEDs_State = threeLEDs_Bit1;
			}
			else
			{
				threeLEDs_State = threeLEDs_Bit0;
				++i;
			}
			break;
			
		case threeLEDs_Bit1:
			if (i == 500)
			{
				i = 0;
				threeLEDs_State = threeLEDs_Bit2;
			}
			else
			{
				threeLEDs_State = threeLEDs_Bit1;
				++i;
			}				
			break;
			
		case threeLEDs_Bit2:
			if (i == 500)
			{
				i = 0;
				threeLEDs_State = threeLEDs_Bit0;
			}
			else
			{
				threeLEDs_State = threeLEDs_Bit2;
				++i;
			}			
			break;
			
		default:
			threeLEDs_State = threeLEDs_Start;
			break;
	}
	
	switch(blinkingLED_State) // blinkingLED transitions
	{
		case blinkingLED_Start:
			j = 0;
			blinkingLED_State = blinkingLED_High;
			break;
			
		case blinkingLED_High:
			if (j == 500)
			{
				j = 0;
				blinkingLED_State = blinkingLED_Low;
			}
			else
			{
				++j;
				blinkingLED_State = blinkingLED_High;
			}
			break;
			
		case blinkingLED_Low:
			if (j == 500)
			{
				j = 0;
				blinkingLED_State = blinkingLED_High;
			}	
			else
			{
				++j;
				blinkingLED_State = blinkingLED_Low;
			}
			break;
			
		default:
			blinkingLED_State = blinkingLED_Start;
			break;
	}
	
	switch(sound_State) // sound transitions
	{
		case sound_Start:
			sound_State = sound_Init;
			break;
			
		case sound_Init:
			if (button2)
			{
				sound_State = sound_High;
			}
			else
			{
				sound_State = sound_Init;
			}
			break;
			
		case sound_High:
			sound_State = sound_Low;
			break;
			
		case sound_Low:
			if (button2)
			{
				sound_State = sound_High;
			}
			else
			{
				sound_State = sound_Low;
			}
			break;
		
		default:
			sound_State = sound_Init;
			break;
	}
	
	switch(threeLEDs_State) // threeLEDs actions
	{
		case threeLEDs_Start:
			break;
		
		case threeLEDs_Bit0:
			threeLEDs = 0x01;
			break;
		
		case threeLEDs_Bit1:
			threeLEDs = 0x02;
			break;
			
		case threeLEDs_Bit2:
			threeLEDs = 0x04;
			break;
		
		default:
			threeLEDs = 0x00;
			break;
	}
	
	switch(blinkingLED_State) // blinkingLED actions
	{
		case blinkingLED_Start:
			break;
		
		case blinkingLED_High:
			blinkingLED = 0x08;
			break;
		
		case blinkingLED_Low:
			blinkingLED = 0x00;
			break;
		
		default:
			blinkingLED = 0x00;
			break;
	}
	
	switch(combineLEDs_State) // combineLEDs transitions
	{
		case combineLEDs_Start:
			combineLEDs_State = combineLEDs_Out;
			break;
			
		case combineLEDs_Out:
			combineLEDs_State = combineLEDs_Out;
			break;
			
		default:
			combineLEDs_State = combineLEDs_Out;
			break;
	}
	
	switch(combineLEDs_State) // combineLEDs actions
	{
		case combineLEDs_Start:
			break;
		
		case combineLEDs_Out:
			curr = 0x00 | (blinkingLED | threeLEDs);
			PORTB = curr;
			break;
		
		default:
			break;
	}
	
	switch(sound_State) // sound actions
	{
		case sound_Start:
			sound = 0xEF;
			PORTB = PORTB & sound;
			break;
			
		case sound_Init:
			sound = 0xEF;
			PORTB = PORTB & sound;
			break;
		
		case sound_High:
			sound = 0x10;
			PORTB = PORTB | sound;
			break;
		
		case sound_Low:
			sound = 0xEF;
			PORTB = PORTB & sound;
			break;
			
		default:
			sound = 0xEF;
			PORTB = PORTB & sound;
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(2);
	TimerOn();
	threeLEDs = 0x00;
	blinkingLED = 0x00;
	sound = 0x00;
	threeLEDs_State = threeLEDs_Start;
	blinkingLED_State = blinkingLED_Start;
	combineLEDs_State = combineLEDs_Start;
	sound_State = sound_Start;
	
	while(1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}



