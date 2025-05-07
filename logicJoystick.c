#include <xc.h>

#include "logicJoystick.h"

#include "eusart_interface.h"
#include "adcConversion.h"
#include "timer.h"
#include "rtcController.h"


#define HIGH_THRESHOLD 1000
#define LOW_THRESHOLD 24
#define TI_BOUNCES 16



static unsigned char buttonTimer;

//MESSAGES:  PIC->PC------------------------------------------------------
static const unsigned char joyMaxIndex[4] = {4, 6, 6, 7};
static const unsigned char joyMessages[4][7] = {
    "UP\r\n   ",
    "DOWN\r\n ",
    "LEFT\r\n ",
    "RIGHT\r\n",
};

#define buttonMaxIndex 8
static const unsigned char buttonMessage[buttonMaxIndex] = "SELECT\r\n";
//------------------------------------------------------------------------


void initLogicJoystick(void){
  
    TRISAbits.RA3 = 1;
    TI_NewTimer(&buttonTimer);
    
}


void logic_joystickMotor(void) {
	static char state = 0;
    static char yUpFlag = 0;
    static char yDownFlag = 0;
    static char xUpFlag = 0;
    static char xDownFlag = 0;
    static char joyIndex = 0;
    static char sendFlag = 0;
    static char sendSequence = 0;
    

	switch(state) {
		case 0:
            if (PORTAbits.RA3 == 0) {
				TI_ResetTics(buttonTimer);
				state = 4;
			}
			
			else if ( getYjoystick() < LOW_THRESHOLD) {
				if(yDownFlag == 0){
					sendFlag = 1;
					sendSequence = 1;
					yDownFlag = 1;
				}
				state = 1;
			}
			
			else if ( getYjoystick() > HIGH_THRESHOLD) {
				if(yUpFlag == 0){
					sendFlag = 1;
					sendSequence = 0;
					yUpFlag = 1;
				}
				state = 1;
			}
            else if (getYjoystick() >= (LOW_THRESHOLD + 10) && getYjoystick() <= (HIGH_THRESHOLD - 10)) {
				yDownFlag = 0;
				yUpFlag = 0;
				state = 1;
			}
		break;
		case 1:
			if ( getXjoystick() < LOW_THRESHOLD) {
				if(xDownFlag == 0){
					sendFlag = 1;
					sendSequence = 3;
					xDownFlag = 1;
				}
                
				state = 2;
			}
			else if ( getXjoystick() > HIGH_THRESHOLD) {
				if(xUpFlag == 0){
					sendFlag = 1;
					sendSequence = 2;
					xUpFlag = 1;
				}
				state = 2;
			}
			else if (getXjoystick() >= (LOW_THRESHOLD + 10) && getXjoystick() <= (HIGH_THRESHOLD - 10)) {
				xDownFlag = 0;
				xUpFlag = 0;
				state = 2;
			}
		break;
		case 2:
			if (sendFlag == 0) {
				state = 0;
			}
			else if (sendFlag == 1) {
				sendFlag = 0;
				joyIndex = 0;
				sendBits(joyMessages[sendSequence][joyIndex]);
				joyIndex++;
				state = 3;
			}
		break;
		case 3:
			if (numSentCorrectly() && joyIndex >= joyMaxIndex[sendSequence]) {
                
				state = 0;
			}
			else if (numSentCorrectly() && joyIndex < joyMaxIndex[sendSequence]) {
				sendBits(joyMessages[sendSequence][joyIndex]);
				joyIndex++;
			}
		break;
		case 4:
			if (TI_GetTics(buttonTimer) >= TI_BOUNCES && PORTAbits.RA3 == 0) {
				joyIndex = 0;
				sendBits(buttonMessage[joyIndex]);
				joyIndex++;
                
				state = 5;
			}
			else if (TI_GetTics(buttonTimer) >= TI_BOUNCES && PORTAbits.RA3 == 1) {
				state = 0;
			}
		break;
		case 5:
			if (numSentCorrectly() && joyIndex < buttonMaxIndex) {
				sendBits(buttonMessage[joyIndex]);
				joyIndex++;
			}
			else if (numSentCorrectly() && joyIndex >= buttonMaxIndex) {
				state = 6;
			}
		break;
		case 6:
           
			if (PORTAbits.RA3 == 1) {
				TI_ResetTics(buttonTimer);
				state = 7;
                
         
			}
		break;
		case 7:
			if (TI_GetTics(buttonTimer) >= TI_BOUNCES && PORTAbits.RA3 == 1) {
				state = 0;
			}
			else if (TI_GetTics(buttonTimer) >= TI_BOUNCES && PORTAbits.RA3 == 0) {
				state = 6;
			}
		break;
	}
}