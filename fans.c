#include <xc.h>

#include "fans.h"

#include "rgbLed.h"
#include "timer.h"


static unsigned char timerFan = 0;
static unsigned char Period = 5;

void initFans(void){
  
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TI_NewTimer(&timerFan);
    
}


void fansMotor(void) {
	static unsigned char state = 0;
	static unsigned char timeHigh = 0;

	switch(state) {
		case 0:
            LATDbits.LATD5 = 1;
            if(getSystemState() == 0){
                LATDbits.LATD4 = 0;
				//LATDbits.LATD5 = 0;
                break;
            }
			TI_ResetTics(timerFan);
			if(getRoomState() == 0){
				LATDbits.LATD4 = 1;
				//LATDbits.LATD5 = 0;
				timeHigh = 2;
			}else if(getRoomState() == 1){
				LATDbits.LATD4 = 1;
				//LATDbits.LATD5 = 1;
				timeHigh = 2;
			}else if(getRoomState() == 2){
				LATDbits.LATD4 = 1;
				//LATDbits.LATD5 = 1;
				timeHigh = 4;
			}else{
				LATDbits.LATD4 = 0;
				//LATDbits.LATD5 = 0;
			}
			state = 1;
		break;
		case 1:
			if (TI_GetTics(timerFan) >= timeHigh) {
				TI_ResetTics(timerFan);
				LATDbits.LATD4 = 0;
				//LATDbits.LATD5 = 0;
				state = 2;
			}
		break;
		case 2:
			if (TI_GetTics(timerFan) >= Period - timeHigh ) {
				state = 0;
			}
		break;
	}
}
