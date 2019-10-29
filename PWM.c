/*
 * File:   main.c
 * Author: DELL
 * Bcuk Boost Converter Application
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
int ADC_Read();
void ADC_Setup();
void PWM_Init(int p1,int p2);
void PWM_Update(int pwm1,int pwm2);
void PWM_Sync(float d);

float d=1;
int p1=100,p2=100;


int main(void) {
    
    SetupOSC();
	ADC_Setup();
    PWM_Init(200,200);
	
    LED=1;
   
	while (1)
	{      
	d=(float)ADCRead()/4095;
    PWM_Sync(d);
	}    
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

vvoid PWM_Init()
{
    TRISB = 0x00; // make sure PWM pins are set to be outputs
    PORTB = 0x00; // clear the outputs 
 
    PTCONbits.PTOPS = 1; // PWM timer post-scale
    PTCONbits.PTCKPS = 0; // PWM timer pre-scale
    PTCONbits.PTMOD = 2; // PWM operates in Up-down Mode continuously
 
    PTMR = 0; // PWM counter value, start at 0
 
    PTPER = 19999; // PWM Timebase period
 
    PWMCON1bits.PMOD3 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD2 = 0; // PWM in complimentary mode
    PWMCON1bits.PMOD1 = 0; // PWM in complimentary mode
    PWMCON1bits.PEN3H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN2H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN1H = 1; // PWM High pin is enabled
    PWMCON1bits.PEN3L = 1; // PWM Low pin enabled (direction control later?)
    PWMCON1bits.PEN2L = 1; // PWM Low pin enabled (direction control later?)
    PWMCON1bits.PEN1L = 1; // PWM Low pin enabled (direction control later?)
 
    DTCON1bits.DTAPS = 0;  //DeadTime pre-scaler
    DTCON1bits.DTA = 59;   //DeadTime value for 4 us. 
 

    PDC1 = 19999; // PWM#1 Duty Cycle register (11-bit)
    PDC2 = 19999; // PWM#2 Duty Cycle register (11-bit)
    PDC3 = 19999; // PWM#3 Duty Cycle register (11-bit)
 
    PTCONbits.PTEN = 1; // Enable PWM Timerbase!
}

int ADC_Read(){
		
		AD1CON1bits.SAMP = 1;
		while (!AD1CON1bits.DONE);// conversion done?
		return ADC1BUF0;    
}

//read each 5khz
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