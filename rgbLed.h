#ifndef _RGBLED_H
#define _RGBLED_H

void Leds_init(void);
void rgbLedMotor2(unsigned char led);
void rgbLedMotor(void);
void setSystemState(unsigned char stateNew);
unsigned char getSystemState();

void setRoomState(unsigned char newState);
unsigned char getRoomState();
void setThresholds(unsigned char blue, unsigned char red, unsigned char magenta);



#endif
