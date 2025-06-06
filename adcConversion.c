#include <xc.h>

#include "adcConversion.h"

#include "adc_interface.h"



#define NUM_OF_ANS 3

static int ansArray[NUM_OF_ANS];
static int goofyAhhCounter = 0;


void adcConversionMotor(void) {
	static char state = 0;
    static char i = 0;

	switch(state) {
		case 0:
			i = 0;
			state = 1;
		break;
		case 1:
			selectChannel(i);
			AD_StartConversion();
			state = 2;
		break;
		case 2:
			if (AD_SampleAvailable() == 1) {
				AD_StartConversion();
				state = 4;
			}
		break;
		case 3:
			if (i >= NUM_OF_ANS) {
				state = 0;
			}
			else if (i < NUM_OF_ANS) {
				state = 1;
			}
		break;
		case 4:
			if (AD_SampleAvailable() == 1) {
                goofyAhhCounter++;
                if(i == 2 && goofyAhhCounter >= 500){
                        ansArray[i] = ((AD_GetSample_10bits() * 100) / 205) - 50;
                        //ansArray[i] = AD_GetSample_8bits();
                        
                        goofyAhhCounter = 0;
                  
                }else if(i != 2){
                    ansArray[i] = AD_GetSample_10bits();
                    
                }
                i++;
                state = 3;
			}
		break;
	}
}

int getXjoystick(){

    return ansArray[0];
}

int getYjoystick(){

    return ansArray[1];
}

int getTemp(){
    return ansArray[2];
}
