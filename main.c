#include <xc.h>
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

    void sendByteAsASCII(unsigned char num) {
        unsigned char d = 0;
        while (num >= 10) { num -= 10; d++; }

        sendBits(d + '0');  // decenas
        while (!numSentCorrectly());

        sendBits(num + '0');  // unidades
        while (!numSentCorrectly());
        sendBits(num + '\r');  // unidades
        while (!numSentCorrectly());
        sendBits(num + '\n');  // unidades
        while (!numSentCorrectly());
        
        
}
    
void main(void){
	
    enableTimerInterrups();
    TI_Init();
    initPorts();
    initLogicJoystick();

    AD_Init();
    selectChannel(1);
    initEusart();
    Leds_init();

    InitI2C();
    
    ram_Init();

    
   
   
    
    
    
    //EPROM ESCRITO MANUAL-----------------------------
    //TOTAL ESCRITOS
    writeEEPROM(0, 14);
    while(stillWriting());
    //ULTIMO ESCRITO
    writeEEPROM(1, 8);
    while(stillWriting());
    
    for(int r = 3; r < 17; r++){
        writeEEPROM(r,1);
        while(stillWriting());
    }
    for(int r = 17; r < 31; r++){
        writeEEPROM(r,2);
        while(stillWriting());
    }
    for(int r = 31; r < 45; r++){
        writeEEPROM(r,3);
        while(stillWriting());
    }
    for(int r = 45; r < 59; r++){
        writeEEPROM(r,4);
        while(stillWriting());
    }
    for(int r = 59; r < 73; r++){
        writeEEPROM(r,5);
        while(stillWriting());
    }
    for(int r = 73; r < 87; r++){
        writeEEPROM(r,6);
        while(stillWriting());
    }
    for(int r = 87; r < 101; r++){
        writeEEPROM(r,7);
        while(stillWriting());
    }
    for(int r = 101; r < 115; r++){
        writeEEPROM(r,8);
        while(stillWriting());
    }
    for(int r = 115; r < 129; r++){
        writeEEPROM(r,9);
        while(stillWriting());
    }
    for(int r = 129; r < 143; r++){
        writeEEPROM(r,0);
        while(stillWriting());
    }
    for(int r = 143; r < 157; r++){
        writeEEPROM(r,0);
        while(stillWriting());
    }
    for(int r = 157; r < 171; r++){
        writeEEPROM(r,1);
        while(stillWriting());
    }
    for(int r = 171; r < 185; r++){
        writeEEPROM(r,1);
        while(stillWriting());
    }
    for(int r = 185; r < 199; r++){
        writeEEPROM(r,0);
        while(stillWriting());
    }
    for(int r = 199; r < 213; r++){
        writeEEPROM(r,0);
        while(stillWriting());
    }
    for(int r = 213; r < 226; r++){
        writeEEPROM(r,7);
        while(stillWriting());
    }
    

    
    //RTC PRUEBAS-------------------------------------------------------------------------
    //setRTC(30, 45, 13, 1, 14, 4, 24); // Esto pone: 13:45:30 lunes 14 de abril de 2024

    //unsigned char sec, min, hour, day, date, month, year;

    //getRTC(&sec, &min, &hour, &day, &date, &month, &year);

    //sendByteAsASCII(sec);
    //sendByteAsASCII(min);
    //sendByteAsASCII(hour);
    //-------------------------------------------------------------------------
	while(1){
        
        adcConversionMotor();
        logic_joystickMotor();
        menuMotor();
        rgbLedMotor();
        epromMotor();
        ramMotor();

	}				
}

void enableTimerInterrups(void){
    
    INTCON = 0b11100000;
    INTCON2 = 0b00000100;
    RCONbits.IPEN = 0;
  
}

void initPorts(){

    //TRISBbits.TRISB1 = 1; 
}



