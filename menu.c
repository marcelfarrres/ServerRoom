#include <xc.h>

#include "menu.h"

#include "eusart_interface.h"



#define NUM_OF_ANS 2

static unsigned char inputCommand[50];


void menuMotor(void) {
	static char state = 0;
    
    static unsigned char i = 0;
   

	switch(state) {
		case 0:
			if (numReceivedAtRCREG()) {
                
				i = 0;
				inputCommand[i] = receiveNumber();
				i++;
				state = 1;
			}
		break;
		case 1:
			if (numReceivedAtRCREG()) {
				inputCommand[i] = receiveNumber();
				i++;
				state = 2;
			}
		break;
		case 2:
			if (inputCommand[i - 1] != '\n') {
				state = 1;
			}
			else if (inputCommand[i - 1] == '\n') {
				state = 3;
			}
		break;
		case 3:
            if(inputCommand[0] == 'I'){
                LATAbits.LA3 = 1;
            }else{
                LATAbits.LA3 = 0;
            }
            
            while(!numSentCorrectly());
            sendBits(inputCommand[0]);
            while(!numSentCorrectly());
            sendBits(inputCommand[1]);
            while(!numSentCorrectly());
            sendBits(inputCommand[2]);
            while(!numSentCorrectly());
            sendBits('\n');
			state = 0;
		break;
	}
}



