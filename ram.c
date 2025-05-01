#include <xc.h>

#include "ram.h"
#include "timer.h"
#include "eusart_interface.h"
#include "adcConversion.h"
#include "menu.h"



#define LOG_LENGTH 14

static unsigned char timerRam = 0;
static unsigned char frecuencia = 2;
static unsigned char readFlag = 0;
static unsigned char generalIndex = 0;

void ram_Init(){
    TI_NewTimer(&timerRam);
    TRISAbits.TRISA4 = 0; //ReadNotWrite
    TRISAbits.TRISA5 = 0; //CHIP SELECT
    
    TRISDbits.TRISD2 = 0; //CLOCK RAM
    TRISDbits.TRISD3 = 0; //RESET RAM
    
    LATAbits.LA4 = 1; //ReadNotWrite
    LATAbits.LA5 = 1; //ChipSelect
    LATDbits.LATD2 = 0; //clock
    LATDbits.LATD3 = 0; //reset
  
    
    
}

void setFrecuencia(unsigned char pollingRate){
    frecuencia = pollingRate;
}

void setReadFlag(){
    readFlag = 1;
}


void setPortsInOut(char inOut){
    TRISDbits.TRISD1 = inOut; //LSB bit 0
    TRISDbits.TRISD0 = inOut; //    bit 1
    TRISCbits.TRISC2 = inOut; //    bit 2
    TRISCbits.TRISC1 = inOut; //    bit 3
    TRISCbits.TRISC0 = inOut; //    bit 4
    TRISEbits.TRISE2 = inOut; //    bit 5
    TRISEbits.TRISE1 = inOut; //    bit 6
    TRISEbits.TRISE0 = inOut; //MSB bit 7
    

}

void setReadNotWrite(char readNotWrite){
    LATAbits.LA4 = readNotWrite;

}

void setChipSelect(char chipSelect){
    LATAbits.LA5 = chipSelect;

}

void setClockCounter(char clockRam){
    LATDbits.LATD2 = clockRam;

}

void setResetRam(char resetRam){
    LATDbits.LATD3 = resetRam;

}

void escribirNumeroEnPines(int valor) {
   
    LATDbits.LATD1 = (valor >> 0) & 0x01;
    LATDbits.LATD0 = (valor >> 1) & 0x01;
    LATCbits.LATC2 = (valor >> 2) & 0x01;
    LATCbits.LATC1 = (valor >> 3) & 0x01;
    LATCbits.LATC0 = (valor >> 4) & 0x01;
    LATEbits.LATE2 = (valor >> 5) & 0x01;
    LATEbits.LATE1 = (valor >> 6) & 0x01;
    LATEbits.LATE0 = (valor >> 7) & 0x01;
}

unsigned char leerNumeroDesdePines() {
    unsigned char valor = 0;

    valor |= (PORTDbits.RD1 & 0x01) << 0;
    valor |= (PORTDbits.RD0 & 0x01) << 1;
    valor |= (PORTCbits.RC2 & 0x01) << 2;
    valor |= (PORTCbits.RC1 & 0x01) << 3;
    valor |= (PORTCbits.RC0 & 0x01) << 4;
    valor |= (PORTEbits.RE2 & 0x01) << 5;
    valor |= (PORTEbits.RE1 & 0x01) << 6;
    valor |= (PORTEbits.RE0 & 0x01) << 7;

    return valor;
}

static unsigned char prefixDat[3] = "DAT";
static unsigned char finArray[5] = "FIN\r\n";



void ramMotor(void) {
	static unsigned char state = 0;
	static unsigned char totalSent = 0;
	static unsigned char i = 0;
	

	switch(state) {
		case 0:
			TI_ResetTics(timerRam);
			setChipSelect(1);
			state = 1;
		break;
		case 1:
			if (TI_GetTics(timerRam) > 1000 * frecuencia) {
				setChipSelect(1);
				setReadNotWrite(0);
				setPortsInOut(0);
				state = 2;
			}
			else if (readFlag == 1) {
				setResetRam(1);
				totalSent = 0;
				readFlag = 0;
				state = 7;
			}
		break;
		case 2:
			escribirNumeroEnPines(getTemp());
			state = 3;
		break;
		case 3:
			setChipSelect(0);
			state = 4;
		break;
		case 4:
			setChipSelect(1);
			state = 5;
		break;
		case 5:
			generalIndex++;
			setClockCounter(1);
			state = 6;
		break;
		case 6:
			setClockCounter(0);
			state = 0;
		break;
		case 7:
			if (totalSent < generalIndex) {
				i = 0;
				state = 13;
			}
			else if (totalSent >= generalIndex) {
				setResetRam(0);
				i = 0;
				state = 15;
			}
		break;
		case 8:
			setChipSelect(0);
			state = 9;
		break;
		case 9:
			if (numSentCorrectly()) {
				sendBits((leerNumeroDesdePines() / 10) + '0');
				state = 17;
			}
		break;
		case 10:
			if (numSentCorrectly()) {
				sendBits('\r');
				setChipSelect(1);
				setClockCounter(1);
				totalSent++;
				state = 11;
			}
		break;
		case 11:
			if (numSentCorrectly()) {
				sendBits('\n');
				setClockCounter(0);
				state = 12;
			}
		break;
		case 12:
			state = 7;
		break;
		case 13:
			if (numSentCorrectly()) {
				sendBits(prefixDat[i]);
				i++;
				state = 14;
			}
		break;
		case 14:
			if (i < 3) {
				state = 13;
			}
			else if (i >= 3) {
				setResetRam(0);
				setChipSelect(1);
				setReadNotWrite(1);
				setPortsInOut(1);
				state = 8;
			}
		break;
		case 15:
			if (numSentCorrectly()) {
				sendBits(finArray[i]);
				i++;
				state = 16;
			}
		break;
		case 16:
			if (i < 5) {
				state = 15;
			}
			else if (i >= 5) {
				state = 0;
			}
		break;
		case 17:
			if (numSentCorrectly()) {
				sendBits((leerNumeroDesdePines() % 10) + '0');
				state = 10;
			}
		break;
	}
}


