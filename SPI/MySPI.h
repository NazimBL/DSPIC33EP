/* 
 * File: SPI Header   
 * Author: Nazim BL
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


void SPI_Init(char master){
    
    //data in
     RPINR20bits.SDI1R0=0;
    //data out
     RPOR1bits.RP2R=7;
     SPI1CON1bits.MSTEN = master; // Master mode enabled
    
    if(master==1){
        //clock out
        RPOR1bits.RP3R=8;              
    }
    else {
        //SCK  and CS
        RPINR20bits.SCK1R0=1;
        RPINR21bits.SS1R0=4;   
    }
   
    
    //rp0 = Data IN
    //rp1 = SCK
    //rp2 = Data OUT
    //rp3 = master clock
    
   TRISBbits.TRISB0=1;
   TRISBbits.TRISB2=0;
   if(master==1)TRISBbits.TRISB3=0;
   else TRISBbits.TRISB1=1;
    
IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
IEC0bits.SPI1IE = 0; // Disable the interrupt

// SPI1CON1 Register Settings
SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
SPI1CON1bits.MODE16 = 0; // Communication is word-wide (16 bits)
SPI1CON1bits.PPRE = 2; //Pre-scaler
SPI1CON1bits.SPRE = 8;//Pre-scaler 2
 // Input data is sampled at the middle of data output time
SPI1CON1bits.CKE = 0; // Serial output data changes on transition from
// Idle clock state to active clock state
SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;
SPI1CON1bits.SMP = 0;
// active state is a high level
SPI1STATbits.SPIEN = 1; // Enable SPI module
// Interrupt Controller Settings
IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
IEC0bits.SPI1IE = 1; // Enable the interrupt
} 

char SPI_Writ(char w){
  
  SPI1BUF=w;
  while (SPI1STATbits.SPITBF); 
  return SPI1BUF;
}

char SPI_Read()       
{
 
 //while (SPI1STATbits.SPITBF); 
 while(!SPI1STATbits.SPIRBF);
 return SPI1BUF; 
 // read a byte
}

#endif	/* XC_HEADER_TEMPLATE_H */

