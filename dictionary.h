#ifndef _DICTIONARY_H
#define _DICTIONARY_H

//RAM:
void resetRam();
void ram_Init();
void setFrecuencia(unsigned char pollingRate);
void setReadFlag();
void setPortsInOut(char inOut);
void setReadNotWrite(char readNotWrite);
void setChipSelect(char chipSelect);
void setClockCounter(char clockRam);
void setResetRam(char resetRam);
void escribirNumeroEnPines(int valor);
unsigned char leerNumeroDesdePines();
void ramMotor(void);

//RGB Led:
void Leds_init(void);
void setSystemState(unsigned char stateNew);
unsigned char getSystemState();
void setRoomState(unsigned char newState);
unsigned char getRoomState();
void setThresholds(unsigned char blue, unsigned char red, unsigned char magenta);
void rgbLedMotor(void);

//Timer:
void TI_NewTimer(unsigned char *timer);
void TI_ResetTics(unsigned char timer);
unsigned int TI_GetTics(unsigned char timer);

//ADC:
unsigned char getTemp();

//Menu:
void displayMenu();
void processMenuInput(unsigned char input);
void resetMenuState();

//Logic Joystick:
void initJoystick();
unsigned char readJoystick();
void setJoystickThreshold(unsigned char threshold);

//Fan:
void initFan(void);
void setFanSpeed(unsigned char speed);
unsigned char getFanSpeed();
void fanMotor(void);

//Eprom:
void initEprom();
void writeToEprom(unsigned char address, unsigned char data);
unsigned char readFromEprom(unsigned char address);
void clearEprom();
void epromMotor(void);

#endif
