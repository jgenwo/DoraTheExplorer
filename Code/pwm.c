/*
 * File:   pwm.c
 * Author: Steff
 *
 * Created on 11. Juni 2019, 18:31
 */

#include "xc.h"
#include "gpio.h"
#include "dma.h"
#include <stdio.h>
#include "qei.h"
#include "control.h"

void init_PWM()
{

    /*~~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    P1TCONbits.PTEN = 0; //Disable the PWM Module for configuration
    P2TCONbits.PTEN = 0;

    P1TCONbits.PTMOD = 0; // Define free running mode for PWM 1
    P2TCONbits.PTMOD = 0;

    PWM1CON1bits.PMOD1 = 1; // Define independent mode for PWM output pair 1
    //PWM1CON1bits.PMOD2 = 1; // Define independent mode for PWM output pair 2
    PWM2CON1bits.PMOD1 = 1;
    

    P1TPERbits.PTPER = 2000; // PWM will count "up to 100" before resetting
    P2TPERbits.PTPER = 2000;

    /* THIS IS THE KEY VARIABLE TO CHANGE SPEED WITH
	* The duty cycle basically calculates as:
	* P1DC1/PxTPERbits.PTPER , where x denotes the number of the PWM module
	* Raising PxDCy increases speed and vice versa.
	* Here, x stands for the PWM module number and y for the PWM output channel
	* number of that module
	* Raising PTPER allows smaller increments (e.g. 100 allows increments of 1/100)
	* but makes the puls width bigger, decreasing smoothness of V curve
	*/
    P1DC1 = 1000; // PWM output will be active till count goes to 1000
    P2DC1 = 1000;

    //P1DC2 = 1000; // PWM output will be active till count goes to 1000

    PWM1CON1bits.PEN1H = 0; // PWM1H is controlled by GPIO module
    PWM1CON1bits.PEN1L = 1; // PWM1L is controlled by PWM module
    // PIN 26 is used as PWM output PIN for motor 1

    PWM2CON1bits.PEN1H = 0; // PWM3H is controlled by GPIO module
    PWM2CON1bits.PEN1L = 1; // PWM3L is controlled by PWM module
    // PIN 24 is used as PWM output PIN for motor 2

    P1TCONbits.PTEN = 1; // Enable the PWM Module
    P2TCONbits.PTEN = 1;    

    // No deadtime inserted yet, as it seems irrelevant right now
}

// The HBRIDGE settings depend on where +/- of the motors is put and which pins
// go exactly where. The current ones are completely arbitrary as
// "placeholders" for future values with the board
void drive_motor_1_backward(int dutycycle)
{
	HBRIDGE1 = 1;
	HBRIDGE2 = 0;
	P1DC1 = dutycycle;
}

void drive_motor_1_forward(int dutycycle)
{
	HBRIDGE1 = 0;
	HBRIDGE2 = 1;
	P1DC1 = dutycycle;
}

void drive_motor_2_forward(int dutycycle)
{
	HBRIDGE3 = 1;
	HBRIDGE4 = 0;
	P1DC2 = dutycycle;
}

void drive_motor_2_backward(int dutycycle)
{
	HBRIDGE3 = 0;
	HBRIDGE4 = 1;
	P1DC2 = dutycycle;
}

void fast_stop_motor1()
{
	HBRIDGE1 = HBRIDGE2;
}

void fast_stop_motor2()
{
	HBRIDGE3 = HBRIDGE4;
}