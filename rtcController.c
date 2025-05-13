#include <xc.h>
#include "pic18f4321.h"
#include "rtcController.h"

#include "TAD_I2C.h"



void setRTC(
    unsigned char sec,   // 0-59
    unsigned char min,   // 0-59
    unsigned char hour,  // 0-23
    unsigned char day,   // 1-7 (día de la semana)
    unsigned char date,  // 1-31
    unsigned char month, // 1-12
    unsigned char year   // 0-99 (ej: 24 -> 2024)
) {
    StartI2C(0xD0);         // Dirección RTC escritura
    I2C_Write(0x00);        // Comienza en el registro de segundos

    I2C_Write(sec);
    I2C_Write(min);
    I2C_Write(hour);
    I2C_Write(day);
    I2C_Write(date);
    I2C_Write(month);
    I2C_Write(year);

    I2C_Stop_();
}

void getRTC(
    unsigned char *sec,
    unsigned char *min,
    unsigned char *hour,
    unsigned char *day,
    unsigned char *date,
    unsigned char *month,
    unsigned char *year
) {
    StartI2C(0xD0);        // Dirección escritura para setear puntero
    I2C_Write(0x00);       // Comenzamos desde el registro 0
    ReStartI2C_(0xD1);     // Dirección lectura

    *sec   = I2C_Read(0);
    *min   = I2C_Read(0);
    *hour  = I2C_Read(0);
    *day   = I2C_Read(0);
    *date  = I2C_Read(0);
    *month = I2C_Read(0);
    *year  = I2C_Read(1);  // Último byte con NACK

    I2C_Stop_();
}
void enableMinuteAlarm() {
    
    StartI2C(0xD0); 
    I2C_Write(0x07);
    
   
    I2C_Write(0b01011001); 
    I2C_Write(0b10000000); 
    I2C_Write(0b10000000);
    I2C_Write(0b10000000);
    I2C_Stop_();
    
   
    StartI2C(0xD0);
    I2C_Write(0x0E);
    I2C_Write(0b00000101); 
    I2C_Stop_();
}


void clearAlarm2Flag() {
    StartI2C(0xD0);
    I2C_Write(0x0F); // Dirección del registro de estado
    I2C_Write(0b00000000); // Limpiar todos los flags
    I2C_Stop_();
}

