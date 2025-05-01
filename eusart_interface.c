
#include <xc.h>

#include "eusart_interface.h"

void initEusart(void){
   
    
    //Configure the EUSART ports
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    
    //Configure the BaudRate: 
    TXSTAbits.SYNC = 0;
    BAUDCONbits.BRG16 = 1;
    TXSTAbits.BRGH = 1;
    SPBRGH = (8332 >> 8) & 0xFF;  //1040 for 9600     86 for 115k   520 for 19k //8332 for 1200
    SPBRG  = 8332 & 0xFF;         


    
    //Configure the rest of the registers
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    BAUDCONbits.RXDTP = 0;
    BAUDCONbits.TXCKP = 0;
    BAUDCONbits.WUE = 1;
    BAUDCONbits.ABDEN = 0;
    
    
    
    
    
    
}


void sendBits(unsigned char value){
    TXREG = value;
}

unsigned char numSentCorrectly(void){
    if(TXSTAbits.TRMT == 1){
        return 1;
    }else{
        return 0;
    }
}

unsigned char numReceivedAtRCREG(void){
    if (PIR1bits.RCIF == 1){
        return 1;
    } else{
        return 0;
    }
}

unsigned char receiveNumber(void){
    return RCREG;
}
