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

unsigned char getRoomState(){
    return roomState;
}


static unsigned char blueThreshold = 10;
static unsigned char redThreshold = 20;
static unsigned char magentaThreshold = 30;

void setThresholds(unsigned char blue, unsigned char red, unsigned char magenta){
    blueThreshold = blue;
    redThreshold = red;
    magentaThreshold = magenta;
}

void rgbLedMotor(void) {
	static char state = 0;
    static unsigned char magentaCounter = 0;
    static unsigned char Period = 22;
    static unsigned char direction = 0;
    

	switch(state) {
		case 0:
			TI_ResetTics(timerLed);
            if( getTemp()  >= magentaThreshold){
                LATB &= ~(0b00011100);
				LATB |= (1 << 2);
				LATB |= (1 << 4);
            }
			state = 1;
		break;
		case 1:
			if (getTemp() < magentaThreshold && TI_GetTics(timerLed)  >= 1) {
				LATB &= ~(0b00011100);
                
				if(getTemp() < blueThreshold){
					LATB |= (1 << 3);
					setRoomState(0);
				}else if(getTemp() < redThreshold){
					LATB |= (1 << 4);
					setRoomState(1);
				}else if(getTemp() < magentaThreshold){
					LATB |= (1 << 2);
					setRoomState(2);
				}
				state = 0;
			}
			else if (getTemp()  >= magentaThreshold && TI_GetTics(timerLed)  >= magentaCounter) {
				LATB &= ~(0b00011100);
				
				LATB |= (1 << 2);
				setRoomState(3);
				TI_ResetTics(timerLed);
				state = 2;
                
                if(direction == 0){
                    magentaCounter++;
                    if(magentaCounter >= Period){
                        direction = 1;
                    }
                }else{
                    magentaCounter--;
                    if(magentaCounter <= 0){
                        direction = 0;
                    }
                }
			}
		break;
		case 2:
			if ( TI_GetTics(timerLed)  >= (Period - magentaCounter)) {
				
				
				state = 0;
			}
		break;
		
	}
}