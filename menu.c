#include <xc.h>

#include "menu.h"

#include "eusart_interface.h"
#include "eprom.h"
#include "adcConversion.h"
#include "rgbLed.h"



#define LOG_LENGTH 14
#define INITIAL_LOG_POSITION 3
#define FINISH_SEND_LENGTH 5
#define TOTAL_LOGS_POSITION 0
#define INDEX_LOGS_POSITION 1


static unsigned char finArray[FINISH_SEND_LENGTH] = "FIN\r\n";
static unsigned char inputCommand[50];

static unsigned char totalLogsStored;
static unsigned char lastLogStored;

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

char checkGraf(){
    if(inputCommand[0] == 'G' && inputCommand[1] == 'R' && inputCommand[2] == 'A' && inputCommand[3] == 'F' ){
        return 1;
    }else{
        return 0;
    }
}


void sendIntAsASCII(unsigned int num) {
    unsigned char d;

    // Miles (solo si el n�mero es >= 1000)
    if (num >= 1000) {
        d = num / 1000;
        sendBits(d + '0');
        while (!numSentCorrectly());
        num %= 1000;
    }

    // Centenas
    if (num >= 100 || num >= 1000) {
        d = num / 100;
        sendBits(d + '0');
        while (!numSentCorrectly());
        num %= 100;
    } else if (num >= 10) {
        // Si no hay centenas pero hay decenas, poner '0' delante
        sendBits('0');
        while (!numSentCorrectly());
    }

    // Decenas
    if (num >= 10) {
        d = num / 10;
        sendBits(d + '0');
        while (!numSentCorrectly());
        num %= 10;
    } else if (num < 10 && num < 100) {
        // Si es un n�mero como 007, a�adir ceros delante
        sendBits('0');
        while (!numSentCorrectly());
    }

    // Unidades
    sendBits(num + '0');
    while (!numSentCorrectly());
}

void printTemp(){
    sendIntAsASCII(getTemp());
    while (!numSentCorrectly());
    sendBits('\r');
    while (!numSentCorrectly());
    sendBits('\n');
}

void printCommand(){
    while(!numSentCorrectly());
            sendBits(arrayPrefixes[0][0]);
            while(!numSentCorrectly());
            sendBits(arrayPrefixes[0][1]);
            while(!numSentCorrectly());
            sendBits(arrayPrefixes[0][2]);
            while(!numSentCorrectly());
            sendBits('-');
            for(int re = 0; re < 23; re++){
                while(!numSentCorrectly());
                sendBits(inputCommand[re]);

            }
            while(!numSentCorrectly());
            sendBits('-');
            while(!numSentCorrectly());
            sendBits('\n');
}

void printWord(unsigned char word){
    
           
            while(!numSentCorrectly());
            sendBits('-');
            
            
            unsigned char dec = 0;
            unsigned char num = word;
    while (num >= 10) {
        num -= 10;
        dec++;
    }
    while(!numSentCorrectly());
    // Decenas
    sendBits(dec + '0');
    while (!numSentCorrectly());

    // Unidades
    sendBits(num + '0');
    while (!numSentCorrectly());
            
            
            while(!numSentCorrectly());
            sendBits('-');
            while(!numSentCorrectly());
            sendBits('\n');
}

static unsigned char year;
static unsigned char month;
static unsigned char day;
static unsigned char hour;
static unsigned char minute;
static unsigned char lowThres;
static unsigned char midThres;
static unsigned char highThres;
static unsigned char carry;
static unsigned char pollingRate;




void menuMotor(void) {
	static unsigned char state = 0;
    
    static unsigned char i = 0;
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
				a = INITIAL_LOG_POSITION;
				totalLogsSended = 0;
				totalLogsStored = readEPROM(0);
				lastLogStored = readEPROM(1);
				state = 10;
			}
			else if (checkINIT()) {
				state = 15;
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
				sendBits(readEPROM(a + (lastLogStored * 14)) + '0');
				a++;
				state = 13;
			}
		break;
		case 7:
			if (a < LOG_LENGTH + INITIAL_LOG_POSITION) {
				state = 6;
			}
			else if (a >= LOG_LENGTH + INITIAL_LOG_POSITION) {
				a = INITIAL_LOG_POSITION;
				totalLogsSended++;
				lastLogStored++;
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
			if (totalLogsSended < totalLogsStored && lastLogStored < totalLogsStored) {
				i = 0;
				state = 4;
			}
			else if (totalLogsSended >= totalLogsStored) {
				a = 0;
				state = 11;
			}
			else if (totalLogsSended < totalLogsStored && lastLogStored >= totalLogsStored) {
				i = 0;
				lastLogStored = 0;
				state = 4;
			}
		break;
		case 13:
			if (numSentCorrectly()) {
				sendBits(readEPROM(a + (lastLogStored * 14)) + '0');
				a++;
				state = 14;
			}
		break;
		case 14:
			if (numSentCorrectly()) {
				if(a < 9){
					sendBits(':');
				}else if(a == 9){
					sendBits(' ');
				}else if(a > 9 && a < 15){
					sendBits('/');
				}
				state = 7;
			}
		break;
		case 15:
			year = ((inputCommand[7] - '0') * 10 ) + (inputCommand[8] - '0');
			month = ((inputCommand[10] - '0') * 10 ) + (inputCommand[11] - '0');
			day = ((inputCommand[13] - '0') * 10 ) + (inputCommand[14] - '0');
			hour = ((inputCommand[16] - '0') * 10 ) + (inputCommand[17] - '0');
			minute = ((inputCommand[19] - '0') * 10 ) + (inputCommand[20] - '0');
			carry = 0;
			state = 16;
		break;
		case 16:
			if(inputCommand[23] == '$'){
				pollingRate = (inputCommand[22] - '0');
			}else{
				pollingRate = ((inputCommand[22] - '0') * 10) + (inputCommand[23] - '0');
				carry++;
			}
			state = 17;
		break;
		case 17:
			if(inputCommand[25 + carry] == '$'){
				lowThres = (inputCommand[24 + carry] - '0');
			}else{
				lowThres = ((inputCommand[24 + carry] - '0') * 10) + (inputCommand[25 + carry] - '0');
				carry++;
			}
			state = 18;
		break;
		case 18:
			if(inputCommand[27 + carry] == '$'){
				midThres = (inputCommand[26 + carry] - '0');
			}else{
				midThres = ((inputCommand[26 + carry] - '0') * 10) + (inputCommand[27 + carry] - '0');
				carry++;
			}
			state = 19;
		break;
		case 19:
			if(inputCommand[29 + carry] == '$'){
				highThres = (inputCommand[28 + carry] - '0');
			}else{
				highThres = ((inputCommand[28 + carry] - '0') * 10) + (inputCommand[29 + carry] - '0');
				carry++;
			}
			setThresholds(lowThres, midThres, highThres);
			state = 0;
		break;
	}
}



