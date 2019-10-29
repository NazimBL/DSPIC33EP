/*
 * File:   main.c
 * Author: Nazim BL
 *
 * Created on 5 juillet 2018, 10:54
 */

#include "xc.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);
// Enable Clock Switching and Configure Primary Oscillator in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

#define FCY 40535000

void SetupOSC();


char ReceivedChar='v';
int main(void) {
    
    SetupOSC();
    UartInit(4800);
      
	if(U1STAbits.URXDA==1){
        
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     ReceivedChar=(char)U1RXREG;
     UART_Write(ReceivedChar);
     UART_Write('\r');
     UART_Write('\n');
    }
   
   /*
    decomment to write
    UART_Write('1');
    UART_Write('2');
    UART_Write('3');
    */
            
    return 0;
}
 
void SetupOSC(){

PLLFBD=31; // M=33
CLKDIVbits.PLLPOST=0; // N2=2
CLKDIVbits.PLLPRE=1; // N1=3
__builtin_write_OSCCONH(0x01);
__builtin_write_OSCCONL(OSCCON | 0x01);
// Wait for Clock switch to occur
while (OSCCONbits.COSC!= 0b001);
// Wait for PLL to lock
while (OSCCONbits.LOCK!= 1);
}
