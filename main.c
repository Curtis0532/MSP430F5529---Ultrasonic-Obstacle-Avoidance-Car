#include <msp430.h> 
#include <module.h>
/**
 * main.c
 */
unsigned int Distance;
int servo = 250; // D= servo / 10000 from 200 to 1200
int direction = 5; //


void main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	ini_();
	initClock();
	initButton();
	initSR04();
	initTimerB();
	initTimerA();
	__enable_interrupt();
	while(1)
	{
	    if ((P2IN & BIT1) == 0)
	    {
	        while(1)
	        {
	            while(Distance>=30)
	            {
	                forward();
	                Distance = measureDistance();

	            }
	            brake();
	            delay(100);
	            while(Distance <=50)
	            {
	                turnleft();
	                delay(20);
	                Distance = measureDistance();

	            }
	            brake();
	            delay(100);
	            Distance = measureDistance();
	        }

	    }


	    if ((P1IN & BIT1) == 0)
	    {

	    }

	}

}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if (P2IFG & BIT1)
    {
        P2IFG &= ~BIT1;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port4_ISR(void)
{
    if (P1IFG & BIT1)
    {
        P1IFG &= ~BIT1; // Clear the interrupt flag
    }
}
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    if (servo >=950)
        direction=-5;
    if (servo <=550)
        direction= 5;
    servo += direction;
    TB0CCR2 = servo;

}
