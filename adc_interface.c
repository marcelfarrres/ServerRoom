
#include <xc.h>

#include "adc_interface.h"

void AD_Init(){
    
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ADCON1 = 0b00001101; // AN0-AN1 as analog and Vmax and Vmin Default 0-5V
    ADCON2 = 0b10001011; //Right justified and time and clock bits
    ADCON0bits.ADON = 1;

}

void selectChannel(unsigned char whichChannel){
    //ADCON0 = (whichChannel << 2) + 1; //we select whichChannel and activate the ADC
    ADCON0bits.CHS = whichChannel;
}

void AD_StartConversion(void){
    ADCON0bits.GODONE = 1;
}


char AD_SampleAvailable(void){
    if(ADCON0bits.GODONE == 0){
        return 1;
    }else{
        return 0;
    }
}

int AD_GetSample_10bits(void){
    return ((ADRESH << 8) | ADRESL);

}

unsigned char AD_GetSample_8bits(void){
   
    return ((ADRESH << 8) | ADRESL) >> 2;

}




