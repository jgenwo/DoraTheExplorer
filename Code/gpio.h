/* 
 * File:   gpio.h
 * Author: a2-lenz
 *
 * Created on April 5, 2019, 11:31 AM
 */

#ifndef GPIO_H
#define	GPIO_H
#include "xc.h"

#define LED  LATBbits.LATB8 // Use Pin 17

//******* Define two bits as output bits for the first H-Bridge
#define HBRIDGE1 LATBbits.LATB6 // Use Pin 15
#define HBRIDGE2 LATBbits.LATB7 // Use Pin 16

//******* Define two bits as output bits for the second H-Bridge
#define HBRIDGE3 LATBbits.LATB4 // Use Pin 11
#define HBRIDGE4 LATAbits.LATA4 // Use Pin 12


void initIO();
#endif	/* GPIO_H */

