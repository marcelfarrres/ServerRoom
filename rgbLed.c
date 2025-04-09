#include <xc.h>
#include "pic18f4321.h"
#include "rgbLed.h"

#include "timer.h"

static const unsigned int pwmOnTimes[18] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 0, 0, 0, 0, 0, 0, 0, 20};
static const unsigned int pwmOffTimes[18] = {20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0};

static unsigned char timerLeds[3];


void Leds_init(void) {
    
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;

   
    for (unsigned char i = 0; i < 3; i++) {
        TI_NewTimer(&timerLeds[i]);
    }
}


void rgbLedMotor(unsigned char led) {
   
    static unsigned char state = 0;
    switch(state) {
		case 0:
			TI_ResetTics(timerLeds[led - 2]);
			state = 1;
		break;
		case 1:
			if (TI_GetTics(timerLeds[led - 2]) >= pwmOffTimes[2] ) {
				TI_ResetTics(timerLeds[led - 2]);
				LATB |= (1 << led);
				state = 2;
			}
		break;
		case 2:
			if (TI_GetTics(timerLeds[led - 2]) >= pwmOnTimes[2] ) {
				TI_ResetTics(timerLeds[led - 2]);
				LATB &= ~(1 << led);
				state = 0;
			}
		break;
	}
}
