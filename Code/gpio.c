
#include "gpio.h"

void initIO()
{
    int i;

    // set up analogue pins
    AD1CON1bits.ADON = 0; // disable ADC1 module
    AD1PCFGL = 0xFFFF;    //all pins are digital

    // set digital port direction
    // inputs
    //all pins are inputs by default

    //outputs
    TRISCbits.TRISC4 = 0; //LEDBack output / NOT WORKING YET
    TRISCbits.TRISC3 = 0; //LEDFront output

    // ******* Configure two bits as output bits for the first hbridge
    TRISAbits.TRISA7 = 0;
    TRISBbits.TRISB14 = 0; 
    // ******* Configure two bits as output bits for the second hbridge
    TRISBbits.TRISB9 = 0; 
    TRISCbits.TRISC6 = 0; 
    // ******* H-Bridge Part over

    // set up remappable pins
    __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 (unlock, they are usually write protected)

    RPINR14bits.QEA1R = 24; 
    RPINR14bits.QEB1R = 25; 

    RPINR16bits.QEA2R = 13; 
    RPINR16bits.QEB2R = 12;
    
    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS registers (lock again!)

    for (i = 0; i < 30000; i++)
        ; // short delay
}
