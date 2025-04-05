#include <xc.h>
#include "adc_interface.h"
#include "eusart_interface.h"
#include "adcConversion.h"
#include "logicJoystick.h"
#include "timer.h"
#include "menu.h"
#include "eprom.h"



#pragma config OSC = HSPLL 
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

#define u 1
#define zero 0

void HighInterruptRSI(void);
void main(void);

void enableTimerInterrups(void);
void initPorts();


int tick_count;
//Important: We CANNOT call interrupting functions from our code 
//The returns from the interrupts are different than the returns from the rest of the functions.
#if __XC8_VERSION >= 2000
    extern void __interrupt (high_priority) HighRSI (void){
                if (INTCONbits.TMR0IF==1) RSI_Timer0();
    }
    extern void __interrupt (low_priority) LowRSI (void){
        
    }
#else
    extern void interrupt HighRSI(void){ // High priority interrupt
        
    }
    extern void interrupt low_priority LowRSI(void){ //Low priority interrupt
        
    }
#endif
#define _XTAL_FREQ 4000000  // Adjust based on your crystal frequency

void main(void){
	
    enableTimerInterrups();
    TI_Init();
    initPorts();
    initLogicJoystick();

    AD_Init();
    selectChannel(1);
    initEusart();
    
    
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    
    LATAbits.LA3 = 0;
    LATAbits.LA4 = 0;
    
    //EPROM ESCRITO MANUAL-----------------------------
    for(int r = 3; r < 17; r++){
        writeEEPROM(r,'1');
        while(stillWriting());
    }
    for(int r = 17; r < 31; r++){
        writeEEPROM(r,'2');
        while(stillWriting());
    }
    for(int r = 31; r < 45; r++){
        writeEEPROM(r,'3');
        while(stillWriting());
    }
    
	while(1){
        
        adcConversionMotor();
        logic_joystickMotor();
        menuMotor();
       
        

	}				
}

void enableTimerInterrups(void){
    
    INTCON = 0b11100000;
    INTCON2 = 0b00000100;
    RCONbits.IPEN = 0;
  
}

void initPorts(){

    TRISBbits.TRISB1 = 1; 
}



