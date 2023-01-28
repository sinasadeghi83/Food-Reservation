// include guard for Date.h
#ifndef DATE_H
#define DATE_H

#include <stdbool.h>

typedef struct Date Date;

struct Date
{
    int year;
    int month;
    int day;
};

void initialiseDate(Date *this, int year, int month, int day);
Date *CreateDate(int year, int month, int day);
Date *CreateDateFromString(char *str);
void DateFree(Date *this);
char *DateToString(Date *this);
bool DateIsValid(Date *this);
#endif