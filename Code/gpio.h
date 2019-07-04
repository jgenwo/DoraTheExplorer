/* 
 * File:   gpio.h
 * Author: a2-lenz
 *
 * Created on April 5, 2019, 11:31 AM
 */

#ifndef GPIO_H
#define	GPIO_H
#include "xc.h"

#define LED_Back  LATCbits.LATC4 // NOT WORKING YET
#define LED_Front LATCbits.LATC3 

//#TODO 
//******* Define two bits as output bits for the first H-Bridge
#define HBRIDGE2 LATAbits.LATA7 
#define HBRIDGE1 LATBbits.LATB14 

//******* Define two bits as output bits for the second H-Bridge
#define HBRIDGE3 LATBbits.LATB9 
#define HBRIDGE4 LATCbits.LATC6


void initIO();
#endif	/* GPIO_H */

