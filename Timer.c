/*
 * MPLAB X IDE
 * Author: Nazim BL
 * Created on 5 juillet 2018, 10:54
 */

#include "xc.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);
// Enable Clock Switching and Configure Primary Oscillator in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

#define FCY 40535000
#define LED	LATAbits.LATA0

void SetupOSC();
void Timer_Setup();

//Timer ISR
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{  
IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
LED=!LED; 
}

int main(void) {
    
    SetupOSC();
    Timer_Setup();
    TRISAbits.TRISA0=0;     
    LED=0;
    while (1);
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

void Timer_Setup(){

T3CONbits.TON = 0; // Disable Timer
T3CONbits.TCS = 0; // Select internal instruction cycle clock
T3CONbits.TGATE = 0; // Disable Gated Timer mode
T3CONbits.TCKPS = 0b11; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
//PR3 = 19999; // Load the period value
PR3 = 65000; 
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;
}
