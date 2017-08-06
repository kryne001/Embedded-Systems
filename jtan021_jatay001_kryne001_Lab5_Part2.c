/*
 * lab5_1.c
 *
 * Created: 1/26/2017 12:45:14 PM
 * Author : Kyler
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	
	TCCR1B = 0x0B;
	
	OCR1A = 50;
	
	TIMSK1 = 0x02;
	
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR (TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum Game_States { GameStart, StateA, StateB, StateC, StateB2 } Game_State;
enum States { Start, Init, Game, Wait } State;
	
void Tick()
{
	unsigned char button = ~PINA & 0x01;
	unsigned char startButton = ~PINA & 0x08;
	
	switch (State) {
		case Start:
			State = Init;
			break;
			
		case Init:
			if (startButton)
			{
				State = Game;
				Game_State = GameStart;
			}
			else
			{
				State = Init;
			}
			break;
			
		case Game:
			if (button)
			{
				State = Wait;
			}
			else
			{
				if (Game_State == StateB2)
				{
					Game_State = StateA;
				}
				else
				{
					Game_State++;
				}
				State = Game;
			}
			break;
			
		case Wait:
			if (startButton)
			{
				Game_State = GameStart;
				State = Init;
			}
			else
			{
				State = Wait;
			}
			break;
			
		default:
			State = Init;
			Game_State = GameStart;
			break;
	}
	
	switch (Game_State)
	{
		case GameStart:
			PORTB = 0x00;
			break;
			
		case StateA:
			PORTB = 0x01;
			while(!TimerFlag);
			TimerFlag = 0;
			break;
			
		case StateB:
			PORTB = 0x02;
			while(!TimerFlag);
			TimerFlag = 0;
			break;
			
		case StateC:
			PORTB = 0x04;
			while(!TimerFlag);
			TimerFlag = 0;
			break;
			
		case StateB2:
			PORTB = 0x02;
			while(!TimerFlag);
			TimerFlag = 0;
			break;		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	
	State = Start;
	Game_State = GameStart;
	
    while (1) 
    {
		Tick();
    }
	
	return 0;
}



