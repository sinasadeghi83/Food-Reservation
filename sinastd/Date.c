#include "Date_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Parser.h"

// Implementation of Date.h
Date *CreateDate(int year, int month, int day)
{
    Date *this = malloc(sizeof(Date));
    initialiseDate(this, year, month, day);
    return this;
}

Date *CreateDateFromString(char *str)
{
    int year, month, day;
    sscanf(str, "%d-%d-%d", &year, &month, &day);
    if (year == 0)
    {
        sscanf(str, "%d/%d/%d", &year, &month, &day);
    }
    return CreateDate(year, month, day);
}

void DateFree(Date *this)
{
    free(this);
}

// Implementation of Date_internal.h
void initialiseDate(Date *this, int year, int month, int day)
{
    this->year = year;
    this->month = month;
    this->day = day;
}

// Returns Date *this as a string
char *DateToString(Date *this)
{
    char *year = parseInt(this->year), *month = parseInt(this->month), *day = parseInt(this->day);
    char *result = year;
    strcat(result, "-");
    if (this->month < 10)
    {
        strcat(result, "0");
    }
    strcat(result, month);
    strcat(result, "-");
    if (this->day < 10)
    {
        strcat(result, "0");
    }
    strcat(result, day);
    free(year);
    free(month);
    free(day);
    return result;
}