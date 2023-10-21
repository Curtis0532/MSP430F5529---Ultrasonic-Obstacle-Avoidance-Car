#include <msp430.h>
#define TRIG_PIN BIT1  // P4.1 as trigger
#define ECHO_PIN BIT2  // P4.2 as echo
#define SOUND_SPEED 340 // Speed of sound in m/s
#define LD 7000 //left duty cycle
/*
 * module.c
 *
 *  Created on: Oct 17, 2023
 *      Author: curtis.lin
 */


void forward(void)
{
    P8OUT |= 0b00000110;
    P6OUT |= 0b00001010;
    TB0CCR5 = LD; //set P3.5 to PWM d=ld/10000

}

void backward(void)
{
    P8OUT |= 0b00000110;
    P6OUT |= 0b00010100;
    TB0CCR5 = LD;
}

void turnleft(void)
{
    P8OUT |= 0b00000110;
    P6OUT |= 0b00001100;
    TB0CCR5 = LD;
}

void turnright(void)
{
    P8OUT |= 0b00000110;
    P6OUT |= 0b00010010;
    TB0CCR5 = LD;
}

void brake(void)
{
    P6OUT &= 0b00011110;
    __delay_cycles(500000);
    P8OUT &= 0b00000000;
    P6OUT &= 0b00000000;
    TB0CCR5 = 0;
}

void delay(int milliseconds)
{
    int i;
    for (i = 0; i < milliseconds; i++)
    {
        __delay_cycles(1000);
    }
}

void ini_(void)
{
        P8DIR |= 0b00000110;
        P8OUT &= 0b00000000;
        P6DIR |= 0b00011110;
        P6OUT &= 0b00000000;
        P4DIR |= 0b10000110;
        P4OUT &= 0b00000000;
        P4DIR |= BIT7; // Set P4.7 as an output
        P4OUT |= BIT7; // Set P4.7 to high
        P1DIR |= BIT0; // Set P1.0 as an output
        P1OUT &= ~BIT0; // Set P1.0 to low
        P7DIR |= BIT4;  // Set P7.4 as an output
        P7SEL |= BIT4; // Set P7.4 to TB0.2 output
        P3DIR |= BIT5;  // Set P3.5 as an output
        P3SEL |= BIT5; // Set P3.5 to TB0.5 output
        P3DIR |= BIT6;  // Set P3.6 as an output
        P3SEL |= BIT6; // Set P3.6 to TB0.6 output
}


 void initClock()
 {
    // Set DCO to 1 MHz
    UCSCTL1 = DCORSEL_0; // DCO Range Select: 1 MHz
    UCSCTL2 = FLLD_0 + 29; // FLL Multiplier: 30, 1 MHz x 30 = 30 MHz

    // Set ACLK to XT1 (assumes XT1 is connected)
    UCSCTL3 = SELREF__XT1CLK;

    // Set SMCLK and MCLK to DCO
    UCSCTL4 = SELM__DCOCLK | SELS__DCOCLK;

    // Divide SMCLK by 1
    UCSCTL5 = DIVS__1;
 }

 void initSR04()
 {
     // Configure the TRIG_PIN as an output and ECHO_PIN as an input
     P4DIR |= TRIG_PIN;
     P4DIR &= ~ECHO_PIN;
 }

 void triggerSR04()
 {
     P4OUT |= TRIG_PIN;
     __delay_cycles(10);  // Generate a 10us trigger pulse
     P4OUT &= ~TRIG_PIN;
 }



 void initButton()
 {

     P2DIR &= ~BIT1;
     P2REN |= BIT1;
     P2OUT |= BIT1;


     P2IES |= BIT1;
     P2IFG &= ~BIT1;
     P2IE |= BIT1;

     P1DIR &= ~BIT1;
     P1REN |= BIT1;
     P1OUT |= BIT1;


     P1IES |= BIT1;
     P1IFG &= ~BIT1;
     P1IE |= BIT1;
 }
 unsigned int measureDistance()
 {
     unsigned int echo_pulse_duration = 0;
     unsigned int distance =0;
     // Trigger the sensor
     triggerSR04();
     while ((P4IN & BIT2) == 0);   // Wait for start of echo pulse
     TA1R = 0;                     // Reset timer at start of pulse
     while ((P4IN & BIT2) > 0);    // Wait for end of echo pulse
     echo_pulse_duration = TA1R;   // Current timer value is pulse length
     distance = 0.017 * echo_pulse_duration; // Convert from us to cm

     if (distance == 0) {
         // Set a default value (e.g., 999) if no valid distance is measured
         distance = 999;
     }

     return distance;
 }


 void initTimerB()
 {
     TB0CTL = TASSEL_2 + MC_1 + ID_1; //1Mhz/2=500k
     TB0CCTL0 = CCIE;
     TB0CCR0 = 10000; //500k/10k=50Hz
     TB0CCTL2 = OUTMOD_7;
     TB0CCTL5 = OUTMOD_7;
     TB0CCTL6 = OUTMOD_7;
     TB0CCR1 = 0;//500k/50k=10Hz
     TB0CCR2 = 0;
     TB0CCR5 = 0;
     TB0CCR6 = 0;
 }

 void initTimerA()
 {
     TA1CTL = TASSEL_2 + ID_0 + MC_2;
 }
