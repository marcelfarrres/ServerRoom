#ifndef _EPROM_INTERFACE_H_
#define	_EPROM_INTERFACE_H_

void writeEEPROM(unsigned char i, unsigned char data);
char stillWriting();
unsigned char  readEPROM(char i);

void epromMotor();
#endif	

