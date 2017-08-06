/*
 * jtan021_jatay001_kryne001_Lab10.c
 *
 * Created: 2/26/2017 11:07:51 PM
 * Author : jonat
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;

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

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char GetKeypadKey() {
	// Check keys in col 1
	// Enable col 4 with 0, disable others with 1?s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }
	// Check keys in col 2
	// Enable col 5 with 0, disable others with 1?s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }
	// Check keys in col 3
	// Enable col 6 with 0, disable others with 1?s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }
	// Check keys in col 4
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }
	
	return('\0'); // default value
}

unsigned char SM2_output = 0x00;
unsigned char SM3_output = 0x00;
unsigned char pause = 0;
unsigned char x = 0;

enum States { Wait, Pressed, Released } State;

int tick(int state)
{
	switch (state) { // State transitions
		case Wait:
			if (GetKeypadKey() != '\0')
			{
				state = Pressed;
			}
			else
			{
				state = Wait;
			}
			break;

		case Pressed:
			if (GetKeypadKey() != '\0')
			{
				state = Released;
				break;
			}
			else
			{
				state = Pressed;
			}
			break;

		case Released:
			if (GetKeypadKey() == '\0')
			{
				state = Wait;
			}
			else
			{
				state = Released;
			}
			break;

		default:
			state = Wait;
			break;
	}

	switch(state) { // State actions
		case Wait:
			break;
		case Pressed:
			x = GetKeypadKey();
			switch (x) {
				case '1':LCD_WriteData('1');break; // hex equivalent
				case '2':LCD_WriteData('2');break;
				case '3':LCD_WriteData('3');break; // All 5 LEDs on
				case '4':LCD_WriteData('4');break; // hex equivalent
				case '5':LCD_WriteData('5');break;
				case '6':LCD_WriteData('6');break; // All 5 LEDs on
				case '7':LCD_WriteData('7');break; // hex equivalent
				case '8':LCD_WriteData('8');break;
				case '9':LCD_WriteData('9');break; // All 5 LEDs on
				case 'A':LCD_WriteData('A'); break; // hex equivalent
				case 'B':LCD_WriteData('B'); break;
				case 'C':LCD_WriteData('C'); break;
				case 'D':LCD_WriteData('D'); break;
				case '*':LCD_WriteData('*'); break;
				case '0':LCD_WriteData('0'); break;
				case '#':LCD_WriteData('#'); break; // toggle pause
			}
			break;
		case Released:
			break;
		default:
			break;
	}

	return state;
}
int main(void)
{
	DDRA = 0XFF; PORTA = 0X00;
	DDRB = 0xFF; PORTB = 0x00; 
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(1000);
	TimerOn();
	LCD_init();
	
	unsigned long int value = 1000;
	unsigned long int tmpGCD = 1;
	unsigned long int GCD = tmpGCD;
	unsigned long int period = value/GCD;
	static task task1;
	task *tasks[] =  {&task1};
	task1.state = -1;
	task1.period = period;
	task1.elapsedTime = period;
	task1.TickFct = &tick;

	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned short i;
	while(1)
	{
		for (i = 0; i < numTasks; ++i)
		{
			if ( tasks[i]->elapsedTime == tasks[i]->period )
			{
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
	}
	return 0;
}
