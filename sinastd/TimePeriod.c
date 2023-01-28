#include "TimePeriod.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Create TimePeriod
TimePeriod *TimePeriodCreate(int startHour, int startMin, int endHour, int endMin)
{
    TimePeriod *this = malloc(sizeof(TimePeriod));
    TimePeriodInitialise(this, startHour, startHour, endHour, endMin);
    return this;
}

// TimePeriod constructor
void TimePeriodInitialise(TimePeriod *this, int startHour, int startMin, int endHour, int endMin)
{
    this->startHour = startHour;
    this->startMin = startMin;
    this->endHour = endHour;
    this->endMin = endMin;
}

// TimePeriod destructor
void TimePeriodFree(TimePeriod *this)
{
    if (this == NULL)
        return;
    free(this);
}

// Create time period from string
// Format: HHMM-HHMM
// Return NULL if string is NULL
TimePeriod *TimePeriodFromString(char *str)
{
    // Check if string is NULL
    if (str == NULL)
    {
        return NULL;
    }
    int startHour, startMin, endHour, endMin;
    sscanf(str, "%2d%2d-%2d%2d", &startHour, &startMin, &endHour, &endMin);
    return TimePeriodCreate(startHour, startMin, endHour, endMin);
}

// Convert time period to string
// Format: HHMM-HHMM
// Return NULL if time period is NULL
char *TimePeriodToString(TimePeriod *this)
{
    // check if time period is NULL
    if (this == NULL)
    {
        return NULL;
    }
    char *str = malloc(9);
    sprintf(str, "%02d%02d-%02d%02d", this->startHour, this->startMin, this->endHour, this->endMin);
    return str;
}

// Validate time period
// Return true if time period is valid or NULL
bool TimePeriodValidate(TimePeriod *this)
{
    // Check if time period is NULL
    if (this == NULL)
    {
        return true;
    }
    // Check if time numbers are valid
    if (this->startHour < 0 || this->startHour > 23 || this->startMin < 0 || this->startMin > 59 || this->endHour < 0 || this->endHour > 23 || this->endMin < 0 || this->endMin > 59)
    {
        return false;
    }

    // Check if start time is before end time
    if (this->startHour > this->endHour)
    {
        return false;
    }
    else if (this->startHour == this->endHour && this->startMin > this->endMin)
    {
        return false;
    }

    return true;
}