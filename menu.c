#include <xc.h>

#include "menu.h"

#include "eusart_interface.h"
#include "eprom.h"



#define LOG_LENGTH 14
#define INITIAL_LOG_POSITION 3
#define FINISH_SEND_LENGTH 5
#define TOTAL_LOGS_POSITION 0
#define ACTUAL_LOGS_POSITION 1


static unsigned char finArray[FINISH_SEND_LENGTH] = "FIN\r\n";
static unsigned char inputCommand[50];

static unsigned char totalLogsStored = 3;

static unsigned char arrayPrefixes[4][3] = {
    "LOG",
    "DOW",
    "LEF",
    "RIG",
};


char checkINIT(){
    if(inputCommand[0] == 'I' && inputCommand[1] == 'N' && inputCommand[2] == 'I' && inputCommand[3] == 'T' ){
        return 1;
    }else{
        return 0;
    }
}

char checkLOGS(){
    if(inputCommand[0] == 'L' && inputCommand[1] == 'O' && inputCommand[2] == 'G' && inputCommand[3] == 'S' ){
        return 1;
       

    }else{
        
        return 0;
    }
}



void menuMotor(void) {
	static unsigned char state = 0;
    
    static unsigned char i = 0;
    static unsigned char u = 0;
    static unsigned char a = 0;
    static unsigned char totalLogsSended = 0;
   

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
			state = 0;
			if (checkLOGS()) {
				i = 0;
				u = 0;
				a = INITIAL_LOG_POSITION;
				totalLogsSended = 0;
				state = 4;
			}
		break;
		case 4:
			if (numSentCorrectly()) {
				sendBits(arrayPrefixes[0][i]);
				i++;
				state = 5;
			}
		break;
		case 5:
			if (i < 3) {
				state = 4;
			}
			else if (i >= 3) {
				state = 6;
			}
		break;
		case 6:
			if (numSentCorrectly()) {
				sendBits(readEPROM(a + u));
				a++;
				state = 7;
			}
		break;
		case 7:
			if (a < LOG_LENGTH + INITIAL_LOG_POSITION) {
				state = 6;
			}
			else if (a >= LOG_LENGTH + INITIAL_LOG_POSITION) {
				a = INITIAL_LOG_POSITION;
				u = u + LOG_LENGTH;
				totalLogsSended++;
				state = 8;
			}
		break;
		case 8:
			if (numSentCorrectly()) {
				sendBits('\r');
				state = 9;
			}
		break;
		case 11:
			if (numSentCorrectly()) {
				sendBits(finArray[a]);
				a++;
				state = 12;
			}
		break;
		case 12:
			if (a < FINISH_SEND_LENGTH) {
				state = 11;
			}
			else if (a >= FINISH_SEND_LENGTH) {
				state = 0;
			}
		break;
		case 9:
			if (numSentCorrectly()) {
				sendBits('\n');
				state = 10;
			}
		break;
		case 10:
			if (totalLogsSended < totalLogsStored) {
				i = 0;
				state = 4;
			}
			else if (totalLogsSended >= totalLogsStored) {
				a = 0;
				state = 11;
			}
		break;
	}
}



