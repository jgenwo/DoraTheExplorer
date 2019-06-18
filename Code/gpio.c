
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
    TRISBbits.TRISB8 = 0; //LEDBack output
    //TRISCbits.TRISC3=0; //LEDFront output

    // ******* Configure two bits as output bits for the first hbridge
    TRISBbits.TRISB6 = 0; // HBRIDGE1 (Pin 15)
    TRISBbits.TRISB7 = 0; //HBRIDGE2 (Pin 16)
    // ******* Configure two bits as output bits for the second hbridge
    TRISBbits.TRISB4 = 0; // HBRIDGE3 (Pin 11)
    TRISAbits.TRISA4 = 0; //HBRIDGE4 (Pin 12)
    // ******* H-Bridge Part over

    // set up remappable pins
    __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 (unlock, they are usually write protected)
    // inputs
    // input pin mappig example below:, here you choose the register with the device and assign an remappable pin

    //I now set A1 to RP12, B1 to RP13, A2 to RP2 (pin 6) and B2 to RP10 (pin 21).
    RPINR14bits.QEA1R = 0xB; // Configure QEI RP11 channel A1 pin 22
    RPINR14bits.QEB1R = 0xC; // Configure QEI RP12 channel B1 pin 23
    RPINR16bits.QEA2R = 0x2; // Configure QEI RP2 channel A2 pin 6
    RPINR16bits.QEB2R = 0xA; // Configure QEI RP10 channel B2 pin 21

    //RPINR14bits.QEA1R = 8; // QEI1 A to pin RB8
    //RPINR14bits.QEA1R = 11; // QEI1 A to pin RB11
    //RPINR14bits.QEB1R = 10; // QEI1 B to pin RB10

    // RPINR16bits.QEA2R = 3; //QEI2 A to pin RB3 (used to be TAST input with switch, resolder )
    //RPINR16bits.QEB2R = 8;//QEI2 B to pin RB8

    //RPINR18bits.U1RXR = 1 ; //U1RX register is pin RB1 / RP1

    // outputs
    //output pin mapping example below, here you choose the pin register and then map it to a device
    //RPOR6bits.RP12R = 0b00111; // pin RB12 to SPI1 miso (the cod 0b00111 idetifies SPI miso, table in datasheet)
    //RPOR1bits.RP2R = 0b00011; // pin RP2 is connected to UART1 TX, for coding see table 11-2 in datasheet
    //end of set up of remappable pins

    //RPOR1bits.RP2R=0; //default pin output for LEDFront
    // RPOR10bits.RP20R=0;////default pin output for LEDBack
    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS registers (lock again!)

    for (i = 0; i < 30000; i++)
        ; // short delay
}
