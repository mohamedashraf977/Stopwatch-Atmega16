/*
 * main.c
 *
 * Created: 18-Nov-21 2:44:11 PM
 *  Author: Mohamed Ashraf
 */ 

#include <xc.h>
#include <avr/interrupt.h>
#include <util/delay.h>


unsigned char flag_seconds=0;


unsigned char seconds_count=0;
unsigned char minutes_count=0;
unsigned char hours_count=0;
unsigned char g_ticks=0;

void TIMER0_init (unsigned char ticks)
{
	TCNT0=0;
	TCCR0= (1<<WGM01) | (1<<CS00) | (1<<CS02) | (1<<FOC0) ;
	OCR0=ticks;
	TIMSK= TIMSK | (1<<OCIE0);
}


ISR (TIMER0_COMP_vect)
{
	g_ticks++;
	if (g_ticks==4)
	{
		flag_seconds=1;
		g_ticks=0;
	}
}


void interrupt_init ()
{
	GICR=GICR | (1<<INT0);
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);
}

ISR (INT0_vect)
{
	seconds_count=0;
	minutes_count=0;
	hours_count=0;
}


int main(void)
{
	/* Initializing Enable pins of the 7-seg connected to PORTA*/
	DDRA=0xFF;
	PORTA=0xFF;
	
	DDRC= (1<<0) | (1<<1) | (1<<2) | (1<<3);
	 PORTC=0x00;
	 
	 DDRD=DDRD & (~(1<<2));
	 SREG|= (1<<7);
	
	TIMER0_init (250);
	interrupt_init ();
    while(1)
    {
        if (flag_seconds)
		{
			seconds_count++;
			if (seconds_count==60)
			{
				minutes_count++;
				seconds_count=0;
				if (minutes_count==60)
				{
					hours_count++;
					minutes_count=0;
				}
			}
			flag_seconds=0;
		}
		
		else
		{
			PORTA=0x01;
			PORTC=(seconds_count%10);
			_delay_ms(2);
			
			PORTA=0x02;
			PORTC=(seconds_count/10);
			_delay_ms(2);
			
			PORTA=0x04;
			PORTC=(minutes_count%10);
			_delay_ms(2);
			
			PORTA=0x08;
			PORTC=(minutes_count/10);
			_delay_ms(2);
			
			PORTA=0x10;
			PORTC=(hours_count%10);
			_delay_ms(2);
			
			PORTA=0x20;
			PORTC=(hours_count/10);
			_delay_ms(2);
		}
		
    }
}