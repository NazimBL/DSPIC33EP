/*
 * MPLAB X IDE
 * Author: Nazim BL
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

    while (1){      
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

void PWM_Sync(float d){
	
    d=d*2;
    if(d>=1){
     d=d-1;
     if((d*800)<600)PWM_Update((int)(d*800),800);
     else PWM_Update(600,800);   
    }
    else PWM_Update(0,(int)(d*800));  
}

void PWM_Update(int pwm1,int pwm2){
  
PDC1 = pwm2;
PDC2 = pwm1;
}

void PWM_Init(int p1,int p2)
{
TRISBbits.TRISB12=0;
TRISBbits.TRISB14=0;
// RB14 and Rb12
/* Set PWM Period on Primary Time Base */
PTPER = 800;
//PHASE1 = 500;
/* Set Duty Cycles */
PDC1 = p1;
PDC2 = p2;
IOCON1 = IOCON2=0xC000;
/* Set Primary Time Base, Edge-Aligned Mode and Independent Duty Cycles */
PWMCON1 = PWMCON2 =0x0001;
/* Configure Faults */
FCLCON1 = FCLCON2 =0x0003;
/* 1:1 Prescaler */
PTCON2 = 0x0000;
/* Enable PWM Module */
//chancege to 8000
PWMCON1bits.IUE = 1;
PWMCON2bits.IUE = 1;
PTCON = 0x8000;
PTCONbits.EIPU = 1;
}

int ADC_Read(){
		
AD1CON1bits.SAMP = 1;
while (!AD1CON1bits.DONE); // conversion done?
return ADC1BUF0;    
}

//read each 5khz
void ADC_Setup(){

 TRISBbits.TRISB0=1; 
 ANSELB=0x0001;
 ANSELBbits.ANSB0=1;
 //10 bit //AD1CON1 = 0x00E0; 
 //12 bit
 AD1CON1 = 0x04E0; 
 
 AD1CHS0= 0x0002; // Connect RA0/AN0 as CH0 input ..
 
 AD1CSSL = 0;
 AD1CON3 = 0x1F02; // Tsampling = 2 Tad
 AD1CON2 = 0;
 AD1CON1bits.ADON = 1; // turn ADC ON 
}
