// include guard for Date.h
#ifndef DATE_H
#define DATE_H

typedef struct Date Date;

Date *CreateDate(int year, int month, int day);
Date *CreateDateFromString(char *str);
void DateFree(Date *this);
char *DateToString(Date *this);
#endif