#include <xc.h>

#include "ram.h"
#include "timer.h"
#include "eusart_interface.h"
#include "adcConversion.h"



#define LOG_LENGTH 14

static unsigned char timerRam = 0;

void ram_Init(){
    TI_NewTimer(&timerRam);
    TRISAbits.TRISA4 = 0; //ReadNotWrite
    TRISAbits.TRISA5 = 0; //CHIP SELECT
    
    TRISDbits.TRISD2 = 0; //CLOCK RAM
    TRISDbits.TRISD3 = 0; //RESET RAM
    
}


void setPortsInOut(char inOut){
    TRISDbits.TRISD1 = inOut; //LSB bit 0
    TRISDbits.TRISD0 = inOut; //    bit 1
    TRISCbits.TRISC2 = inOut; //    bit 2
    TRISCbits.TRISC1 = inOut; //    bit 3
    TRISCbits.TRISC0 = inOut; //    bit 4
    TRISEbits.TRISE2 = inOut; //    bit 5
    TRISEbits.TRISE1 = inOut; //    bit 6
    TRISEbits.TRISE0 = inOut; //MSB bit 7
    

}

void setReadNotWrite(char readNotWrite){
    LATAbits.LA4 = readNotWrite;

}

void setChipSelect(char chipSelect){
    LATAbits.LA5 = chipSelect;

}

void setclockCounter(char clockRam){
    LATDbits.LATD2 = clockRam;

}

void setResetRam(char resetRam){
    LATDbits.LATD3 = resetRam;

}

void escribirNumeroEnPines(int valor) {
   
    LATDbits.LATD1 = (valor >> 0) & 0x01;
    LATDbits.LATD0 = (valor >> 1) & 0x01;
    LATCbits.LATC2 = (valor >> 2) & 0x01;
    LATCbits.LATC1 = (valor >> 3) & 0x01;
    LATCbits.LATC0 = (valor >> 4) & 0x01;
    LATEbits.LATE2 = (valor >> 5) & 0x01;
    LATEbits.LATE1 = (valor >> 6) & 0x01;
    LATEbits.LATE0 = (valor >> 7) & 0x01;
}


