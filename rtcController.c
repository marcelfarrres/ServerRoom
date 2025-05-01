#include <xc.h>
#include "pic18f4321.h"
#include "rtcController.h"

#include "TAD_I2C.h"

unsigned char toBCD(unsigned char n) {
    unsigned char dec = 0;
    while (n >= 10) { n -= 10; dec++; }
    return (dec << 4) | n;
}

unsigned char fromBCD(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}


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

    I2C_Write(toBCD(sec));
    I2C_Write(toBCD(min));
    I2C_Write(toBCD(hour));
    I2C_Write(toBCD(day));
    I2C_Write(toBCD(date));
    I2C_Write(toBCD(month));
    I2C_Write(toBCD(year));

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

    *sec   = fromBCD(I2C_Read(0));
    *min   = fromBCD(I2C_Read(0));
    *hour  = fromBCD(I2C_Read(0));
    *day   = fromBCD(I2C_Read(0));
    *date  = fromBCD(I2C_Read(0));
    *month = fromBCD(I2C_Read(0));
    *year  = fromBCD(I2C_Read(1));  // Último byte con NACK

    I2C_Stop_();
}

