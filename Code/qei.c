#include "qei.h"
#include "xc.h"
#include "p33FJ128MC802.h"
#include "gpio.h"

void init_QEI(void)
{
    //I now set A1 to RP12, B1 to RP13, A2 to RP2 (pin 6) and B2 to RP10 (pin 21).
    RPINR14bits.QEA1R = 0xC; // Configure QEI RP12 channel A1 pin 23
    RPINR14bits.QEB1R = 0xD; // Configure QEI RP13 channel B1 pin 24
    RPINR16bits.QEA2R = 0xC; // Configure QEI RP2 channel A2 pin 6
    RPINR16bits.QEB2R = 0xD; // Configure QEI RP10 channel B2 pin 21

    // Configure QEI module
    // QEIEN - QEI enable/disable  Count Error Status Flag bit
    QEI1CONbits.CNTERR = 0; //CNTERR
    QEI2CONbits.CNTERR = 0;
    // QEISIDL - Idle mode 0=continue, 1=stop
    QEI1CONbits.QEISIDL = 1; //QEICONbits.QEISIDL // Discontinue operation when in idle mode
    QEI2CONbits.QEISIDL = 1;
    //INDEX: Index Pin State Status bit (read-only)
    //QEI1CONbits.INDEX = 0; // 1 = Index pin is high 0 = Index pin is low
    //UPDN: Position Counter Direction Status bit
    QEI1CONbits.UPDN = 0; // 1 positive/0 negative
    QEI2CONbits.UPDN = 0; // 1 positive/0 negative

    /*
     * QEIM<2:0>: Quadrature Encoder Interface Mode Select bits
     * 111 = Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match (MAXxCNT)
     * 110 = Quadrature Encoder Interface enabled (x4 mode) with index pulse reset of position counter
     * 101 = Quadrature Encoder Interface enabled (x2 mode) with position counter reset by match (MAXxCNT)
     * 100 = Quadrature Encoder Interface enabled (x2 mode) with index pulse reset of position counter
     * 011 = Unused (module disabled)
     * 010 = Unused (module disabled)
     * 001 = Starts 16-bit Timer
     * 000 = Quadrature Encoder Interface/Timer off
     * From example QEIM = 0b111; // x4 edge gain and reset POSCNT when == MAXCNT
     */
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;

    //SWPAB: Phase A and Phase B Input Swap Select bit
    QEI1CONbits.SWPAB = 0; // Do not swap the polarity of phase comparison
    QEI2CONbits.SWPAB = 0;

    //PCDOUT: Position Counter Direction State Output Enable bit
    QEI1CONbits.PCDOUT = 0; // Disable counter direction pin (normal I/O operation)
    QEI2CONbits.PCDOUT = 0;

    // TQGATE: Timer Gated Time Accumulation Enable bit
    QEI1CONbits.TQGATE = 0;
    QEI2CONbits.TQGATE = 0;

    //POSRES: Position Counter Reset Enable bit
    QEI1CONbits.POSRES = 0; // index pulse does not reset POSCNT
    QEI2CONbits.POSRES = 0;

    //TQCS: Timer Clock Source Select bitTQCS: Timer Clock Source Select bit
    QEI1CONbits.TQCS = 0; // internal clock source (Tcy)
    QEI2CONbits.TQCS = 0;

    DFLT1CONbits.QEOUT = 0; // disable digital filters bit 7
    DFLT2CONbits.QEOUT = 0;

    // set initial counter value and maximum range
    MAX1CNT = 0xffff; // set the highest possible time out
    POS1CNT = 0x7fff; // set POSCNT into middle of range
    MAX2CNT = 0xffff;
    POS2CNT = 0x7fff;

    // Configure Interrupt controller for QEI 1 - first motor
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    IEC3bits.QEI1IE = 1;  // enable QEI interrupt
    IPC14bits.QEI1IP = 5; // set QEI interrupt priority

    // Configure Interrupt controller for QEI 2 - second motor
    IFS4bits.QEI2IF = 0;  // clear interrupt flag
    IEC4bits.QEI2IE = 1;  // enable QEI interrupt
    IPC18bits.QEI2IP = 5; // set QEI interrupt priority

    U1TXREG = 'I'; // Transmit one character
}

// interrupt service routine that resets the position counter for the QEI 1
void __attribute__((interrupt, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    if (POS1CNT < 0x7fff) //less than half of maxcount 32768
                          //longpos + 0xFFFF + 1; // overflow condition caused interrupt
        U1TXREG = 'o';
    else
        //longpos - 0xFFFF - 1; // underflow condition caused interrupt
        U1TXREG = 'u';
}

// another timer interrupt for accessing the position
