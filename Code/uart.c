/*
 * File:   uart.c
 * Author: Natalia
 *
 * Created on June 10, 2019, 10:26 PM
 */

#include "uart.h"
#include<string.h>

void initUART()
{
    RPOR4bits.RP8R = 0b00011;
    RPINR18bits.U1RXR = 7;
    IPC3bits.U1TXIP = 4;

    U1MODEbits.STSEL = 0;   // 1-Stop bit
    U1MODEbits.PDSEL = 0;   // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0;   // Auto-Baud disabled
    U1MODEbits.BRGH = 0;    // Standard-Speed mode
    U1BRG = BRGVAL;         // Baud Rate setting for BAUDRATE
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.URXISEL =0b00; // Interrupt after one RX character is received;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    IPC2bits.U1RXIP = 4;
    IEC0bits.U1TXIE = 1;   // Enable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1;   // Enable UART TX
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    DELAY_105uS
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; 
}

/**
 * 
 * @param msg sends this message up to length 4
 */
void send(char msg[]){
    int i, j;
    int n = strlen(msg);
    if (n < 5){
        for (i = 0; i < n ; i++){
            U1TXREG = msg[i];
        } 
        U1TXREG = '\n';
    }
    else{
        for (j = 0; j <= n/4; j++){
            for (i = 0; i < 4 && j*4+i < n; i++){
                U1TXREG = msg[j*4+i];
            }
            for (i = 0; i < 20000; i++){
            //wait
            }
        }
        U1TXREG = '\n';
    }    
}
    char my_char;

//=================================
/* UART1 Receive Interrupt Service Routine */

void __attribute__((__interrupt__)) _U1RXInterrupt(void)
{
  IFS0bits.U1RXIF = 0;            
  my_char = U1RXREG;  
  char msg[50] = "UPDATE{F:";
  strcat(msg,"5");
  strcat(msg,";R:");
  strcat(msg,"5");
  strcat(msg,";L:");
  strcat(msg,"5");
  strcat(msg,";X:");
  strcat(msg,"4");
  strcat(msg,";Y:");
  strcat(msg,"1");
  strcat(msg,";ER:");
  strcat(msg,"4056");
  strcat(msg,";EL:");
  strcat(msg,"3000");
  strcat(msg,";}");  
  send(msg);
}
