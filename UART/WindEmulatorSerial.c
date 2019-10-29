/*
 * File:   main.c
 * Author: DELL
 *
 * Created on 30 août 2018, 11:48
 */


#include "xc.h"
#include "MyUart.h"

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);

#define FCY 40000000
#define LED	LATBbits.LATB10

int ADCValue=500;
char temp;
char received=0;
void SetupOSC();
void ADC_Read();
void ADC_Setup();
void Timer_Setup();
void myDelay();
void sendInt(int val);
int windSpeed=0;


int main(void) {
    
	SetupOSC();
	ADC_Setup();
	Timer_Setup();
	
    while(1)
    {
        
     if(U1STAbits.URXDA==1){
        
     received=U1RXREG;
     //higihbyte
     windSpeed=(int)(received*256);
     received=U1RXREG;
     //lowbyte
     windSpeed+=received;
       
    }
     sendInt(ADCValue);
     myDelay();
        
    }       
        return 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  
IFS0bits.T3IF = 0; 
 LED=!LED;
 ADC_Read(); 
}

void sendInt(int val){
    char highB=val / 256;
    char lowB=val % 256;
     UART_Write(highB);
     UART_Write(lowB);
    
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
T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
//PR3 = 19999; // Load the period value
//20 khz
PR3 = 2000; 
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;

}

void ADC_Setup(){

 TRISBbits.TRISB0=1; 
 ANSELB=0x0001;
 ANSELBbits.ANSB0=1;
 //10 bit
 //AD1CON1 = 0x00E0; 
 //12 bit
 AD1CON1 = 0x04E0; 
 
 AD1CHS0= 0x0002; // Connect RA0/AN0 as CH0 input ..
 
 AD1CSSL = 0;
 AD1CON3 = 0x1F02; // Tsampling = 2 Tad
 AD1CON2 = 0;
 AD1CON1bits.ADON = 1; // turn ADC ON
 
}
void ADC_Read(){
 AD1CON1bits.SAMP = 1;
 while (!AD1CON1bits.DONE);// conversion done?
 ADCValue = ADC1BUF0;
}
void myDelay(){
    
T3CONbits.TON = 0; // Disable Timer
T3CONbits.TCS = 0; // Select internal instruction cycle clock
T3CONbits.TGATE = 0; // Disable Gated Timer mode
T3CONbits.TCKPS = 0b11; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear timer register
PR3 = 65200; // Load the period value
//IPC0bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer1 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer1 interrupt
T3CONbits.TON = 1;

while(IFS0bits.T3IF == 0); 
IFS0bits.T3IF = 0;     
}
