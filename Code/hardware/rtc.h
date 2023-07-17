#ifndef RTC_H
#define RTC_H

#include "pico/stdlib.h"

typedef struct time
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Time;

bool RTC_Init(void);

bool RTC_SetTime(Time t);
bool RTC_GetTime(Time* t);

bool RTC_GetPDPUTime(Time* t_pd, Time* t_pu);

void RTC_PrintTime(Time* t, char* s, size_t strlen);

#endif /* RTC_H */
