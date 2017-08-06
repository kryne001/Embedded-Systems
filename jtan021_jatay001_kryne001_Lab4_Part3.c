/*
* jtan021_jatay001_kryne001_lab4_part3.c
*
* Created: 1/20/2017 12:34:04 PM
* Author: student
*/

#include <avr/io.h>

enum Action_States { Idle, Pattern0, Pattern1, Pattern2, Pattern3, Pattern4, Pattern5, Pattern6 } Action_State;
enum B_States { Start, Pressed, Released } B_State;

unsigned char tempC;

void action()
{
	unsigned char button0 = (~PINA) & 0x01;

	switch (B_State)
	{
	case Start:
		B_State = Released;
		break;

	case Released:
		if (button0)
		{
			B_State = Pressed;
		}
		else
		{
			B_State = Released;
		}
		break;

	case Pressed:
		if (!button0)
		{
			if (Action_State == Pattern6)
			{
				Action_State = Idle;
			}
			else
			{
				Action_State++;
			}
			B_State = Released;
		}
		else
		{
			B_State = Pressed;
		}
		break;

	default:
		B_State = Released;
		Action_State = Idle;
		break;
	}

	switch (Action_State)
	{
	case Idle:
		PORTB = 0x00;
		break;

	case Pattern1:
		PORTB = 0x01;
		break;

	case Pattern2:
		PORTB = 0x02;
		break;

	case Pattern3:
		PORTB = 0x03;
		break;

	case Pattern4:
		PORTB = 0x04;
		break;

	case Pattern5:
		PORTB = 0x05;
		break;

	case Pattern6:
		PORTB = 0x06;
		break;

	default:
		PORTB = 0x00;
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x07;

	Action_State = Idle;
	B_State = Start;

	while (1)
	{
		action();
	}
}
