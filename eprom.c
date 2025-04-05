
#include <xc.h>

#include "eprom.h"
//#include "eusart_interface.h"



void writeEEPROM(unsigned char i, unsigned char data) {
    
        EEADR = i;
        EEDATA = data;
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EECON1bits.WREN = 1;

        INTCONbits.GIE = 0; // Disable interrupts
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1;
        INTCONbits.GIE = 1; // Enable interrupts

        //STILLWRITING MUST BE USED
}

char stillWriting(){
    if(EECON1bits.WR){
        return 1;
    }else{
        EECON1bits.WREN = 0;
        return 0;
    } 

}



unsigned char readEPROM(char i) {
    
    
    EEADR = i;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
        
    
    return EEDATA;
}


//PUBLIC FUNCTIONS

void readFromEprom(){

}







/*
 * static unsigned char list[10] = {'1','2','3','4','5','6','7','8','9','\n'};
 
void epromMotor(){
    static unsigned char state = 0;
    static unsigned char i = 0;
    
    switch (state){
        
        case 0:
            writeEEPROM(i, list[i]);
            state = 1;
            break;
            
        case 1:
            if(!stillWriting()){
                if(i < 10){
                    i++;
                    state = 0;
                
                }else{
                    state = 2;
                    i = 0;
                }
            }
            
            break;
            
        case 2:
            while(!numSentCorrectly());
            sendBits(readEPROM(i));
            
            if(i >= 10){
                i = 0;
                state = 3;
            }else{
                i++;
            }
            break;
            
        case 3:
            state = 2;
            
            
        
    }
    

}
 */