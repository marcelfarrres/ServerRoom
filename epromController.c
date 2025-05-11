#include <xc.h>

#include "epromController.h"
#include "eprom.h"
#include "rgbLed.h"
#include "eusart_interface.h"
#include "adcConversion.h"
#include "menu.h"
#include "rtcController.h"

#define INITIAL_LOG_POSITION 3

static unsigned char initialPosition = 0;
static unsigned char magentaLimit = 30;

void setMagentaLimit(unsigned char newLimit){
    magentaLimit = newLimit;
}



void epromMotor(void) {
	static unsigned char state = 0;
	static unsigned char done = 0;
	static unsigned char totalLogsStored = 0;
	static unsigned char lastLogStored = 0;

	static unsigned char hour = 0x09;
	static unsigned char min = 0x23;
	static unsigned char sec = 0x54;
	static unsigned char day = 0x24;
	static unsigned char month = 0x07;
	static unsigned char year = 0x25;
	static unsigned char date = 0x25;

	switch(state) {
		case 0:
			if (getTemp() < (magentaLimit - 3) && done == 1) {
				done = 0;
				state = 2;
			}
			else if (getRoomState() == 3 && done == 0) {
				done = 1;
				if(readEPROM(0) == 0xFF){
					writeEEPROM(0,0);
					while(stillWriting());
					if(readEPROM(1) == 0xFF){
						writeEEPROM(1,0);
					}
				}
				state = 19;
			}
		break;
		case 1:
            getRTC(&sec, &min, &hour, &date, &day, &month, &year);
			state = 3;
		break;
		case 2:
			state = 0;
		break;
		case 3:
			writeEEPROM(initialPosition, hour >> 4);
			state = 4;
		break;
		case 4:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 1, hour & 0x0F);
				state = 5;
			}
		break;
		case 5:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 2, min >> 4);
				state = 6;
			}
		break;
		case 6:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 3, min & 0x0F);
				state = 7;
			}
		break;
		case 7:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 4, sec >> 4);
				state = 8;
			}
		break;
		case 8:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 5, sec & 0x0F);
				state = 9;
			}
		break;
		case 9:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 6, day >> 4);
				state = 10;
			}
		break;
		case 10:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 7, day & 0x0F);
				state = 11;
			}
		break;
		case 11:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 8, month >> 4);
				state = 12;
			}
		break;
		case 12:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 9, month & 0x0F);
				state = 13;
			}
		break;
		case 13:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 10, 2);
				state = 14;
			}
		break;
		case 14:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 11, 0);
				state = 15;
			}
		break;
		case 15:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 12, year >> 4);
				state = 16;
			}
		break;
		case 16:
			if (!stillWriting()) {
				writeEEPROM(initialPosition + 13, year & 0x0F);
				state = 17;
			}
		break;
		case 17:
			if (!stillWriting()) {
				if(lastLogStored >= 14){
					writeEEPROM(1, 0);
				}else{
					writeEEPROM(1, lastLogStored + 1);
				}
				state = 18;
			}
		break;
		case 18:
			if (!stillWriting()) {
				if(totalLogsStored >= 15){
				}else{
					writeEEPROM(0, totalLogsStored + 1);
				}
				state = 0;
			}
		break;
		case 19:
			if (!stillWriting()) {
				totalLogsStored = readEPROM(0);
				lastLogStored = readEPROM(1);
				initialPosition = (lastLogStored * 14) + INITIAL_LOG_POSITION;
				state = 1;
			}
		break;
	}
}
