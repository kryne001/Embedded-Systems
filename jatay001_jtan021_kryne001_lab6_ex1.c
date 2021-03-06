/*
 * jtan021_kryne001_jatay001_lab6.c
 *
 * Created: 2/2/2017 1:13:01 PM
 *  Author: student
 */ 


#include <avr/io.h>
//#include "io.c"
#include "io.h"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum ID_States { ID_Init, ID_wait, ID_increment, ID_decrement, ID_reset } ID_State;

unsigned char tmpOut;

void TickFct()
{
	unsigned char button0 = ~PINA & 0x01;
	unsigned char button1 = ~PINA & 0x02;
	
	switch(ID_State)
	{		
		case ID_Init:
			ID_State = ID_wait;
			break;
			
		case ID_wait:
			if (button0 && !button1)
			{
				ID_State = ID_increment;
			}
			else if (button1 && !button0)
			{
				ID_State = ID_decrement;
			}
			else if (button0 && button1)
			{
				ID_State = ID_reset;
			}
			else
			{
				ID_State = ID_wait;
			}
			break;
			
		case ID_increment:
			if (tmpOut < 9)
			{
				++tmpOut;
			}
			ID_State = ID_wait;
			break;
			
		case ID_decrement:
			if (tmpOut > 0)
			{
				--tmpOut;
			}
			ID_State = ID_wait;
			break;
			
		case ID_reset:
			ID_State = ID_wait;
			break;
			
		default:
			ID_State = ID_wait;
			break;
	}
	
	switch(ID_State)
	{
		case ID_wait:
			break;
		case ID_Init:
			tmpOut = 0;
			LCD_Cursor(1);
			LCD_WriteData(tmpOut + '0');
			break;
		case ID_increment:
			LCD_Cursor(1);
			LCD_WriteData(tmpOut + '0');
			break;
		case ID_decrement:
			LCD_Cursor(1);
			LCD_WriteData(tmpOut + '0');
			break;
		case ID_reset:
			tmpOut = 0;
			LCD_Cursor(1);
			LCD_WriteData(tmpOut + '0');
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	tmpOut = 0;
	ID_State = ID_Init;
	// Initializes the LCD display
	LCD_init();
	TimerSet(1000);
	TimerOn();
	LCD_WriteData(0 + '0');
	
	while(1) {
		TickFct();
		while(!TimerFlag);
		TimerFlag = 0;
		
	}
}
