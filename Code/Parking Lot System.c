// include headerfiles
#include <xc.h>
#include "LCD.h"
// some pre-configuration for our microcontroller
#pragma config WDTEN=OFF, FOSC=INTIO67,MCLRE=EXTMCLR,IESO=OFF, LVP=OFF
// define the crystall oscillator frequency
#define _XTAL_FREQ 4000000
#define TRIG1 PORTAbits.RA0
#define ECHO1 PORTAbits.RA1
#define TRIG2 PORTBbits.RB3
#define ECHO2 PORTCbits.RC7
#define LED	PORTCbits.RC1
#define CLOSE 0
#define WELCOME 1
#define GOODBYE 2
#define WAIT 4
// initialise variables
unsigned int sens_time1,sens_time2,distance1,distance2,count,i;
unsigned char state,res;
// function for LED blinking
void interrupt blink(void)
{
	if (TMR1IF==1)
	{
		count++;
		if (count==7)	// 500 ms delay
		{
			LED=~LED;
			count=0;
		}
		TMR1IF=0;
	}
}
// function for delay
void delay(unsigned char sec)
{
	for (i=0;i<sec*10;i++)
	{
		__delay_ms(100);
	}
}
// functions for motor rotations
char open(void)
{
	for(i=0;i<5;i++)
	{
		PORTCbits.RC0=1;
		__delay_ms(2);
		PORTCbits.RC0=0;
		__delay_ms(18);
	}
	res=1;
	return res;
}
char close(void)
{
	for(i=0;i<5;i++)
	{
		PORTCbits.RC0=1;
		__delay_ms(1);
		PORTCbits.RC0=0;
		__delay_ms(19);
	}
	res=0;
	return res;
}
void main()
{
	OSCCON=0b01010010;	// set oscillator frequency
	// pins' settings
	TRISA=0x02;			// all port A pins – outputs, except RA1
	ANSELA=0x00;		// and digital
	PORTA=0x00;			// clear port A
	TRISB=0x00;			// all port B pins - outputs
	ANSELB=0x00;		// and digital
	PORTB=0x00;			// clear port B except RB3
	TRISC=0x80;			// all port C pins – outputs, except RC7
	ANSELC=0x00;		// and digital
	PORTC=0x00;			// clear port C
	// timer0 settings (for sensor)
	T0PS0=1;	// prescaler is 4
	T0PS1=0;
	T0PS2=0;
	PSA=0;		// enable prescaler
	T08BIT=0;	// timer0 is 16 bit
	T0CS=0;		// timer0 is a timer
	// timer1 settings
	T1SYNC=0;	// synchronise ext. clock with system internal clock
	T1SOSCEN=0;	// secondary oscillator disabled
	TMR1CS0=0;	// instruction clock source
	TMR1CS1=0;
	T1CKPS0=0;	// prescaler is 1
	T1CKPS1=0;
	T1RD16=0;	// 8-bit R/W operations
	TMR1IE=1;	// enable timer1 interrupts
	GIEL=1;		// enable global interrupts
	GIEH=1;
	TMR1IF=0;	// clear the flag
	init_display();		// initialise display
	set_address_line1(1);
	send_msg("PARKING");
	close();		// gates are closed
	LED=0;
	state=CLOSE;	// initial state of the gate
	while(1)
	{
		switch(state)
		{
			case CLOSE:
				TMR1ON=0;
				TMR0H=0x00;		// reset timer0
				TMR0L=0x00;
				TRIG1=1;		// trigger sensor1
				__delay_us(14);
				TRIG1=0;
				while (ECHO1==0);	// turn on timer0 for counting time
				TMR0ON=1;	        // while echo1 hasn't been received
				while (ECHO1==1);	// when echo1 is received
				TMR0ON=0;		    // stop counting
				sens_time1=TMR0;
				distance1=sens_time1/15;    // calculate the distance
				TMR0H=0x00;		            // reset timer0
				TMR0L=0x00;
				TRIG2=1;		// trigger sensor2
				__delay_us(14);
				TRIG2=0;
				while (ECHO2==0);	// turn on timer0 for counting time
				TMR0ON=1;	        // while echo2 hasn't been received
				while (ECHO2==1);	// when echo2 is received
				TMR0ON=0;			// stop counting
				sens_time2=TMR0;
				distance2=sens_time2/15;    // calculate the distance
				if (distance1<=10)
				{
					state=WELCOME;
					TMR1ON=1;
					set_address_line2(1);   // display "WELCOME!"
					send_msg("WELCOME!");
				}
				else if (distance2<=10)
				{
					state=GOODBYE;
					TMR1ON=1;
					set_address_line2(1);// display "GOODBYE!"
					send_msg("GOODBYE!");
				}
				else
				{
					close();
					state=CLOSE;
					LED=0;
					set_address_line2(1);	// display "CLOSED"
					send_msg(" CLOSED ");
				}
				break;
			case WELCOME:
				if (res==0)
				{open();}			// open the gates
				delay(5);			// wait for 3 sec.
				state=CLOSE;		// change the state
				break;
			case GOODBYE:
				if (res==0)
				{open();}			// open the gates
				delay(5);			// wait for 3 sec.
				state=CLOSE;		// change the state;
				break;
		}
	}
}
