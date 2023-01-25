// include guard for Date_internal.h
#ifndef DATE_INTERNAL_H
#define DATE_INTERNAL_H
#include "Date.h"

struct Date
{
    int year;
    int month;
    int day;
};

void initialiseDate(Date *this, int year, int month, int day);

#endif