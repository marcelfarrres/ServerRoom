#include <xc.h>
#include "pic18f4321.h"
#include "rgbLed.h"

#include "timer.h"
#include "adcConversion.h"


static unsigned char timerLeds[3];
static unsigned char timerLed;


void Leds_init(void) {
    
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;

   
    for (unsigned char i = 0; i < 3; i++) {
        TI_NewTimer(&timerLeds[i]);
    }
    TI_NewTimer(&timerLed);
}

static unsigned char modulation[3][3] =  { {0,9,0}, {0,0,9}, {9,0,0} };
static unsigned char roomState = 0;

void setRoomState(unsigned char newState){
    roomState = newState;
}


static unsigned char blueThreshold = 27;
static unsigned char redThreshold = 32;
static unsigned char magentaThreshold = 35;

void setThresholds(unsigned char blue, unsigned char red, unsigned char magenta){
    blueThreshold = blue;
    redThreshold = red;
    magentaThreshold = magenta;
}

void rgbLedMotor(void) {
	static char state = 0;
    static unsigned char magentaCounter = 0;

	switch(state) {
		case 0:
			TI_ResetTics(timerLed);
			state = 1;
		break;
		case 1:
			if (getTemp() < magentaThreshold && TI_GetTics(timerLed)  >= 1) {
				LATB &= ~(0b00011100);
				if(getTemp() < blueThreshold){
					 LATB |= (1 << 3);
                     
				}else if(getTemp() < redThreshold){
					LATB |= (1 << 4);
                    
				}else{
					LATB |= (1 << 2);
                    
				}
				state = 0;
			}
			else if (getTemp() >= magentaThreshold && TI_GetTics(timerLed)  >= magentaCounter) {
				LATB &= ~(0b00011100);
				LATB |= (1 << 2);
				LATB |= (1 << 4);
				TI_ResetTics(timerLed);
				state = 2;
			}
		break;
		case 2:
			if ( TI_GetTics(timerLed)  >= 10 - magentaCounter) {
				LATB &= ~(0b00011100);
				LATB |= (1 << 2);
				magentaCounter++;
				state = 3;
			}
		break;
		case 3:
			if (magentaCounter >= 10) {
				magentaCounter = 0;
				state = 0;
			}
			else if (magentaCounter < 10) {
				state = 0;
			}
		break;
	}
}