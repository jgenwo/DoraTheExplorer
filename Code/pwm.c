/*
 * File:   pwm.c
 * Author: Steff
 *
 * Created on 11. Juni 2019, 18:31
 */


#include "xc.h"
#include "gpio.h"

void init_PWM()
{

    /*~~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/  
    
    P1TCONbits.PTEN = 1;    //Disable the PWM Module for configuration
    
    P1TCONbits.PTMOD = 0;   // Define free running mode for PWM 1
    
    PWM1CON1bits.PMOD1 = 1; // Define independent mode for PWM output pair 1
    
    P1TPERbits.PTPER = 100; // PWM will count "up to 100" before resetting
    
    P1DC1 = 50; // Will be active till count goes to 50
    // P1DC2 = 50; // Will be active till count goes to 50
    
    PWM1CON1bits.PEN1H = 0; // PWM1H is controlled by GPIO module
    
    PWM1CON1bits.PEN1L = 1; // PWM1L is controlled by PWM module
    // PIN 26 is used as PWM output PIN
    
    P1TCONbits.PTEN = 1;    // Enable the PWM Module
    
    HBRIDGE1 = 1; // An output for H-Bridge logic input
    HBRIDGE2 = 0; // An output for H-Bridge logic input
    
    // No deadtime inserted yet, as it seems irrelevant right now
}
