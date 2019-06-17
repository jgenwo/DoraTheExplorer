#include "qei.h" 
#include "xc.h"
#include "p33FJ128MC802.h"
   

void init_QEI(void)
{
    RPINR14bits.QEA1R = 00001100; // Configure QEI pin 12 channel A 
    RPINR14bits.QEB1R = 00001101; // Configure QEI pin 13 channel B    
    
    // Configure QEI module
    
    // QEIEN - QEI enable/disable  Count Error Status Flag bit
    //QEI1CONbits.QEIEN = 0; //CNTERR
    // QEISIDL - Idle mode 0=continue, 1=stop
    QEICONbits.QEISIDL = 1; //QEICONbits.QEISIDL // Discontinue operation when in idle mode
    //INDEX: Index Pin State Status bit (read-only)
    //QEI1CONbits.INDEX = 0; // 1 = Index pin is high 0 = Index pin is low
    //UPDN: Position Counter Direction Status bit
    QEI1CONbits.UPDN = 0; // 1 positive/0 negative

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
    QEI1CONbits.QEIM = 0;
    
    //SWPAB: Phase A and Phase B Input Swap Select bit
    QEI1CONbits.SWPAB = 0; // Do not swap the polarity of phase comparison
    
    //PCDOUT: Position Counter Direction State Output Enable bit 
    QEICONbits.PCDOUT = 0; // Disable counter direction pin (normal I/O operation)
    
    // TQGATE: Timer Gated Time Accumulation Enable bit
    QEICONbits.TQGATE = 0;
    
    //POSRES: Position Counter Reset Enable bit
    QEICONbits.POSRES = 0; // index pulse does not reset POSCNT
    
    //TQCS: Timer Clock Source Select bitTQCS: Timer Clock Source Select bit
    QEICONbits.TQCS = 0; // internal clock source (Tcy)
    

    DFLTCONbits.QEOUT = 0; // disable digital filters bit 7
    
    // set initial counter value and maximum range
    MAXCNT = 0xffff; // set the highest possible time out
    POSCNT = 0x7fff; // set POSCNT into middle of range
}
    /*
     
    // Configure Interrupt controller
    IFS2bits.QEIIF = 0; // clear interrupt flag
    IEC2bits.QEIIE = 1; // enable QEI interrupt
    IPC10bits.QEIIP = 5; // set QEI interrupt priority
    }


    // interrupt service routine
   void __attribute__((interrupt, auto_psv)) _QEIInterrupt(void)
    {
    IFS2bits.QEIIF = 0; // clear interrupt flag
    if(POSCNT<32768)
    xx_xx++; // over-run condition caused interrupt
    else
    xx_xx--; // under-run condition caused interrupt
    }
     * 
     * */

