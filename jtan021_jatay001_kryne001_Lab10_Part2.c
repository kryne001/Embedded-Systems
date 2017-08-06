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
unsigned long _avr_timer_M = 1;
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
//--------End Task scheduler data structure-----------------------------------

unsigned char curr = 1;

enum State { Wait } state;

int tick(int state)
{
	switch (state) // State transitions
	{
		case Wait:
			state = Wait;
			break;

		default:
			state = Wait;
			break;
	}

	switch(state)  // State actions
	{
		case Wait:
			if (curr == 1)
			{
				LCD_DisplayString(1,"               l");
			}
			else if (curr == 2)
			{
				LCD_DisplayString(1,"              le");
			}
			else if (curr == 3)
			{
				LCD_DisplayString(1,"             leg");
			}
			else if (curr == 4)
			{
				LCD_DisplayString(1,"            lege");
			}
			else if (curr == 5)
			{
				LCD_DisplayString(1,"           legen");
			}
			else if (curr == 6)
			{
				LCD_DisplayString(1,"          legend");
			}
			else if (curr == 7)
			{
				LCD_DisplayString(1,"         legend.");
			}
			else if (curr == 8)
			{
				LCD_DisplayString(1,"        legend..");
			}
			else if (curr == 9)
			{
				LCD_DisplayString(1,"       legend...");
			}
			else if (curr == 10)
			{
				LCD_DisplayString(1,"      legend...w");
			}
			else if (curr == 11)
			{
				LCD_DisplayString(1,"     legend...wa");
			}
			else if (curr == 12)
			{
				LCD_DisplayString(1,"    legend...wai");
			}
			else if (curr == 13)
			{
				LCD_DisplayString(1,"  legend...wait ");
			}
			else if (curr == 14)
			{
				LCD_DisplayString(1," legend...wait  ");
			}
			else if (curr == 15)
			{
				LCD_DisplayString(1,"legend...wait  f");
			}
			else if (curr == 16)
			{
				LCD_DisplayString(1,"egend...wait  fo");
			}
			else if (curr == 17)
			{
				LCD_DisplayString(1,"gend...wait  for");
			}
			else if (curr == 18)
			{
				LCD_DisplayString(1,"end...wait  for ");
			}
			else if (curr == 19)
			{
				LCD_DisplayString(1,"nd...wait  for i");
			}
			else if (curr == 20)
			{
				LCD_DisplayString(1,"d...wait  for it");
			}
			else if (curr == 21)
			{
				LCD_DisplayString(1,"...wait  for it ");
			}
			else if (curr == 22)
			{
				LCD_DisplayString(1,"..wait  for it D");
			}
			else if (curr == 23)
			{
				LCD_DisplayString(1,".wait  for it DA");
			}
			else if (curr == 24)
			{
				LCD_DisplayString(1,"wait  for it DAI");
			}
			else if (curr == 25)
			{
				LCD_DisplayString(1,"ait  for it DAIR");
			}
			else if (curr == 26)
			{
				LCD_DisplayString(1,"it  for it DAIRY");
			}
			else if (curr == 27)
			{
				LCD_DisplayString(1,"t  for it DAIRY ");
			}
			else if (curr == 28)
			{
				LCD_DisplayString(1,"  for it DAIRY  ");
			}
			else if (curr == 29)
			{
				LCD_DisplayString(1," for it DAIRY   ");
			}
			else if (curr == 30)
			{
				LCD_DisplayString(1,"for it DAIRY    ");
			}
			else if (curr == 31)
			{
				LCD_DisplayString(1,"or it DAIRY     ");
			}
			else if (curr == 32)
			{
				LCD_DisplayString(1,"r it DAIRY      ");
			}
			else if (curr == 33)
			{
				LCD_DisplayString(1," it DAIRY       ");
			}
			else if (curr == 34)
			{
				LCD_DisplayString(1,"it DAIRY        ");
			}
			else if (curr == 35)
			{
				LCD_DisplayString(1,"t DAIRY         ");
			}
			else if (curr == 36)
			{
				LCD_DisplayString(1," DAIRY          ");
			}
			else if (curr == 37)
			{
				LCD_DisplayString(1,"DAIRY           ");
			}
			else if (curr == 38)
			{
				LCD_DisplayString(1,"AIRY            ");
			}
			else if (curr == 39)
			{
				LCD_DisplayString(1,"IRY             ");
			}
			else if (curr == 40)
			{
				LCD_DisplayString(1,"RY              ");
			}
			else if(curr == 41)
			{
				LCD_DisplayString(1,"Y               ");
			}
			else if(curr == 42)
			{
				LCD_DisplayString(1,"               ");
				curr = 0;
			}
			curr++;
			break;
		
		default:
			state = Wait;
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
	LCD_init();
	TimerOn();
	
	unsigned long int calc = 500;
	unsigned long int tempGCD = 500;
	unsigned long int GCD = tempGCD;
	unsigned long int period = calc/GCD;
	static task task1;
	task *tasks[] =  { &task1};
	task1.state = -1;
	task1.period = period;
	task1.elapsedTime = period;
	task1.TickFct = &tick;

	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned short i;
	TimerSet(GCD);
	while(1)
	{
		for (i = 0; i < numTasks; ++i)
		{
			if (tasks[i]->elapsedTime == tasks[i]->period)
			{
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}
