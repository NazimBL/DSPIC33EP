  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#define FCY 40535000
#define BAUDRATE 1200
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop();
//#define BRGVAL 259

#include <xc.h> // include processor files - each processor file is guarded.  

unsigned int i=0;
void PPS();

void UartInit(unsigned int baud){
     
    //RX,TX
    TRISBbits.TRISB5=1;
    TRISBbits.TRISB4=0;
     //peripheral pin select
     
    // AD1PCFGL = 0xFFFF;
    AD1CON1bits.ADON = 0;
     //peripheral pin select
    PPS();
    
 
U1MODEbits.STSEL = 0; // 1-Stop bit
U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
U1MODEbits.ABAUD = 0; // Auto-Baud disabled
U1MODEbits.BRGH = 0; // Standard-Speed mode
//U1BRG = BRGVAL; // Baud Rate setting for 9600
baud*=2;
U1BRG=((FCY/baud)/16)-1;
U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
U1STAbits.UTXISEL1 = 0;
//IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
U1STAbits.URXISEL = 0;
//IEC0bits.U1RXIE = 1;
U1MODEbits.UARTEN = 1; // Enable UART
U1STAbits.UTXEN = 1; // Enable UART TX
/* Wait at least 105 microseconds (1/9600) before sending first char */
//DELAY_105uS

}

void PPS(){

    //37
OSCCONbits.IOLOCK = 0;
RPINR18bits.U1RXR =  0x25;
RPOR1bits.RP36R=1;
OSCCONbits.IOLOCK = 1;

}

void UART_Write(char send){
	
while(U1STAbits.UTXBF== 1);
U1TXREG = send;
}

char UART_Read(){

return (char)U1RXREG;
}
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* XC_HEADER_TEMPLATE_H */

