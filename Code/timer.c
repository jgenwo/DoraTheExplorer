
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "xc.h"
#include "dma.h"
#include "sensor.h"
#include "stdio.h"
#include "qei.h"
#include "control.h"


void initTimer1(unsigned int period)
{
    T1CON = 0;              // ensure Timer 1 is in reset state
    T1CONbits.TGATE = 0;    // gated time accumulation disabled
    T1CONbits.TCKPS = 0b10; // FCY divide by 64: tick = 1.6us
    T1CONbits.TCS = 0;      // select internal FCY clock source
    TMR1 = 0;
    PR1 = period - 1;  // set Timer 1 period register ()
    IFS0bits.T1IF = 0; // reset Timer 1 interrupt flag
    IPC0bits.T1IP = 4; // set Timer1 interrupt priority level to 4
    IEC0bits.T1IE = 1; // enable Timer 1 interrupt
    T1CONbits.TON = 0; // leave timer disabled initially
}

void startTimer1(void)
{
    T1CONbits.TON = 1; //
}

void initTimer2(unsigned int period)
{
    // This timer is used to constantly calculate motor speed.
    // The interrupt interval must be less than the minimum time required
    // for a 1/2 revolution at maximum speed. The motors no-load speed is
    // ca. 7000 RPM, e.g. about 8571 us per revolution. The timer period should
    // therefore never be longer than 4285 us (I think).
    // With a 1:64 prescaler and TCY = 25 ns, this means that the int period
    // should always be <= 2678
    T2CON = 0;              // ensure Timer 2 is in reset state
    T2CONbits.T32 = 0;      // Run Timer 2 in 16 bit instead of 32 bit
    T2CONbits.TGATE = 0;    // gated time accumulation disabled
    T2CONbits.TCKPS = 0b10; // FCY divide by 64: tick = 1.6us
    T1CONbits.TCS = 0;      // select internal FCY clock source
    TMR2 = 0;               // Just setting initial timer value to 0
    
    PR2 = period - 1;  // set Timer 2 period register ()
    IFS0bits.T2IF = 0; // reset Timer 2 interrupt flag
    IPC1bits.T2IP = 5; // set Timer2 interrupt priority level to 5
    IEC0bits.T2IE = 1; // enable Timer 2 interrupt
    T2CONbits.TON = 0; // leave timer disabled initially
}

void startTimer2(void)
{
    T2CONbits.TON = 1; //
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    // this gets set the first time it is called and then never again
    static int count = 0;

    IFS0bits.T1IF = 0; // reset Timer 1 interrupt flag

    //LEDFront=~LEDFront;
    if (count < 9)
    {
        count++;
    }
    else
    {
        LED_Back = ~LED_Back;
        LED_Front = ~LED_Front;
        count = 0;
    }
}

// Timer that is there to constantly update the current speed of the motors
// atm it's just one motor though.
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0; // reset Timer 2 interrupt flag
    
    //Negative value for backward / Positive value for forward
    go_straight(-20);

}
