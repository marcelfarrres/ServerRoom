#include <xc.h>
#include <pic18f4321.h>
#include "adc_interface.h"
#include "eusart_interface.h"
#include "adcConversion.h"
#include "logicJoystick.h"
#include "timer.h"
#include "menu.h"
#include "eprom.h"
#include "rgbled.h"
#include "rtcController.h"
#include "TAD_I2C.h"
#include "epromController.h"
#include "ram.h"
#include "fans.h"



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
    initLogicJoystick();
    AD_Init();
    selectChannel(1);
    initEusart();
    
    ram_Init();
    
    InitI2C();
    initFans();
    Leds_init();
    
    INTCONbits.PEIE = 0;
    TRISBbits.RB0 = 1;
    enableMinuteAlarm();
    TRISDbits.RD7 = 0;
    LATDbits.LATD7 = 0;
 
    //EPROM ESCRITO MANUAL-----------------------------
    //TOTAL ESCRITOS
    
    if(readEPROM(0) == 'F'){
        writeEEPROM(0,0);
        while(stillWriting());
        writeEEPROM(1,0);
        while(stillWriting());
    }
    //writeEEPROM(0,0);
    //while(stillWriting());
    //ULTIMO ESCRITO
    //writeEEPROM(1, 0);
    //while(stillWriting());
    /*
    for(int r = 3; r < 17; r++){
        writeEEPROM(r,1);
        while(stillWriting());
    }
     */
    
    //receiveNumber();
  
	while(1){
        
        adcConversionMotor();
        logic_joystickMotor();
        menuMotor();
        rgbLedMotor();
        epromMotor();
        ramMotor();
        fansMotor();
        LATDbits.LATD7 = !LATDbits.LATD7;

	}				
}

void enableTimerInterrups(void){
    
    INTCON = 0b11100000;
    INTCON2 = 0b00000100;
    RCONbits.IPEN = 0;
  
}




