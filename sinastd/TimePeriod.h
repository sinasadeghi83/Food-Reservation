// include guard for TimePeriod.h
#ifndef TIMEPERIOD_H
#define TIMEPERIOD_H

#include <stdbool.h>

typedef struct TimePeriod TimePeriod;

struct TimePeriod
{
    int startHour;
    int startMin;
    int endHour;
    int endMin;
};

// TimePeriod initialiser
void TimePeriodInitialise(TimePeriod *this, int startHour, int startMin, int endHour, int endMin);
void TimePeriodFree(TimePeriod *this);
TimePeriod *TimePeriodCreate(int startHour, int startMin, int endHour, int endMin);
TimePeriod *TimePeriodFromString(char *str);
bool TimePeriodValidate(TimePeriod *this);
char *TimePeriodToString(TimePeriod *this);
#endif