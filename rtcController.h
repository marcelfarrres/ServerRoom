#ifndef _RTCCONTROLLER_H
#define _RTCCONTROLLER_H


void setRTC(
    unsigned char sec,   // 0-59
    unsigned char min,   // 0-59
    unsigned char hour,  // 0-23
    unsigned char day,   // 1-7 (día de la semana)
    unsigned char date,  // 1-31
    unsigned char month, // 1-12
    unsigned char year   // 0-99 (ej: 24 -> 2024)
);

void getRTC(
    unsigned char *sec,
    unsigned char *min,
    unsigned char *hour,
    unsigned char *day,
    unsigned char *date,
    unsigned char *month,
    unsigned char *year
);

void enableMinuteAlarm();
void clearAlarmFlag();
#endif
