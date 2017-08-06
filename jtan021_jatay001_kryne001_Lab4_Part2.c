/*
 * jtan021_jatay001_kryne001_lab4_part2.c
 *
 * Created: 1/20/2017 12:34:04 PM
 * Author: student
 */ 

#include <avr/io.h>

enum Action_States {Idle, Reset, Increment, Decrement} Action_State;
enum C_States {Init, Pressed} C_State;
	
unsigned char tempC;

void action()
{
    unsigned char button0 = (~PINA) & 0x01;
	unsigned char button1 = (~PINA) & 0x02;
    
    switch(C_State)
    {
        case Init:
			if (button0 && button1)
			{
				Action_State = Reset;
				C_State = Pressed;
			}
			else if (button0 && !button1)
			{
				Action_State = Increment;
				C_State = Pressed;
			}
			else if (!button0 && button1)
			{
				Action_State = Decrement;
				C_State = Pressed;
			}
			else
			{
				Action_State = Idle;
				C_State = Init;
			}
			break;
			
		case Pressed:
			if (Action_State == Reset)
			{
				button0 = (~PINA) & 0x01;
				button1 = (~PINA) & 0x02;
				
				if (!button0 && !button1)
				{
					tempC = 7;
					PORTC = tempC;
					C_State = Init;
					Action_State = Idle;
				}
				else
				{
					C_State = Pressed;
				}
			}
			else if (Action_State == Increment)
			{
				tempC++;

				if (tempC <= 9)
				{
					PORTC = tempC;
				}
				
				if ((~PINA) == 0x00)
				{
					C_State = Init;
				}
				else
				{
					C_State = Pressed;
				}
				Action_State = Idle;
			}
			else if (Action_State == Decrement)
			{
				tempC--;
				
				if (tempC >= 0)
				{
					PORTC = tempC;
				}
				
				if ((~PINA) == 0x00)
				{
					C_State = Init;
				}
				else
				{
					C_State = Pressed;
				}
				Action_State = Idle;
			}
			else
			{
				if ((~PINA) == 0x00)
				{
					C_State = Init;
				}
				else
				{
					C_State = Pressed;
				}
			}
			break;

		default:
			PORTC = 0x07;
			Action_State = Idle;
			C_State = Init;
			break;
    }
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x07;
    
	Action_State = Idle;
    C_State = Init;
	tempC = 7;
    
    while(1)
	{
        action();
	}
}
