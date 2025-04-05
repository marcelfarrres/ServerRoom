#ifndef _ADT_EUSART_H_
#define	_ADT_EUSART_H_

void initEusart(void);

void sendBits(unsigned char value);

unsigned char numSentCorrectly(void);

unsigned char numReceivedAtRCREG(void);

unsigned char receiveNumber(void);


#endif	

