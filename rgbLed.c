#include <xc.h>
#include "pic18f4321.h"
#include "rgbLed.h"
#include "timer.h"
#include "adcConversion.h"

static unsigned char timerLed;
static unsigned char systemState = 0;

void setSystemState(unsigned char stateNew){
    systemState = stateNew;
}

unsigned char getSystemState(){
    return systemState;
}

void Leds_init(void) {
    // Configurar solo los pines RB2, RB3 y RB4 como salidas
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    TI_NewTimer(&timerLed);
}

static unsigned char modulation[3][3] = { {0,9,0}, {0,0,9}, {9,0,0} };
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
    unsigned char temp = getTemp();

    switch(state) {
        case 0:
            if(systemState == 0) {
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                break;
            }
            TI_ResetTics(timerLed);
            if(temp >= magentaThreshold) {
                LATBbits.LATB2 = 1;
                LATBbits.LATB4 = 1;
            }
            state = 1;
            break;
        case 1:
            if(temp < magentaThreshold && TI_GetTics(timerLed) >= 1) {
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                if(temp < blueThreshold) {
                    LATBbits.LATB3 = 1;
                    setRoomState(0);
                } else if(temp < redThreshold) {
                    LATBbits.LATB4 = 1;
                    setRoomState(1);
                } else if(temp < magentaThreshold) {
                    LATBbits.LATB2 = 1;
                    setRoomState(2);
                }
                state = 0;
            } else if(temp >= magentaThreshold && TI_GetTics(timerLed) >= magentaCounter) {
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                LATBbits.LATB4 = 0;
                LATBbits.LATB2 = 1;
                setRoomState(3);
                TI_ResetTics(timerLed);
                state = 2;

                if(direction == 0) {
                    magentaCounter++;
                    if(magentaCounter >= Period) {
                        direction = 1;
                    }
                } else {
                    magentaCounter--;
                    if(magentaCounter == 0) {
                        direction = 0;
                    }
                }
            }
            break;
        case 2:
            if(TI_GetTics(timerLed) >= (Period - magentaCounter)) {
                state = 0;
            }
            break;
    }
}
